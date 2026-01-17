import 'package:dixom/driver/device_rx_tx/command_msg.dart';
import 'package:dixom/main.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

class DeviceVersion {
  void setDefault() {
    appRef.read(deviceVersionPrdr.notifier).updateModel(const DeviceVersionModel());
  }

  void deviceVeceiver(CommandMsg msg) {
    /*   
     * Информацию о устройстве и прошивке
     * Сообщение содержится в msg.split список List<String>  
     * Параметры отмеченные * константы
     *
     * 0 3 0 1 2 2 4.026 5.10 2.2 Platform Dixom-m AsIWant 
     * | | | | | |   |    |    |     |        |     +- 11 0 Название прошивки
     * | | | | | |   |    |    |     |        + ------ 10 0 Модель устройства
     * | | | | | |   |    |    |     +---------------- 9 0 Тип устройства
     * | | | | | |   |    |    +---------------------- 8 0 Ревизия печатной платы
     * | | | | | |   |    +--------------------------- 7 0 Версия загрузчика
     * | | | | | |   +-------------------------------- 6 0 Версия прошивки
     * | | | | | +------------------------------------ 5 0 Версия прошивки в циврофой форме
     * | | | | +-------------------------------------- 4 0 Модель в цифровойм формате
     * | | | +---------------------------------------- 3 0 Тип устрйоства цифровойм формате
     * | | +------------------------------------------ 2 0 * Подпараметр информация устройстве
     * | +-------------------------------------------- 1 3 * Параметр information
     * +---------------------------------------------- 0 0 * Пакет sytem
     */
    appRef.read(deviceVersionPrdr.notifier).update(
          dType: int.parse(msg.split[DevVersion.dType.index]),
          dModel: int.parse(msg.split[DevVersion.dModel.index]),
          dFrimwareName: int.parse(msg.split[DevVersion.dFrimwareName.index]),
          verFrimware: msg.split[DevVersion.verFrimware.index],
          verBootloader: msg.split[DevVersion.verBootloader.index],
          revPCB: msg.split[DevVersion.revPCB.index],
          type: msg.split[DevVersion.type.index],
          model: msg.split[DevVersion.model.index],
          firmwareName: msg.split[DevVersion.firmwareName.index],
        );
  }

  void bootloaderNewVersion(CommandMsg msg) {
    appRef.read(deviceVersionPrdr.notifier).newBootloaderFrimware(
          msg.split[NewBootloader.versionBootloader.index],
        );
  }
}

enum DevVersion {
  packed, // 0 System
  parametr, // 1 Information
  podParametr, // 2 Platform
  dType, // 3  Device type, digital designation
  dModel, //4 Device model, digital designation
  dFrimwareName, //5 Frimware Name, digital designation
  verFrimware, //6 Platform firmware version in string form
  verBootloader, //7 Bootloader firmware version in string form
  revPCB, //8 PCB version in string form
  type, //9 Device type in string form
  model, //10 Device model in string form
  firmwareName; //11 Firmware name in string form
}

enum NewBootloader {
  packed, // 0 System
  parametr, // 1 Information
  podParametr, // 2 new wersion
  versionBootloader, // 3  bootloader version
}

final deviceVersionPrdr =
    StateNotifierProvider<DeviceVersionNotifier, DeviceVersionModel>((ref) => DeviceVersionNotifier());

@immutable
class DeviceVersionModel {
  final int dType; //Тип устройства
  final int dModel; //Модель
  final int dFrimwareName; //Название прошивки
  final String verFrimware; //Версия прошивки
  final String verBootloader; //Версия загрузчика
  final String revPCB; //Ревизия печатной платы
  final String type; //Тип устройства
  final String model; //Модель
  final String firmwareName; //Название прошивки
  final String newBootlodaeFrimware; //Модель

  const DeviceVersionModel({
    this.dType = 0,
    this.dModel = 0,
    this.dFrimwareName = 0,
    this.verFrimware = "-",
    this.verBootloader = "-",
    this.revPCB = "-",
    this.type = "-",
    this.model = "-",
    this.firmwareName = "-",
    this.newBootlodaeFrimware = "",
  });

  DeviceVersionModel copyWith({
    int? dType,
    int? dModel,
    int? dFrimwareName,
    String? verFrimware,
    String? verBootloader,
    String? revPCB,
    String? type,
    String? model,
    String? firmwareName,
    String? newBootlodaeFrimware,
  }) {
    return DeviceVersionModel(
      dType: dType ?? this.dType,
      dModel: dModel ?? this.dModel,
      dFrimwareName: dFrimwareName ?? this.dFrimwareName,
      verFrimware: verFrimware ?? this.verFrimware,
      verBootloader: verBootloader ?? this.verBootloader,
      revPCB: revPCB ?? this.revPCB,
      type: type ?? this.type,
      model: model ?? this.model,
      firmwareName: firmwareName ?? this.firmwareName,
      newBootlodaeFrimware: newBootlodaeFrimware ?? this.newBootlodaeFrimware,
    );
  }
}

class DeviceVersionNotifier extends StateNotifier<DeviceVersionModel> {
  DeviceVersionNotifier() : super(const DeviceVersionModel());

  void update({
    required int dType,
    required int dModel,
    required int dFrimwareName,
    required String verFrimware,
    required String verBootloader,
    required String revPCB,
    required String type,
    required String model,
    required String firmwareName,
  }) {
    state = state.copyWith(
      dType: dType,
      dModel: dModel,
      dFrimwareName: dFrimwareName,
      verFrimware: verFrimware,
      verBootloader: verBootloader,
      revPCB: revPCB,
      type: type,
      model: model,
      firmwareName: firmwareName,
    );
  }

  void newBootloaderFrimware(String newBootlodaeFrimware) {
    state = state.copyWith(
      newBootlodaeFrimware: newBootlodaeFrimware,
    );
  }

  void updateModel(DeviceVersionModel model) {
    state = state.copyWith(
      dType: model.dType,
      dModel: model.dModel,
      dFrimwareName: model.dFrimwareName,
      verFrimware: model.verFrimware,
      verBootloader: model.verBootloader,
      revPCB: model.revPCB,
      type: model.type,
      model: model.model,
      firmwareName: model.firmwareName,
    );
  }
}
