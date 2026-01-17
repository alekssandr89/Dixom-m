import 'dart:async';
import 'dart:io';
import 'dart:typed_data';
import 'package:dixom/pages/top_bar/connection/backend/device_connection.dart';
import 'package:dixom/driver/device_rx_tx/reciever.dart';
import 'package:dixom/driver/windows/usb_hid.dart';
import 'package:dixom/main.dart';
import 'package:dixom/pages/top_bar/console/console.dart';
import '../../dataBase/device_constant.dart';
import '../android/service_channel.dart';
import 'command_msg.dart';

void transmitter(dynamic data, {ExchangeInterface interface = ExchangeInterface.usbHid}) {
  if (data is String) {
    CommandMsg cmdMsg = stringToCommandMsg(data, interface: interface);
    dataConsole.addMessage(cmdMsg);
  } else if (data is CommandMsg) {
    dataConsole.addMessage(data);
  }

  if (Platform.isAndroid) {
    AndroidService.transmitter(data);
  } else if (Platform.isWindows) {
    sendData(data);
  }
}

Future<List<T>> deviceResponse<T extends Object>({
  //Команда запроса
  required String request,
  //Возвращаемые данные
  T? standard,
  //Функция
  required Function function,
  //Таймаут для генерации исключения о не пришедших данных
  int timeOutExeption = 1000,
  //Таймаут для поторного запроса
  int timeoutForRetryRequest = 500,
  //Количество попыток поторного запроса
  int numberOfAttempts = 0,
}) async {
  List<T> response = standard != null ? [standard] : [];

  if (appRef.read(connectionState.select((value) => value.status)) != WorkState.connected) {
    response;
  }

  final completer = Completer();

  final subscription = distributor.rxController.stream.listen(
    (msg) {
      if (function(msg, response) == WorkState.complete) {
        completer.complete();
      }
    },
  );

  //Отправляем запрос на получение журнала с подробной информацией "1 9 chipNum regAddr"
  transmitter(request);

  //Запускаем таймер, если данные не придут запрос будет завершен с ошибкой
  final timer = Timer(const Duration(seconds: 2), () {
    subscription.cancel();
    //throw Exception('Данные не пришли');
  });

  // Дожидаемся завершения потока
  await completer.future;
  timer.cancel();
  subscription.cancel();
  return response;
}

Future<WorkState?> transmitByte(
    {required int timeout,
    required int lenght,
    required Uint8List data,
    ExchangeInterface interface = ExchangeInterface.usbHid}) async {
  if (Platform.isAndroid) {
    return AndroidService.transmitter(data);
  } else if (Platform.isWindows) {
    return sendData(data);
  } else {
    return WorkState.error;
  }
}

void androidServiceExchange(String data) {
  if (Platform.isAndroid) {
    AndroidService.exchange(data);
  }
}
