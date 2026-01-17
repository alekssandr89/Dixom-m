// ignore_for_file: public_member_api_docs, sort_constructors_first
import 'package:dixom/driver/android/service_channel.dart';
import 'package:dixom/pages/top_bar/connection/backend/device_connection.dart';
import 'package:dixom/dataBase/device_constant.dart';
import 'package:dixom/main.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

final androidServiceSettingsRepositoryProvider = Provider((ref) {
  return AndroidServiceSettingsRepository();
});
final androidServiceSettings = StateNotifierProvider<AndroidServiceSettingsNotifier, AndroidServiceSettingsModel>(
    (ref) {
      var androidServiceSettingsNotifier = AndroidServiceSettingsNotifier();
      return androidServiceSettingsNotifier;
    });

class AndroidServiceSettingsModel {
/*autonnection settings in ConnectionModel class*/
  ///Состояние HUD монитора
  final bool autoConUsbHid;

  ///Состояние HUD монитора
  final bool autoConUsbCdc;

  ///Состояние HUD монитора
  final bool autoConBluetooth;

  ///Состояние HUD монитора
  final bool hud;

  ///Состояние уведомлений
  final bool notification;

  const AndroidServiceSettingsModel({
    this.autoConUsbHid = false,
    this.autoConUsbCdc = false,
    this.autoConBluetooth = false,
    required this.hud,
    required this.notification,
  });

  AndroidServiceSettingsModel copyWith({
    bool? autoConUsbHid,
    bool? autoConUsbCdc,
    bool? autoConBluetooth,
    bool? hud,
    bool? notification,
  }) {
    return AndroidServiceSettingsModel(
      autoConUsbHid: autoConUsbHid ?? this.autoConUsbHid,
      autoConUsbCdc: autoConUsbCdc ?? this.autoConUsbCdc,
      autoConBluetooth: autoConBluetooth ?? this.autoConBluetooth,
      hud: hud ?? this.hud,
      notification: notification ?? this.notification,
    );
  }
}

class AndroidServiceSettingsNotifier extends StateNotifier<AndroidServiceSettingsModel> {
  AndroidServiceSettingsNotifier()
      : super(const AndroidServiceSettingsModel(
          hud: false,
          notification: false,
        ));

  AndroidServiceSettingsModel copyWith({
    bool? hud,
    bool? notification,
  }) {
    return state = AndroidServiceSettingsModel(
      hud: hud ?? state.hud,
      notification: notification ?? state.notification,
    );
  }
}

//Запрос о состоянии параметров в сервисе
class AndroidServiceSettingsRepository {
  Future<AndroidServiceSettingsModel> getSettingsData() async {
    final String? value = await sendStringToAndroid("get_settings");

    List<String> splt = value!.split(" ");

    AndroidServiceSettingsModel settings = AndroidServiceSettingsModel(
        autoConUsbHid: splt[0] == WorkState.enable.index.toString() ? true : false,
        autoConUsbCdc: splt[1] == WorkState.enable.index.toString() ? true : false,
        autoConBluetooth: splt[2] == WorkState.enable.index.toString() ? true : false,
        hud: splt[3] == WorkState.enable.index.toString() ? true : false,
        notification: splt[4] == WorkState.enable.index.toString() ? true : false);

    appRef.read(connectionState.notifier).copyWith(
          autoConnectionHid: settings.autoConUsbHid,
          autoConnectionCdc: settings.autoConUsbCdc,
          autoConnectionBluetooth: settings.autoConBluetooth,
        );

    appRef.read(androidServiceSettings.notifier).copyWith(
          hud: settings.hud,
          notification: settings.notification,
        );

    return settings;
  }
}

class AndroidServiceConnectionRepository {
  Future<InterfaceConnectionModel> getSettingsData(ExchangeInterface interface) async {
    final String? value = await sendStringToAndroid("get_connection", param: interface.index.toString());

    if (value == null) {
      throw Exception("Failed to get connection data"); // создание исключения
    }

    List<String> splt = value.split(" ");

    InterfaceConnectionModel settings = InterfaceConnectionModel(
        status: WorkState.values[int.parse(splt[InterfaceState.status.index])],
        statusIn: WorkState.values[int.parse(splt[InterfaceState.statusIn.index])],
        statusOut: WorkState.values[int.parse(splt[InterfaceState.statusOut.index])],
        succesIn: int.parse(splt[InterfaceState.succesIn.index]),
        succesOut: int.parse(splt[InterfaceState.succesOut.index]),
        errorIn: int.parse(splt[InterfaceState.errorIn.index]),
        errorOut: int.parse(splt[InterfaceState.errorOut.index]));

    if (interface == ExchangeInterface.usbHid) {
      appRef.read(usbHidProvider.notifier).setStatus(
            settings.status,
            statusIn: settings.statusIn,
            statusOut: settings.statusOut,
          );
    } else if (interface == ExchangeInterface.usbCdc) {
      appRef.read(usbCdcProvider.notifier).setStatus(
            settings.status,
            statusIn: settings.statusIn,
            statusOut: settings.statusOut,
          );
    } else if (interface == ExchangeInterface.bluetoothSpp) {
      appRef.read(bluetoothSppProvider.notifier).setStatus(
            settings.status,
            statusIn: settings.statusIn,
            statusOut: settings.statusOut,
          );
    }
    generalConnectionCheck();
    return settings;
  }
}
