import 'dart:async';
import 'dart:convert';
import 'dart:isolate';
import 'package:dixom/driver/device_rx_tx/command_msg.dart';
import 'package:dixom/pages/top_bar/connection/backend/device_connection.dart';
import 'package:dixom/dataBase/device_constant.dart';
import 'package:dixom/main.dart';
import 'package:flutter/services.dart';
import 'package:quick_usb/quick_usb.dart';
import '../device_rx_tx/reciever.dart';

const int inTimeOut = 30000;
const int outTimeOut = 500;
const String hidRx = "HidRx";
const String hidTx = "HidTx";
final UsbCustomHid usbHid = UsbCustomHid();

final InterfaceConnectionInfo usbHidConnectionInfo = InterfaceConnectionInfo(provider: usbHidProvider);

bool blockConnectIn = false;
bool blockConnectOut = false;

void stopUsb() {
  usbHid.sendCommandInIsolyatePort(WorkState.disconnected);
  usbHid.sendCommandOutIsolyatePort(WorkState.disconnected);
}

void disconnectUsb() {
  sendData("0 17 ${ExchangeInterface.usbHid.index}");

  Future.delayed(const Duration(milliseconds: 1000), () {
    stopUsb();
  });
}

void connectUsb() async {
  if (await deviceAvailabilityCheck() == true) {
    usbHid.init();
  } else {
    appRef.read(usbHidProvider.notifier).setStatus(WorkState.notFound);
  }
}

WorkState sendData(dynamic message) {
  if (message is String) {
    List<int> bytes = utf8.encode(message).toList();
    usbHid.sendCommandOutIsolyatePort(bytes);
  } else if (message is List<int>) {
    usbHid.sendCommandOutIsolyatePort(message);
  } else if (message is CommandMsg) {
    usbHid.sendCommandOutIsolyatePort(message.dataUtf8);
  }
  return WorkState.ok;
}

class UsbCustomHid {
  ///Отправка данных на платформу
  SendPort? hidTxPort;
  //Получение данных из платформы
  SendPort? hidRxPort;

  void init() async {
    // Создаем изолят и получаем его SendPort
    try {
      RootIsolateToken rootIsolateToken = RootIsolateToken.instance!;

      if (blockConnectIn == false) {
        blockConnectIn = true;
        hidRxPort ??= await initIsolate(
          usbHhidRx,
          rootIsolateToken,
          hidRx,
        ) as SendPort;
      }

      if (blockConnectOut == false) {
        blockConnectOut = true;
        hidTxPort ??= await initIsolate(
          usbHhidTx,
          rootIsolateToken,
          hidTx,
        ) as SendPort;
      }
      // ignore: empty_catches
    } catch (e) {}
  }

  void sendCommandInIsolyatePort(dynamic message) {
    hidRxPort?.send(message);
  }

  void sendCommandOutIsolyatePort(dynamic message) {
    hidTxPort?.send(message);
  }

  ///============== АВТОПОДКЛЮЧЕНИЕ К ПЛАТФОРМЕ================///

  static Timer? autoConnect;
  static bool autoContState = false;
  static bool isConnected = true;
  void startAutoConnect() {
    if (autoContState == false) {
      autoContState = true;
      autoConnect = Timer.periodic(const Duration(seconds: 3), (timer) async {
        if (appRef.read(usbHidProvider.select((value) => value.status)) != WorkState.connected) {
          if (await deviceAvailabilityCheck() == true) {
            usbHid.init();
            isConnected = true;
          } else {
            /*if (isConnected) {
              appRef
                  .read(usbHidProvider.notifier)
                  .setStatus(WorkState.notFound);
            }
            isConnected = false;*/
          }
        } else {
          autoConnect!.cancel();
          autoContState = false;
        }
      });
    }
  }

  void stopAutoConenct() {
    if (autoConnect != null) {
      autoConnect!.cancel();
      autoContState = false;
    }
  }
}

