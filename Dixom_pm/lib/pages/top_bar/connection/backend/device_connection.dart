import 'dart:io';
import 'package:dixom/dataBase/app_constants.dart';
import 'package:dixom/driver/android/service_channel.dart';
import 'package:dixom/driver/device_rx_tx/command_msg.dart';
import 'package:dixom/dataBase/device_constant.dart';
import 'package:dixom/driver/device_rx_tx/reciever.dart';
import 'package:dixom/driver/device_rx_tx/transmiter.dart';
import 'package:dixom/main.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

import '../../update/backend/update_const.dart';
import '../../update/backend/update_model.dart';

final bluetoothSppProvider = StateNotifierProvider<InterfaceConnectionNotifier, InterfaceConnectionModel>(
    (ref) => InterfaceConnectionNotifier());

final usbCdcProvider = StateNotifierProvider<InterfaceConnectionNotifier, InterfaceConnectionModel>(
    (ref) => InterfaceConnectionNotifier());

final usbHidProvider = StateNotifierProvider<InterfaceConnectionNotifier, InterfaceConnectionModel>(
    (ref) => InterfaceConnectionNotifier());

final connectionState = StateNotifierProvider<ConnectionNotifier, ConnectionModel>((ref) => ConnectionNotifier());

void setConnectionState(CommandMsg msg) {
  appRef.read(connectionState.notifier).update(msg);
}

///Выставление интерфейса отправки данных
void setTransmitInterface(ExchangeInterface interface) {
  appRef.read(connectionState.notifier).copyWith(rxinterface: interface);

  if (Platform.isAndroid) {
    AndroidService.exchange("set_interface ${interface.index}");
  }
}

void loading() {
  if (Platform.isWindows) {
    appRef.read(usbCdcProvider.notifier).setStatus(WorkState.notSupported);

    appRef.read(bluetoothSppProvider.notifier).setStatus(WorkState.notSupported);
  }
}

///Проверка на наличие хоть какого то подключения
void generalConnectionCheck() {
  final usbHid = appRef.read(usbHidProvider.select((value) => value.status));
  final usbcdc = appRef.read(usbCdcProvider.select((value) => value.status));
  final bluetooth = appRef.read(bluetoothSppProvider.select((value) => value.status));

  if (usbHid == WorkState.connected || usbcdc == WorkState.connected || bluetooth == WorkState.connected) {
    ExchangeInterface curentInterface = ExchangeInterface.none;

    if (usbHid == WorkState.connected) {
      curentInterface = ExchangeInterface.usbHid;
    } else if (usbcdc == WorkState.connected) {
      curentInterface = ExchangeInterface.usbCdc;
    } else if (bluetooth == WorkState.connected) {
      curentInterface = ExchangeInterface.bluetoothSpp;
    }
    if (Platform.isAndroid) {
      serviceMessage("$methodSetInterface $curentInterface");
    }

    if (appRef.read(connectionState.select((value) => value.status)) != WorkState.connected) {
      //Если есть хоть какое то подключение подключение
      //1 Выставляем все настройки по умолчанию
      distributor.setDefault();
      //2 Заправшиваем информацию о платформе и прошивках
      transmitter("0 3");
      //3 Заправшиваем все изменёные параметры
      transmitter("1 4");
    }

    appRef.read(connectionState.notifier).copyWith(
          generalStatus: WorkState.connected,
          txInterface: curentInterface,
          rxinterface: curentInterface,
          statusUsbHid: usbHid,
          statusUsbCdc: usbcdc,
          statusBluetooth: bluetooth,
        );
  } else {
    appRef.read(connectionState.notifier).deviceDisconnected();
  }
}

class ConnectionModel {
  ///Общее состояние аудиоплатформы (подключено или нет)
  final WorkState status;

  final WorkState statusUsbHid;

  final WorkState statusUsbCdc;

  final WorkState statusBluetooth;

  ///Интерфейс отправки данных из платформы
  final ExchangeInterface flutterRx;

  ///Интерфейс получения данных в платформу
  final ExchangeInterface flutterTx;

  ///Состояние подключения Usb Hid внутри платформы
  final bool connectionHid;

  ///Состояние подключения Usb Cdc внутри платформы
  final bool connectionCdc;

  ///Состояние подключения Bluetooth SPP внутри платформы
  final bool connectionBluetooth;

  ///Принудительная отправка данных из плафтормы в usbHid
  final bool manualTxHid;

  ///Принудительная отправка данных из плафтормы в usbCdc
  final bool manualTxCdc;

  ///Принудительная отправка данных из плафтормы в bluetoothSPP
  final bool manualTxBluetooth;

  ///Автоподключение к usbHid
  final bool autoConnectionHid;

  ///Автоподключение к usbCdc
  final bool autoConnectionCdc;

  ///Автоподключение к usbBluetooth Spp
  final bool autoConnectionBluetooth;

  final String selectedBluetoothDevice;

