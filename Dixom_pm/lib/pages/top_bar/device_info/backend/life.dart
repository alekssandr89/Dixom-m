import 'package:dixom/driver/device_rx_tx/command_msg.dart';
import 'package:dixom/main.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

class DeviceLife {
  void setDefault() {
    appRef.read(deviceLifePrdr.notifier).updateModel(const DeviceLifeModel());
  }

  void receiver(CommandMsg msg) {
    /*   
     * Информацию о жизни устройства и даты выпуска прошивки
     * Сообщение содержится в msg.split список List<String>  
     * Параметры отмеченные * константы
     *
     * 0 3 2 04.06.2023 0 15 49 34862
     * | | |     |      | |  |    +---- 7 Общее время работы платформы в секундах
     * | | |     |      | |  + -------- 6 Количество записей в EEPROM память
     * | | |     |      | +------------ 5 Количество записей в Flash память
     * | | |     |      +-------------- 4 0 стабильные прошивки 1 эксперементальные
     * | | |     +--------------------- 3 Дата выпуска прошивки
     * | | +--------------------------- 2 * Подпараметр жизень платформы
     * | +----------------------------- 1 * Параметр life
     * +------------------------------- 0 * Пакет sytem
     */
    appRef.read(deviceLifePrdr.notifier).update(
          dateFirmware: msg.split[DevLife.dateFirmware.index],
          experimentalUse: msg.split[DevLife.experimentalUse.index],
          numFlashWrite: msg.split[DevLife.numFlashWrite.index],
          numEepromWrite: msg.split[DevLife.numEepromWrite.index],
          workingHours: msg.split[DevLife.workingHours.index],
        );
  }
}

enum DevLife {
  packed, // 0 System
  parametr, // 1 Information
  podParametr, // 2 Platform
  dateFirmware, // 3 Firmware release date
  experimentalUse, // 4 Using experimental firmware
  numFlashWrite, // 5 Number of records per flash card
  numEepromWrite, // 6 Number of records per eeprom memory
  workingHours; // 7 Total platform time
}

final deviceLifePrdr = StateNotifierProvider<DeviceLifeNotifier, DeviceLifeModel>((ref) => DeviceLifeNotifier());

@immutable
class DeviceLifeModel {
  final String dateFirmware; //Дата выпуска прошивки
  final String experimentalUse; //Исползование эксперементальных прошивок
  final String numFlashWrite; //Количество записи в влешь память
  final String numEepromWrite; //Количество записей в EEPROM память
  final String workingHours; //Общее время работы платформы

  const DeviceLifeModel({
    this.dateFirmware = "-",
    this.experimentalUse = "-",
    this.numFlashWrite = "-",
    this.numEepromWrite = "-",
    this.workingHours = "-",
  });

  DeviceLifeModel copyWith({
    String? dateFirmware,
    String? experimentalUse,
    String? numFlashWrite,
    String? numEepromWrite,
    String? workingHours,
  }) {
    return DeviceLifeModel(
      dateFirmware: dateFirmware ?? this.dateFirmware,
      experimentalUse: experimentalUse ?? this.experimentalUse,
      numFlashWrite: numFlashWrite ?? this.numFlashWrite,
      numEepromWrite: numEepromWrite ?? this.numEepromWrite,
      workingHours: workingHours ?? this.workingHours,
    );
  }
}

class DeviceLifeNotifier extends StateNotifier<DeviceLifeModel> {
  DeviceLifeNotifier() : super(const DeviceLifeModel());

  void update({
    required String dateFirmware,
    required String experimentalUse,
    required String numFlashWrite,
    required String numEepromWrite,
    required String workingHours,
  }) {
    state = state.copyWith(
      dateFirmware: dateFirmware,
      experimentalUse: experimentalUse,
      numFlashWrite: numFlashWrite,
      numEepromWrite: numEepromWrite,
      workingHours: workingHours,
    );
  }

  void updateModel(DeviceLifeModel model) {
    state = state.copyWith(
      dateFirmware: model.dateFirmware,
      experimentalUse: model.experimentalUse,
      numFlashWrite: model.numFlashWrite,
      numEepromWrite: model.numEepromWrite,
      workingHours: model.workingHours,
    );
  }
}