Future<dynamic> initIsolate(
  void Function(List) isolyate,
  RootIsolateToken token,
  String id,
) async {
  // Completer для получения SendPort из изолята
  Completer completer = Completer<SendPort>();
  // ReceivePort чтобы прослушивать изолят
  ReceivePort receivePort = ReceivePort();
  // Создаем изолят и отправляем в него SendPort из main
  Isolate thread = await Isolate.spawn(
    isolyate,
    [token, receivePort.sendPort],
    debugName: id,
  );

  // Начинаем слушать изолят
  receivePort.listen((dynamic message) {
    try {
      if (message is SendPort) {
        // отправляем SendPort в main через return
        completer.complete(message);
      } else if (message is WorkState) {
        if (message == WorkState.ok) {
          if (thread.debugName == hidTx) {
            usbHidConnectionInfo.setConnectionInfo(InterfaceState.succesOut);
          } else if (thread.debugName == hidRx) {
            usbHidConnectionInfo.setConnectionInfo(InterfaceState.succesIn);
          }
        } else if (message == WorkState.error) {
          if (thread.debugName == hidTx) {
            usbHidConnectionInfo.setConnectionInfo(InterfaceState.errorOut);
          } else if (thread.debugName == hidRx) {
            usbHidConnectionInfo.setConnectionInfo(InterfaceState.errorIn);
          }
        } else if (message == WorkState.disconnected) {
          if (thread.debugName == hidTx) {
            usbHid.hidTxPort = null;
            blockConnectOut = false;
            usbHidConnectionInfo.setConnectionInfo(InterfaceState.statusOut, workState: WorkState.disconnected);
          } else if (thread.debugName == hidRx) {
            usbHid.hidRxPort = null;
            blockConnectIn = false;

            usbHidConnectionInfo.setConnectionInfo(InterfaceState.statusIn, workState: WorkState.disconnected);
          }

          final repository = appRef.read(usbHidProvider);

          if (repository.statusIn != WorkState.connected || repository.statusOut != WorkState.connected) {
            usbHidConnectionInfo.setConnectionInfo(InterfaceState.status, workState: WorkState.disconnected);

            if (appRef.read(connectionState.select((value) => value.autoConnectionHid))) {
              usbHid.startAutoConnect();
            }
          }

          if (thread.debugName == hidTx) {
            //usbHid.sendCommandInIsolyatePort(WorkState.disconnected);
          } else if (thread.debugName == hidRx) {
            usbHid.sendCommandOutIsolyatePort(WorkState.disconnected);
          }

          thread.kill(priority: Isolate.immediate);
        } else if (message == WorkState.connected) {
          if (thread.debugName == hidTx) {
            blockConnectOut = false;

            usbHidConnectionInfo.setConnectionInfo(InterfaceState.statusOut, workState: WorkState.connected);
          } else if (thread.debugName == hidRx) {
            blockConnectIn = false;

            usbHidConnectionInfo.setConnectionInfo(InterfaceState.statusIn, workState: WorkState.connected);
          }

          final repository = appRef.read(usbHidProvider);

          if (repository.statusIn == WorkState.connected && repository.statusOut == WorkState.connected) {
            usbHidConnectionInfo.setConnectionInfo(InterfaceState.status, workState: WorkState.connected);
          }
        } /* else if (message == WorkState.disconnected) {
         
        }*/
      } else if (message is CommandMsg) {
        distributor.receiver(message);
      }
    } catch (e) {
      // Handle the message appropriately if it is not a CommandMsg.
    }
  });
  // отправляем SendPort в main
  return completer.future;
}

Future<bool> deviceAvailabilityCheck() async {
  bool avilable = false;
  if (await QuickUsb.init()) {
    List<UsbDevice> deviceList = await QuickUsb.getDeviceList();
    int deviceNum = 1000;
    for (int i = 0; i < deviceList.length; i++) {
      if (deviceList[i].vendorId == 61525 && (deviceList[i].productId == 6 || deviceList[i].productId == 7)) {
        deviceNum = i;
      }
    }
    if (deviceNum != 1000) {
      avilable = true;
    }
    await QuickUsb.exit();
  }
  return avilable;
}

Future<List<dynamic>> usbHidConnect(int direction) async {
  UsbEndpoint? usbEndpoint;
  bool connectionState = false;
  if (await QuickUsb.init()) {
    List<UsbDevice> deviceList = await QuickUsb.getDeviceList();
    int deviceNum = 1000;
    for (int i = 0; i < deviceList.length; i++) {
      if (deviceList[i].vendorId == 61525 && (deviceList[i].productId == 6 || deviceList[i].productId == 7)) {
        deviceNum = i;
      }
    }
    if (deviceNum != 1000) {
      await QuickUsb.getDeviceDescription(deviceList[deviceNum]);
      if (await QuickUsb.openDevice(deviceList[deviceNum])) {
        UsbConfiguration configuration = await QuickUsb.getConfiguration(0);
        int intrfaceNum = 1;
        if (deviceList[deviceNum].productId == 7) {
          intrfaceNum = 0;
        }
        if (await QuickUsb.claimInterface(configuration.interfaces[intrfaceNum])) {
          if (direction == UsbEndpoint.DIRECTION_IN) {
            usbEndpoint = configuration.interfaces[intrfaceNum].endpoints
                .firstWhere((e) => e.direction == UsbEndpoint.DIRECTION_IN);
          } else {
            usbEndpoint = configuration.interfaces[intrfaceNum].endpoints
                .firstWhere((e) => e.direction == UsbEndpoint.DIRECTION_OUT);
          }
          connectionState = true;
        }
      }
    }
  }
  return [connectionState, usbEndpoint];
}

