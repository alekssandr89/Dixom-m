import 'package:dixom/driver/device_rx_tx/command_msg.dart';
import 'package:dixom/main.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

class UsbSoundInformation {
  void setDefault() {
    appRef.read(usbSoundInfoPrdr.notifier).updateModel(const UsbSoundModel());
  }

  void receiver(CommandMsg msg) {
    /*   
     * Информацию о USB звуковой карте
     * Сообщение содержится в msg.split список List<String>  
     * Параметры отмеченные * константы
     *
     * 0 3 3 1.005 1.00 1 1
     * | | |   |     |  | + - 6 Режим работы звуковой карты
     * | | |   |     |  +---- 5 Модель usb звуковой карты
     * | | |   |     +--------4 Версия загрузчика USB звуковой карты
     * | | |   +------------- 3 Версия прошивки USB звуковой карты
     * | | +----------------- 2 * Подпараметр информация о usb звуковой карте
     * | +------------------- 1 * Параметр usbSoundInformation
     * +--------------------- 0 * Пакет sytem
     */
    appRef.read(usbSoundInfoPrdr.notifier).update(
          verFrimware: msg.split[UsbSoundInfo.verFrimware.index],
          verBootloader: msg.split[UsbSoundInfo.verBootloader.index],
          model: msg.split[UsbSoundInfo.model.index],
          inMode: msg.split[UsbSoundInfo.inMode.index],
        );
  }
}

enum UsbSoundInfo {
  packed, // 0 System
  parametr, // 1 Information
  podParametr, // 2 Platform
  verFrimware, // Firmware version in string form
  verBootloader, // Bootloader firmware version in string form
  model, // Sound card model, in digital form
  inMode; // Sound card operating mode
}

final usbSoundInfoPrdr = StateNotifierProvider<UsbSoundNotifier, UsbSoundModel>((ref) => UsbSoundNotifier());

@immutable
class UsbSoundModel {
  final String verFrimware; //Версия прошивки
  final String verBootloader; //Версия загрузчик
  final String model; //Модель звуковой карты
  final String inMode; //Режим работы звуковой карты

  const UsbSoundModel({
    this.verFrimware = "-",
    this.verBootloader = "-",
    this.model = "-",
    this.inMode = "-",
  });

  UsbSoundModel copyWith({
    String? verFrimware,
    String? verBootloader,
    String? model,
    String? inMode,
  }) {
    return UsbSoundModel(
      verFrimware: verFrimware ?? this.verFrimware,
      verBootloader: verBootloader ?? this.verBootloader,
      model: model ?? this.model,
      inMode: inMode ?? this.inMode,
    );
  }
}

class UsbSoundNotifier extends StateNotifier<UsbSoundModel> {
  UsbSoundNotifier() : super(const UsbSoundModel());

  void update({
    required String verFrimware,
    required String verBootloader,
    required String model,
    required String inMode,
  }) {
    state = state.copyWith(
      verFrimware: verFrimware,
      verBootloader: verBootloader,
      model: model,
      inMode: inMode,
    );
  }

  void updateModel(UsbSoundModel model) {
    state = state.copyWith(
      verFrimware: model.verFrimware,
      verBootloader: model.verBootloader,
      model: model.model,
      inMode: model.inMode,
    );
  }
}
