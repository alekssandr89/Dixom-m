import 'package:dixom/driver/device_rx_tx/command_msg.dart';
import 'package:dixom/driver/device_rx_tx/transmiter.dart';
import 'package:dixom/main.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

class SoundSource {
  void setDefault() {
    appRef.read(deviceSourcePrdr.notifier).update(DspSource.usb);
  }

  void receiver(CommandMsg msg) {
    /*   
     * Информацию о источника звука
     * Сообщение содержится в msg.split список List<String>  
     * Параметры отмеченные * константы
     *
     * 4 7 0  
     * | | +---- 2 Подпараметр источник звука
     * | +------ 1 * Параметр volume
     * +-------- 0 * Пакет sound
     * 
     * отправка 4 7=0
     */

    appRef.read(deviceSourcePrdr.notifier).update(DspSource.values[msg.podParmetr]);
  }
}

final deviceSourcePrdr = StateNotifierProvider<DspSourceNotifier, DspSourceModel>((ref) => DspSourceNotifier());

enum VolControl { packed, parametr, source }

enum DspSource {
  usb("USB"),
  bluetooth("Bluetooth"),
  aux("AUX"),
  fmRadio("FM Radio"),
  spdif("SPDIF"),
  microphone("MIC");

  final String decoding;
  const DspSource(this.decoding);
}

@immutable
class DspSourceModel {
  final DspSource source;

  const DspSourceModel({
    this.source = DspSource.usb,
  });

  DspSourceModel copyWith({
    DspSource? source,
  }) {
    return DspSourceModel(
      source: source ?? this.source,
    );
  }
}

class DspSourceNotifier extends StateNotifier<DspSourceModel> {
  DspSourceNotifier() : super(const DspSourceModel());

  void update(DspSource source) {
    state = state.copyWith(
      source: source,
    );
  }

  void updateUi(DspSource source) {
    //Обновляем интерфейс
    state = state.copyWith(
      source: source,
    );
    //Отправляем новые данные в аудиоплатформу
    transmitter("4 6=${source.index}");
  }
}
