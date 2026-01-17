// DO NOT EDIT. This is code generated via package:easy_localization/generate.dart

// ignore_for_file: prefer_single_quotes

import 'dart:ui';

import 'package:easy_localization/easy_localization.dart' show AssetLoader;

class CodegenLoader extends AssetLoader {
  const CodegenLoader();

  @override
  Future<Map<String, dynamic>> load(String path, Locale locale) {
    return Future.value(mapLocales[locale.toString()]);
  }

  static const Map<String, String> en = {
    "consoleTime": "Time",
    "consoleConnection": "Connection",
    "consoleDirection": "Direction",
    "consolePackage": "Package",
    "consoleParam": "Parameter",
    "consoleData": "Data",
    "pageHome": "Home",
    "pageCan": "Can bus",
    "pageSettings": "Settings",
    "pageSound": "Sound",
    "pageConsole": "Console",
    "usbConnectionNone": "Connection lost",
    "usbConnectionYes": "Connection Ok",
    "appLanguage": "English"
  };
  static const Map<String, String> ru = {
    "consoleTime": "Время",
    "consoleConnection": "Интерфейс",
    "consoleDirection": "Направление",
    "consolePackage": "Пакет",
    "consoleParam": "Параметр",
    "consoleData": "Данные",
    "pageHome": "Дом",
    "pageCan": "Кан шина",
    "pageSettings": "Настройки",
    "pageSound": "Звук",
    "pageConsole": "Консоль",
    "usbConnectionNone": "Соединение разорвано",
    "usbConnectionYes": "Соединение установлено",
    "appLanguage": "Русский"
  };
  static const Map<String, Map<String, dynamic>> mapLocales = {
    "en": en,
    "ru": ru
  };
}
