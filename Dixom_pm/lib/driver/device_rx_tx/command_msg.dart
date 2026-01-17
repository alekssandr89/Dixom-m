import 'dart:convert';
import 'dart:typed_data';
import 'package:dixom/dataBase/device_constant.dart';

///Объект для обмена сообщениями между девайсом и программой
class CommandMsg {
  late final Uint8List buffer;
  String dataUtf8 = "";
  String time = "";
  bool showInConsole = true;
  ExchangeInterface interface = ExchangeInterface.none;
  ExchangeDirection direction = ExchangeDirection.accepted;
  int packed = 0;
  int parametr = 0;
  int podParmetr = 0;
  late List<String> split;

  CommandMsg(this.interface, int size) {
    buffer = Uint8List(size);
    DateTime now = DateTime.now();
    time =
        '${now.minute.toString().padLeft(2, '0')}:${now.second.toString().padLeft(2, '0')}.${now.microsecond.toString().padLeft(3, '0')}';
  }
}

///Получения CommandMsg
CommandMsg stringToCommandMsg(String message, {ExchangeInterface interface = ExchangeInterface.none}) {
  Uint8List bytes = Uint8List.fromList(utf8.encode(message));
  CommandMsg msg = CommandMsg(interface, bytes.length);

  for (int i = 0; i < bytes.length; i++) {
    msg.buffer[i] = bytes[i];
  }

  msg.direction = ExchangeDirection.send;
  msg.dataUtf8 = message;
  msg.split = msg.dataUtf8.split(" ");
  try {
    msg.packed = int.parse(msg.split[0]);
  } catch (e) {
    msg.packed = 9999;
  }
  try {
    msg.parametr = int.parse(msg.split[1]);
  } catch (e) {
    msg.parametr = 9999;
  }
  try {
    msg.podParmetr = int.parse(msg.split[2]);
  } catch (e) {
    msg.podParmetr = 9999;
  }
  return msg;
}

CommandMsg byteToCommandMsg(int size, Uint8List message, {ExchangeInterface interface = ExchangeInterface.none}) {
  //List<int> intlist = message.toList();

  CommandMsg msg = CommandMsg(interface, size);

  for (int i = 0; i < size; i++) {
    msg.buffer[i] = message[i];
  }

  try {
    msg.dataUtf8 = utf8.decode(msg.buffer, allowMalformed: true);
    msg.split = msg.dataUtf8.split(" ");

    try {
      msg.packed = int.parse(msg.split[0]);
    } catch (e) {
      msg.packed = 0;
    }

    try {
      msg.parametr = int.parse(msg.split[1]);
    } catch (e) {
      msg.parametr = 0;
    }

    try {
      msg.podParmetr = int.parse(msg.split[2]);
    } catch (e) {
      msg.podParmetr = 0;
    }
  } catch (e) {
    msg.packed = 0;
  }

  return msg;
}

//Направление передачи данных
enum ExchangeDirection {
  accepted("accepted"), //0 принятое сообщение
  send("send"); // 1 отправленное сообщение

  final String decoding;
  const ExchangeDirection(this.decoding);
}
