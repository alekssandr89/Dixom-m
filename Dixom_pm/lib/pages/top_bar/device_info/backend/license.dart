import 'package:dixom/driver/device_rx_tx/command_msg.dart';
import 'package:dixom/main.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

class DeviceLicense {
  void setDefault() {
    appRef.read(deviceLicensePrdr.notifier).updateModel(const DeviceLicenseModel());
  }

  void receiver(CommandMsg msg) {
    /*   
     * Информацию о серийном номере и ключе активации
     * Сообщение содержится в msg.split список List<String>  
     * Параметры отмеченные * константы
     *
     * 0 3 1 XXXX-XXXX-XXXX-XXXX XXXXXXXXXXXX
     * | | |          |               +-------- 4 0 Ключ активации 12 сиволов
     * | | |          + ----------------------- 3 0 Серийный номер    
     * | | +----------------------------------- 2 1 * Подпараметр лицензия
     * | +------------------------------------- 1 3 * Параметр license
     * +--------------------------------------- 0 0 * Пакет sytem
     */
    appRef.read(deviceLicensePrdr.notifier).update(
          serialNumber: msg.split[DevLicense.serialNumber.index],
          licenseKey: msg.split[DevLicense.licenseKey.index],
        );
  }
}

enum DevLicense {
  packed, // 0 System
  parametr, // 1 Information
  podParametr, // 2 Platform
  serialNumber, // 3 Device type, digital designation
  licenseKey; // 4 Device model, digital designation
}

final deviceLicensePrdr =
    StateNotifierProvider<DeviceLicenseNotifier, DeviceLicenseModel>((ref) => DeviceLicenseNotifier());

@immutable
class DeviceLicenseModel {
  final String serialNumber; //Серийный номер устройства
  final String licenseKey; //Ключь активации

  const DeviceLicenseModel({
    this.serialNumber = "-",
    this.licenseKey = "-",
  });

  DeviceLicenseModel copyWith({
    String? serialNumber,
    String? licenseKey,
  }) {
    return DeviceLicenseModel(
      serialNumber: serialNumber ?? this.serialNumber,
      licenseKey: licenseKey ?? this.licenseKey,
    );
  }
}

class DeviceLicenseNotifier extends StateNotifier<DeviceLicenseModel> {
  DeviceLicenseNotifier() : super(const DeviceLicenseModel());

  void update({
    required String serialNumber,
    required String licenseKey,
  }) {
    state = state.copyWith(
      serialNumber: serialNumber,
      licenseKey: licenseKey,
    );
  }

  void updateModel(DeviceLicenseModel model) {
    state = state.copyWith(
      serialNumber: model.serialNumber,
      licenseKey: model.licenseKey,
    );
  }
}
