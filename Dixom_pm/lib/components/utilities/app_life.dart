import 'dart:io';
import 'package:dixom/driver/android/service_channel.dart';
import 'package:dixom/driver/windows/usb_hid.dart';
import 'package:flutter/material.dart';

class AppLifecycleObserver with WidgetsBindingObserver {
  static int appState = 3;
  @override
  void didChangeAppLifecycleState(AppLifecycleState state) {
    switch (state) {
      case AppLifecycleState.resumed:
        // Приложение было возобновлено
        if (Platform.isAndroid) {
          AndroidService.exchange("app_state resumed");
        }
        appState = 0;
        break;
      case AppLifecycleState.inactive:
        appState = 1;
        // Приложение стало неактивным
        if (Platform.isAndroid) {
          AndroidService.exchange("app_state inactive");
        }
        break;
      case AppLifecycleState.paused:
        appState = 2;
        // Приложение было приостановлено
        if (Platform.isAndroid) {
          AndroidService.exchange("app_state paused");
        }
        break;
      case AppLifecycleState.detached:
        appState = 3;
        // Приложение было закрыто
        if (Platform.isWindows) {
          disconnectUsb();
        }

        break;
      case AppLifecycleState.hidden:       
        break;
    }
  }
}