  const ConnectionModel({
    required this.status,
    required this.statusUsbHid,
    required this.statusUsbCdc,
    required this.statusBluetooth,
    required this.flutterRx,
    required this.flutterTx,
    required this.connectionHid,
    required this.connectionCdc,
    required this.connectionBluetooth,
    required this.manualTxHid,
    required this.manualTxCdc,
    required this.manualTxBluetooth,
    required this.autoConnectionHid,
    required this.autoConnectionCdc,
    required this.autoConnectionBluetooth,
    required this.selectedBluetoothDevice,
  });
}

///Параметры отправки данных из устройства
enum TxControl {
  packed, // 0 System
  parametr, // 1 Information
  interface, // 2 interface 0 Usb-Hid, 1 Usb-Cdc, 2 Bluetooth-Spp
  connectionHid, // 3 connection status 9 connected, 20 disconnected
  manualTxHid, // 4 manual output of data via the interface. 0 disable, 1 allow
  connectionCdc, // 5 connection status 9 connected, 20 disconnected
  manualTxCdc, // 6 manual output of data via the interface. 0 disable, 1 allow
  connectionBluetooth, // 7 connection status 9 connected, 20 disconnected
  manualTxBluetooth; // 8 manual output of data via the interface. 0 disable, 1 allow
}

class ConnectionNotifier extends StateNotifier<ConnectionModel> {
  ConnectionNotifier()
      : super(const ConnectionModel(
          status: WorkState.disconnected,
          statusUsbHid: WorkState.disconnected,
          statusUsbCdc: WorkState.disconnected,
          statusBluetooth: WorkState.disconnected,
          flutterRx: ExchangeInterface.none,
          flutterTx: ExchangeInterface.none,
          connectionHid: false,
          connectionCdc: false,
          connectionBluetooth: false,
          manualTxHid: false,
          manualTxCdc: false,
          manualTxBluetooth: false,
          autoConnectionHid: true,
          autoConnectionCdc: true,
          autoConnectionBluetooth: true,
          selectedBluetoothDevice: "Dixom Hi-Res Audio",
        ));

  ConnectionModel copyWith({
    WorkState? generalStatus,
    WorkState? statusUsbHid,
    WorkState? statusUsbCdc,
    WorkState? statusBluetooth,
    ExchangeInterface? txInterface,
    ExchangeInterface? rxinterface,
    bool? connectionHid,
    bool? connectionCdc,
    bool? connectionBluetooth,
    bool? manualTxHid,
    bool? manualTxCdc,
    bool? manualTxBluetooth,
    bool? autoConnectionHid,
    bool? autoConnectionCdc,
    bool? autoConnectionBluetooth,
    String? selectedBluetoothDevice,
  }) {
    return state = ConnectionModel(
      status: generalStatus ?? state.status,
      statusUsbHid: statusUsbHid ?? state.statusUsbHid,
      statusUsbCdc: statusUsbCdc ?? state.statusUsbCdc,
      statusBluetooth: statusBluetooth ?? state.statusBluetooth,
      flutterRx: txInterface ?? state.flutterRx,
      flutterTx: rxinterface ?? state.flutterTx,
      connectionHid: connectionHid ?? state.connectionHid,
      connectionCdc: connectionCdc ?? state.connectionCdc,
      connectionBluetooth: connectionBluetooth ?? state.connectionBluetooth,
      manualTxHid: manualTxHid ?? state.manualTxHid,
      manualTxCdc: manualTxCdc ?? state.manualTxCdc,
      manualTxBluetooth: manualTxBluetooth ?? state.manualTxBluetooth,
      autoConnectionHid: autoConnectionHid ?? state.autoConnectionHid,
      autoConnectionCdc: autoConnectionCdc ?? state.autoConnectionCdc,
      autoConnectionBluetooth: autoConnectionBluetooth ?? state.autoConnectionBluetooth,
      selectedBluetoothDevice: selectedBluetoothDevice ?? state.selectedBluetoothDevice,
    );
  }

  void updateGetBluetoothDevice() async {
    String deviceName = "";
    state = copyWith(selectedBluetoothDevice: deviceName);
  }

  void deviceDisconnected() {
    state = copyWith(
      generalStatus: WorkState.disconnected,
      statusUsbHid: WorkState.disconnected,
      statusUsbCdc: WorkState.disconnected,
      statusBluetooth: WorkState.disconnected,
      txInterface: ExchangeInterface.none,
      rxinterface: ExchangeInterface.none,
      connectionHid: false,
      connectionCdc: false,
      connectionBluetooth: false,
      manualTxHid: false,
      manualTxCdc: false,
      manualTxBluetooth: false,
    );

    distributor.setDefault();
    appRef.read(frimwareUpdateViewPrdr.notifier).update(btnAction: UpdateBtnAction.getJson);
  }

