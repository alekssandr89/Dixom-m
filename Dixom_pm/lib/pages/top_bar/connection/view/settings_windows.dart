import 'package:dixom/pages/top_bar/connection/backend/device_connection.dart';
import 'package:dixom/driver/windows/usb_hid.dart';
import 'package:dixom/main.dart';
import 'package:dixom/components/widgets/connection.dart';
import 'package:flutter/material.dart';
import 'package:shared_preferences/shared_preferences.dart';

class ConnectionSettingsWindows extends StatefulWidget {
  const ConnectionSettingsWindows({
    super.key,
  });

  @override
  State<ConnectionSettingsWindows> createState() => _ConnectionSettingsWindowsState();
}

class _ConnectionSettingsWindowsState extends State<ConnectionSettingsWindows> {
  @override
  void initState() {
    super.initState();
  }

  @override
  void dispose() {
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        const Text(
          "Настройки обмена и подключения",
          style: TextStyle(fontSize: 20, color: Colors.white70),
        ),
        const Divider(),
        SettingsChecked(
          description:
              "Программа Dixom PM будет автоматический подключаться к аудиоплатформе по протоколу USB-Hid при обнаружении подключения аудиоплатформы к вашему компьютеру",
          parrametr: "Автоматическое подключение через USB-Hid",
          state: appRef.read(connectionState.select((value) => value.autoConnectionHid)),
          onTap: () async {
            final SharedPreferences prefs = await SharedPreferences.getInstance();
            if (appRef.read(connectionState.select((value) => value.autoConnectionHid))) {
              appRef.read(connectionState.notifier).copyWith(autoConnectionHid: false);
              usbHid.stopAutoConenct();
              await prefs.setBool('windowsAutoConnectUsbHid', false);
            } else {
              appRef.read(connectionState.notifier).copyWith(autoConnectionHid: true);
              await prefs.setBool('windowsAutoConnectUsbHid', true);
              usbHid.startAutoConnect();
            }
            setState(() {});
          },
        ),
        const SizedBox(
          height: 26,
        ),
      ],
    );
  }
}
