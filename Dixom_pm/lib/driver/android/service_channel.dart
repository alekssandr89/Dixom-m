import 'package:dixom/dataBase/app_constants.dart';
import 'package:dixom/driver/android/android_service_model.dart';
import 'package:dixom/driver/device_rx_tx/command_msg.dart';
import 'package:dixom/pages/top_bar/connection/backend/device_connection.dart';
import 'package:dixom/dataBase/device_constant.dart';
import 'package:dixom/main.dart';
import 'package:flutter/services.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import '../device_rx_tx/reciever.dart';

Future<dynamic> _handleDataAndroidToDart(MethodCall call) async {
  if (call.method == methodArgument) {
    //Получение сообщений с данными платформы из Android Service
    List<int> intlist = (call.arguments as List).cast<int>();
    Uint8List newTransferIn = Uint8List.fromList(intlist);
    CommandMsg message = byteToCommandMsg(intlist.length, newTransferIn, interface: ExchangeInterface.usbHid);
    distributor.receiver(message);
  } else if (call.method == methodServiceControl) {
    //Получение сообщений с данными сервиса из Android Service
    String rxData = call.arguments as String;
    serviceMessage(rxData);
  } else {
    //Ошибка
    throw MissingPluginException();
  }
}

final serviceConnectionInfo = FutureProvider.autoDispose.family((ref, ExchangeInterface intrface) {
  final connectionInfo = AndroidServiceConnectionRepository();
  return connectionInfo.getSettingsData(intrface);
});

final serviceConfiguration = FutureProvider.autoDispose((ref) {
  final settingsRepository = ref.watch(androidServiceSettingsRepositoryProvider);
  return settingsRepository.getSettingsData();
});

Future<WorkState> seviceAvilableCheck() async {
  try {
    int? state = await const MethodChannel('com.dixom.exchange.dart.android').invokeMethod<int>("check");
    if (state is int) {
      return WorkState.values[state];
    } else {
      return WorkState.error;
    }
  } catch (e) {
    return WorkState.error;
  }
}

Future<String?> sendStringToAndroid(String method, {String param = "no data"}) async {
  try {
    final argument = {methodArgument: param};
    return await const MethodChannel('com.dixom.exchange.dart.android').invokeMethod<String>(method, argument);
  } catch (e) {
    return e.toString();
  }
}

Future<List<String>> getBluetootDeviceList() async {
  try {
    final dynamic result =
        await const MethodChannel('com.dixom.exchange.dart.android').invokeMethod(methodGetBluetoothDeviceList);
    if (result is List) {
      return List<String>.from(result.map((item) => item.toString())); // Приведение к List<String>
    } else {
      throw Exception('Expected a list');
    }
  } on PlatformException catch (e) {
    throw 'Failed to get list: ${e.message}';
  }
}
/*
void setBluetootDevice(String argument) {
  const MethodChannel('com.dixom.exchange.dart.android').invokeMethod(methodSetBluetoothDevice, argument);
}*/

Future<String> setBluetootDevice(String argument) async {
  try {
    final dynamic result =
        await const MethodChannel('com.dixom.exchange.dart.android').invokeMethod(methodGetBluetoothDevice);
    if (result is String) {
      return result; // Приведение к List<String>
    } else {
      throw Exception('Expected a list');
    }
  } on PlatformException catch (e) {
    throw 'Failed to get list: ${e.message}';
  }
}

Future<String> getBluetootDevice() async {
  try {
    final dynamic result =
        await const MethodChannel('com.dixom.exchange.dart.android').invokeMethod(methodGetBluetoothDevice);
    if (result is String) {
      return result; // Приведение к List<String>
    } else {
      throw Exception('Expected a list');
    }
  } on PlatformException catch (e) {
    throw 'Failed to get list: ${e.message}';
  }
}

class AndroidService {
  /// Отправка данных из FlutterApp в Service
  static const _channelDartToAndroid = MethodChannel('com.dixom.exchange.dart.android');

  /// Получение данных из Service в FlutterApp
  static const _channelAndroidToDart = MethodChannel('com.dixom.exchange.android.dart');

