import 'package:dixom/driver/device_rx_tx/command_msg.dart';
import 'package:dixom/main.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

class DacInformation {
  void setDefault() {
    appRef.read(dacInfoPrdr.notifier).updateModel(const DacInfoModel());
  }

  void receiver(CommandMsg msg) {
    /*   
     * Информацию о подключенном ЦАП
     * Сообщение содержится в msg.split список List<String>  
     * Параметры отмеченные * константы
     *
     * 0 3 4 8 1 8
     * | | | | | +---- 5 Количество каналов у одной микросхемы ЦАП
     * | | | | +-------4 Количество микросхем ЦАП
     * | | | +-------- 3 Модель подключенного ЦАП
     * | | +---------- 2 * Подпараметр информация о ЦАП
     * | +------------ 1 * Параметр dacInfromation
     * +-------------- 0 * Пакет sytem
     */
    appRef.read(dacInfoPrdr.notifier).update(
          model: msg.split[DacInfo.model.index],
          numChips: msg.split[DacInfo.numChips.index],
          numChannel: msg.split[DacInfo.numChannel.index],
        );
  }
}

enum DacInfo {
  packed, // 0 System
  parametr, // 1 Information
  podParametr, // 2 Platform
  model, // 3 Connected DAC model
  numChips, // 4  Number of DAC Chips
  numChannel; // 5 Number of audio outputs
}

final dacInfoPrdr = StateNotifierProvider<DacInfoNotifier, DacInfoModel>((ref) => DacInfoNotifier());

@immutable
class DacInfoModel {
  final String model; //Модель ЦАП
  final String numChips; //Количество микросхем на плате цап
  final String numChannel; //Количество аудио каналов

  const DacInfoModel({
    this.model = "-",
    this.numChips = "0",
    this.numChannel = "2",
  });

  DacInfoModel copyWith({
    String? model,
    String? numChips,
    String? numChannel,
  }) {
    return DacInfoModel(
      model: model ?? this.model,
      numChips: numChips ?? this.numChips,
      numChannel: numChannel ?? this.numChannel,
    );
  }
}

class DacInfoNotifier extends StateNotifier<DacInfoModel> {
  DacInfoNotifier() : super(const DacInfoModel());

  void update({
    required String model,
    required String numChips,
    required String numChannel,
  }) {
    state = state.copyWith(
      model: model,
      numChips: numChips,
      numChannel: numChannel,
    );
  }

  void updateModel(DacInfoModel model) {
    state = state.copyWith(
      model: model.model,
      numChips: model.numChips,
      numChannel: model.numChannel,
    );
  }
}
