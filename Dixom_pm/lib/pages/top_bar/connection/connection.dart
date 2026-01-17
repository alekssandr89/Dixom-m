import 'dart:io';
import 'package:dixom/dataBase/app_constants.dart';
import 'package:flutter/material.dart';
import 'view/connection_status.dart';
import 'view/settings_android.dart';
import 'view/settings_windows.dart';

class ConnectionView extends StatelessWidget {
  const ConnectionView({super.key});

  @override
  Widget build(BuildContext context) {
    return SingleChildScrollView(
      child: Container(
        padding: const EdgeInsets.all(AppIndents.pading),
        decoration: const BoxDecoration(
          color: AppColor.secondary,
        ),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            //Состояние подключения
            const ConnectionStatus(),
            //Настройка подключения Windows
            if (Platform.isWindows) const ConnectionSettingsWindows(),
            //Получение разрешений андройд
            if (Platform.isAndroid) const GetAndroidPermissions(),
            //настройка подключения Android
            if (Platform.isAndroid) const ConnectionSettingsAndroid(),
          ],
        ),
      ),
    );
  }
}
