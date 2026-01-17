import 'package:flutter/material.dart';

const String methodSendString = "str";
const String methodSendByte = "byte";
const String methodArgument = "data";
const String methodServiceControl = "service";
const String methodCheck = "check";

const String methodAppState = "app_state";
const String methodGetSettings = "get_settings";
const String methodSetSettings = "set_settings";
const String methodGetConnection = "get_connection";

const String methodSetConnection = "set_connection";
const String methodSetInterface = "set_interface";
const String methodGetBluetoothDeviceList = "get_bt_device_list";
const String methodSetBluetoothDevice = "set_bt_device";
const String methodGetBluetoothDevice = "get_bt_device";
const String methodHudTestMsg = "hud_test_msg";

const String methodInterfaceInfo = "interface_info";

const int intrfaceStateUsbHid = 1;
const int intrfaceStateBluetoothSpp = 2;
const int intrfaceStateUsbCdc = 3;
const int stateHud = 4;
const int stateNotification = 5;
const int stateAutomaticConnection = 6;

enum ServiceSettings {
  settingsVersion, //версия настроек
  connectionUsbHid,
  connectionUsbCdc,
  connectionBluetoothSpp,
  hud,
  notification,
}

///Цвета элементов управления
class ColorDisplay {
  //Верхний градиент кнопки
  static const Color backgroundOn = Color.fromARGB(68, 0, 0, 0);
  //Верхний градиент кнопки
  static const Color lable = Color.fromARGB(185, 255, 255, 255);
  //Верхний градиент кнопки
  static const Color backgroundOff = Color.fromARGB(255, 230, 233, 24);
  //Верхний градиент кнопки
  static const Color displayFrame = Color.fromARGB(169, 255, 255, 255);
  //Верхний градиент кнопки
  static const Color edge = Color.fromARGB(255, 201, 227, 244);
  //Цвет точек полос на графике
  static const Color bandPoint = Colors.deepPurple;
  //Цвет линии графика настраиваемого
  static const Color grapMainLine = Colors.black;
  //Цвет линии графика каналов
  static const List<Color> graphChannelLine = <Color>[
    Colors.lightGreenAccent,
    Colors.teal,
    Colors.orangeAccent,
    Colors.greenAccent,
    Colors.tealAccent,
    Colors.deepOrangeAccent,
    Colors.pinkAccent,
    Colors.greenAccent,
    Colors.indigoAccent,
    Colors.white,
    Colors.grey,
    Colors.teal,
    Color.fromARGB(255, 155, 133, 236),
    Colors.red,
  ];
}

///Цвета элементов управления
class ColorControls {
  //Верхний градиент кнопки
  static const Color buttonUp = Color.fromARGB(255, 63, 70, 94); //
  //Ниний градиент
  static const Color buttonDown = Color.fromARGB(255, 45, 53, 73);
  //Градиент блеска кнопки вверхний
  static const Color buttonShineUp = Color.fromARGB(255, 116, 118, 124);
  //Градиент блеска кнопки нижний
  static const Color buttonShineDown = Color.fromARGB(255, 63, 70, 94);
  //Цвет кнопки
  static const Color buttonBlue = Color.fromARGB(255, 0, 225, 255);
  //Цвет кнопки
  static const Color buttonGreen = Color.fromARGB(255, 0, 255, 34);
  //Цвет кнопки
  static const Color buttonOrange = Color.fromARGB(255, 255, 187, 0);
  //Цвет кнопки
  static const Color buttonRed = Color.fromARGB(255, 255, 238, 0);
  //Градиент блеска кнопки нижний
  static const Color buttonOff = Color.fromARGB(255, 63, 70, 94);
  //Тень
  static const Color shadow = Color.fromARGB(255, 30, 36, 49);
  //Вырез
  static const Color cutout = Color.fromARGB(255, 40, 48, 65);
  //Градиент впадины верхний
  static const Color depressionUp = Color.fromARGB(255, 30, 36, 49);
  //Градиент впадины нижний
  static const Color depressionDown = Color.fromARGB(255, 116, 118, 124);
  //Не активная насечка
  static const Color notchNoActive = Colors.grey;
  //Активная насечка
  static const Color notchActive = Color.fromARGB(255, 0, 225, 255);
  //Ручка
  static const Color handle = Colors.grey;
  //Ручка
  static const Color lable = Color.fromARGB(255, 179, 178, 178);
  //Цвет центра ручки слайдера в активном состоянии
  static const Color activSlider = Color.fromARGB(255, 0, 225, 255);
  //Цвет центра ручки слайдера в не активном состоянии
  static const Color unActivSlider = Color.fromARGB(255, 0, 0, 0);
}

///Цвета приложения
class ColorPanel {
  //Цвет заднео фона
  static const Color background = Color.fromARGB(255, 63, 70, 94);
}

///Цвета приложения
class AppColor {
  //Цвет макетов
  static const Color secondary = Color.fromRGBO(47, 45, 62, 1);
  //Цвет заднео фона
  static const Color background = Color(0xFF212332);
  //Цвет активных элементов
  static Color active = const Color.fromARGB(255, 212, 213, 219);
  //Цвет активных элементов
  static Color notActive = const Color.fromARGB(255, 45, 46, 49);
  //Цвет активных элементов
  static Color btnDefault = const Color.fromARGB(255, 45, 46, 49);
  //Цвет активных элементов
  static Color btnClick = const Color.fromARGB(255, 45, 46, 49);
  //Цвет Ошибка
  static const Color error = Color.fromARGB(255, 18, 161, 218);
  //Цвет всё хорошо
  static const Color ok = Color.fromARGB(255, 75, 255, 4);
  //Цвет фона всплывающих окон
  static const Color allertBacground = Color.fromARGB(245, 38, 44, 58);
}

/// Отступы
class AppIndents {
  ///Наружный отступ
  static const double pading = 16.0;
  //Внутренний отступ
  static const double margin = 16.0;
}

class AppTextSize {
  final main = 17;
  final description = 16.0;
  final header1 = 20.0;
  final header2 = 17.0;
}

//const defaultPaddin = 16.0;
const defaultPasddin = 16.0;
const defaultTextSize = 16.0;
const defRadius = Radius.circular(10);

const String appVersion = "1.0.0";