void usbHhidTx(List args) async {
  RootIsolateToken rootIsolateToken = args[0] as RootIsolateToken;
  BackgroundIsolateBinaryMessenger.ensureInitialized(rootIsolateToken);
  SendPort portFromIsolateToMain = args[1] as SendPort;
  ReceivePort portFromMainToIsolate = ReceivePort();
  portFromIsolateToMain.send(portFromMainToIsolate.sendPort);

  List<dynamic> state = await usbHidConnect(UsbEndpoint.DIRECTION_OUT);
  bool connectionState = state[0] as bool;

  if (connectionState == true) {
    UsbEndpoint usbEndpointOut = state[1] as UsbEndpoint;
    portFromIsolateToMain.send(WorkState.connected);

    void dataSend(dynamic word) async {
      if (connectionState) {
        int maxPackedLength = 65;
        final data = Uint8List(maxPackedLength);
        const int offset = 1;

        if (word is String) {
          final List<int> wordBytes = utf8.encode(word);
          if (wordBytes.length < maxPackedLength) {
            data.setRange(offset, offset + wordBytes.length, wordBytes);
          }
        } else if (word is List<int>) {
          if (word.length < maxPackedLength) {
            data.setRange(offset, offset + word.length, word);
          }
        }
        data[0] = 0x01; //Репорт usb контрола на передачу данных по USB в STM32
        try {
          await QuickUsb.bulkTransferOut(usbEndpointOut, data, timeout: outTimeOut);
          portFromIsolateToMain.send(WorkState.ok);
        } catch (e) {
          portFromIsolateToMain.send(WorkState.error);
          if (e == "bulkTransferIn error: LIBUSB_ERROR_TIMEOUT") {
          } else {
            await QuickUsb.exit();
            portFromIsolateToMain.send(WorkState.disconnected);
            connectionState = false;
          }
        }
      }
    }

    //Слушать изолят из main
    portFromMainToIsolate.listen((dynamic message) {
      if (message is WorkState) {
        //Удаление изолята
        if (message == WorkState.disconnected) {
          connectionState = false;
          QuickUsb.exit();
          portFromIsolateToMain.send(WorkState.disconnected);
        }
      } else {
        dataSend(message);
      }
    });
  } else {
    portFromIsolateToMain.send(WorkState.disconnected);
  }
}

void usbHhidRx(List args) async {
  RootIsolateToken rootIsolateToken = args[0] as RootIsolateToken;
  // Зарегистрировать фоновый изолят с корневым изолятом.
  BackgroundIsolateBinaryMessenger.ensureInitialized(rootIsolateToken);
  // ReceivePort чтобы отправлять данные в main
  SendPort portFromIsolateToMain = args[1] as SendPort;
  ReceivePort portFromMainToIsolate = ReceivePort();
  // отправляем в maim SendPort изолята
  portFromIsolateToMain.send(portFromMainToIsolate.sendPort);

  List<dynamic> state = await usbHidConnect(UsbEndpoint.DIRECTION_IN);
  bool connectionState = state[0] as bool;

  if (connectionState == true) {
    UsbEndpoint usbEndpointIn = state[1] as UsbEndpoint;
    portFromIsolateToMain.send(WorkState.connected);

    CommandMsg msg;

    Uint8List rxData = Uint8List(255);
    int partDataSize = 0;

    Timer timer = Timer.periodic(const Duration(microseconds: 1), (timer) async {
      try {
        Uint8List transferIn = await QuickUsb.bulkTransferIn(usbEndpointIn, 65, timeout: inTimeOut);

        int msgType = transferIn[1];
        int lenght = transferIn[2];
        int msgSize = 0;

        if (msgType == FrameEncoding.byteWhole) {
          while (msgSize < lenght) {
            rxData[msgSize] = transferIn[msgSize + 3];
            msgSize++;
          }
          msg = byteToCommandMsg(msgSize, rxData, interface: ExchangeInterface.usbHid);
          //Отправляем данные в main
          portFromIsolateToMain.send(msg);
          portFromIsolateToMain.send(WorkState.ok);
        } else if (msgType == FrameEncoding.bytePartStart) {
          //начало части кадра
          partDataSize = 0;
          while (msgSize < lenght) {
            rxData[partDataSize] = transferIn[msgSize + 3];
            partDataSize++;
            msgSize++;
          }
        } else if (msgType == FrameEncoding.bytePart) {
          //середина части кадра, если есть
          while (msgSize < lenght) {
            rxData[partDataSize] = transferIn[msgSize + 3];
            partDataSize++;
            msgSize++;
          }
        } else if (msgType == FrameEncoding.bytePartEnd) {
          //конец части кадра
          while (msgSize < lenght) {
            rxData[partDataSize] = transferIn[msgSize + 3];
            partDataSize++;
            msgSize++;
          }

          msg = byteToCommandMsg(partDataSize, rxData, interface: ExchangeInterface.usbHid);
          //Отправляем данные в main
          portFromIsolateToMain.send(msg);
          portFromIsolateToMain.send(WorkState.ok);
        }
      } catch (e) {
        portFromIsolateToMain.send(WorkState.error);
        if (e == "bulkTransferIn error: LIBUSB_ERROR_TIMEOUT") {
        } else {
          await QuickUsb.exit();
          portFromIsolateToMain.send(WorkState.disconnected);
          timer.cancel();
          connectionState = false;
        }
      }
    });

    //слушать изолят из main
    portFromMainToIsolate.listen((dynamic message) {
      if (message is WorkState) {
        //Удаление изолята
        if (message == WorkState.disconnected) {
          connectionState = false;
          QuickUsb.exit();
          timer.cancel();
          portFromIsolateToMain.send(message);
        }
      }
    });
  } else {
    portFromIsolateToMain.send(WorkState.disconnected);
  }
}