  void update(CommandMsg msg) {
    int index = int.parse(msg.split[TxControl.interface.index]);
    ExchangeInterface txInterface = ExchangeInterface.usbHid;
    if (index == ExchangeInterface.usbCdc.index) {
      txInterface = ExchangeInterface.usbCdc;
    } else if (index == ExchangeInterface.bluetoothSpp.index) {
      txInterface = ExchangeInterface.bluetoothSpp;
    }

    copyWith(
      txInterface: txInterface,
      connectionHid: msg.split[TxControl.connectionHid.index] == WorkState.connected.index.toString() ? true : false,
      connectionCdc: msg.split[TxControl.connectionCdc.index] == WorkState.connected.index.toString() ? true : false,
      connectionBluetooth:
          msg.split[TxControl.connectionBluetooth.index] == WorkState.connected.index.toString() ? true : false,
      manualTxHid: msg.split[TxControl.manualTxHid.index] == "1" ? true : false,
      manualTxCdc: msg.split[TxControl.manualTxCdc.index] == "1" ? true : false,
      manualTxBluetooth: msg.split[TxControl.manualTxBluetooth.index] == "1" ? true : false,
    );
  }
}

class InterfaceConnectionInfo {
  const InterfaceConnectionInfo({required this.provider});

  final StateNotifierProvider<InterfaceConnectionNotifier, InterfaceConnectionModel> provider;

  void setConnectionInfo(InterfaceState state, {WorkState workState = WorkState.notInitialized}) {
    switch (state) {
      case (InterfaceState.succesIn):
        appRef.read(provider.notifier).incrementSuccesIn();
        break;
      case (InterfaceState.succesOut):
        appRef.read(provider.notifier).incrementSuccesOut();
        break;
      case (InterfaceState.status):
        appRef.read(provider.notifier).setStatus(workState);
        break;
      case (InterfaceState.statusIn):
        appRef.read(provider.notifier).setStatusIn(workState);
        break;
      case (InterfaceState.statusOut):
        appRef.read(provider.notifier).setStatusOut(workState);
        break;
      case (InterfaceState.errorIn):
        appRef.read(provider.notifier).incrementErrorIn();
        break;
      case (InterfaceState.errorOut):
        appRef.read(provider.notifier).incrementErrorOut();
        break;
    }
  }
}

class InterfaceConnectionModel {
  ///Состояние подключения к интерфесу
  final WorkState status;

  ///Состояние подключения к интерфесу для получения данных
  final WorkState statusIn;

  ///Состояние подключения к интерфесу для отправки данных
  final WorkState statusOut;

  ///Колиество успешно полученных сообщений
  final int succesIn;

  ///Колиество успешно отправленых сообщений
  final int succesOut;

  ///Колиество ошибок при получении сообщений
  final int errorIn;

  ///Колиество ошибок при отправке сообщений
  final int errorOut;

  const InterfaceConnectionModel({
    required this.status,
    required this.statusIn,
    required this.statusOut,
    required this.succesIn,
    required this.succesOut,
    required this.errorIn,
    required this.errorOut,
  });
}

class InterfaceConnectionNotifier extends StateNotifier<InterfaceConnectionModel> {
  InterfaceConnectionNotifier()
      : super(const InterfaceConnectionModel(
          status: WorkState.notInitialized,
          statusIn: WorkState.notInitialized,
          statusOut: WorkState.notInitialized,
          succesIn: 0,
          succesOut: 0,
          errorIn: 0,
          errorOut: 0,
        ));

  void incrementSuccesIn() {
    _copyWith(succesIn: state.succesIn + 1);
  }

  void incrementSuccesOut() {
    _copyWith(succesOut: state.succesOut + 1);
  }

  void incrementErrorIn() {
    _copyWith(errorIn: state.errorIn + 1);
  }

  void incrementErrorOut() {
    _copyWith(errorOut: state.errorOut + 1);
  }

  void setStatus(
    WorkState generalState, {
    WorkState? statusIn,
    WorkState? statusOut,
  }) {
    _copyWith(status: generalState);
    generalConnectionCheck();
  }

  void setStatusIn(WorkState inState) {
    _copyWith(statusIn: inState);
  }

  void setStatusOut(WorkState outState) {
    _copyWith(statusOut: outState);
  }

  void setExchangeState({
    int? succesIn,
    int? succesOut,
    int? errorIn,
    int? errorOut,
  }) {
    _copyWith(
      succesIn: succesIn,
      succesOut: succesOut,
      errorIn: errorIn,
      errorOut: errorOut,
    );
  }

  InterfaceConnectionModel _copyWith({
    WorkState? status,
    WorkState? statusIn,
    WorkState? statusOut,
    int? succesIn,
    int? succesOut,
    int? errorIn,
    int? errorOut,
  }) {
    return state = InterfaceConnectionModel(
      status: status ?? state.status,
      statusIn: statusIn ?? state.statusIn,
      statusOut: statusOut ?? state.statusOut,
      succesIn: succesIn ?? state.succesIn,
      succesOut: succesOut ?? state.succesOut,
      errorIn: errorIn ?? state.errorIn,
      errorOut: errorOut ?? state.errorOut,
    );
  }
}