  static void start() {
    _channelAndroidToDart.setMethodCallHandler(_handleDataAndroidToDart);
  }

  static Future<WorkState?> transmitter(dynamic message) async {
    try {
      int response = WorkState.error.index;
      if (message is String) {
        final argument = {methodArgument: message};
        //Получаем ответ о доставке сообщения в int формате
        response = await _channelDartToAndroid.invokeMethod<int>(methodSendString, argument) ?? WorkState.error.index;
      } else if (message is Uint8List) {
        final argument = {methodArgument: message};
        //Получаем ответ о доставке сообщения в int формате
        response = await _channelDartToAndroid.invokeMethod<int>(methodSendByte, argument) ?? WorkState.error.index;
      } else if (message is CommandMsg) {
        final argument = {methodArgument: message.buffer};
        //Получаем ответ о доставке сообщения в int формате
        response = await _channelDartToAndroid.invokeMethod<int>(methodSendByte, argument) ?? WorkState.error.index;
      }
      //Конвертируем ответ в WorkState
      return WorkState.values[response];
    } on PlatformException {
      return WorkState.error;
    } on MissingPluginException {
      return WorkState.error;
    } // Return 0, success delivery
  }

  static Future<dynamic> exchange(String message) async {
    try {
      final argument = {methodArgument: message};
      dynamic response = await _channelDartToAndroid.invokeMethod<int>(methodServiceControl, argument);
      return response;
    } on PlatformException {
      return WorkState.error;
    } on MissingPluginException {
      return WorkState.error;
    } // Return 0, success delivery
  }
}

void serviceMessage(String message) {
  try {
    late List<String> splitted = message.split(" ");

    if (splitted[0] == methodGetConnection) {
      if (splitted[1] == ExchangeInterface.usbHid.index.toString()) {
        setInterfaceConnection(usbHidProvider, splitted);
      } else if (splitted[1] == ExchangeInterface.usbCdc.index.toString()) {
        setInterfaceConnection(usbCdcProvider, splitted);
      } else if (splitted[1] == ExchangeInterface.bluetoothSpp.index.toString()) {
        setInterfaceConnection(bluetoothSppProvider, splitted);
      }
    } else if (splitted[0] == methodInterfaceInfo) {
      if (splitted[1] == ExchangeInterface.usbHid.index.toString()) {
        setInterfaceInfo(usbHidProvider, splitted);
      } else if (splitted[1] == ExchangeInterface.usbCdc.index.toString()) {
        setInterfaceInfo(usbCdcProvider, splitted);
      } else if (splitted[1] == ExchangeInterface.bluetoothSpp.index.toString()) {
        setInterfaceInfo(bluetoothSppProvider, splitted);
      }
    } else if (splitted[0] == methodGetSettings) {
      String enable = WorkState.enable.index.toString();
      appRef.read(connectionState.notifier).copyWith(
            autoConnectionHid: splitted[1] == enable ? true : false,
            autoConnectionCdc: splitted[2] == enable ? true : false,
            autoConnectionBluetooth: splitted[3] == enable ? true : false,
          );
      appRef.read(androidServiceSettings.notifier).copyWith(
            hud: splitted[4] == enable ? true : false,
            notification: splitted[5] == enable ? true : false,
          );
    }
  } catch (e) {
    // prinDebig(e.toString());
  }
}

void setInterfaceInfo(
    StateNotifierProvider<InterfaceConnectionNotifier, InterfaceConnectionModel> provider, List<String> splitted) {
  appRef.read(provider.notifier).setExchangeState(
        succesIn: int.parse(splitted[5]),
        succesOut: int.parse(splitted[6]),
        errorIn: int.parse(splitted[7]),
        errorOut: int.parse(splitted[8]),
      );
}

void setInterfaceConnection(
    StateNotifierProvider<InterfaceConnectionNotifier, InterfaceConnectionModel> provider, List<String> splitted) {
  appRef.read(provider.notifier).setStatus(
        workState[int.parse(splitted[2])],
        statusIn: workState[int.parse(splitted[3])],
        statusOut: workState[int.parse(splitted[4])],
      );
}
