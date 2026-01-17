import 'package:dixom/driver/device_rx_tx/exchange_param.dart';
import 'package:dixom/pages/audio/backend/source.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

import 'package:dixom/driver/device_rx_tx/command_msg.dart';
import 'package:dixom/main.dart';
import 'package:dixom/driver/device_rx_tx/transmiter.dart';

import 'sound.dart';

const int volumeMasterDef = 20;
const int volumeMax = 60;
const int volumeMin = 0;

class AudioEffects {
  void setDefault() {
    appRef.read(audioEffectsPdr.notifier).setDefault();
  }

  void receiver(CommandMsg msg) {
    /*   
     * Информацию о устройстве регуляторах громкости
     * Сообщение содержится в msg.split список List<String>  
     * Параметры отмеченные * константы
     */
    if (msg.parametr == SoundPacked.loudness.index) {
      /* 4 4 0 14 50 14000 50
     * | | | |  |    |   +-- 6 Уровень подъема высоких
     * | | | |  |    +------ 5 Частота подъема высоких
     * | | | |  +----------- 4 Уровень подъема высоких
     * | | | +-------------- 3 Частота подъема низких
     * | | +---------------- 2 Источник звука
     * | +------------------ 1 * Параметр Loudness
     * +-------------------- 0 * Пакет sound
     * 
     * отправка 4 4=0 50 50 15 14000
     */
      appRef.read(audioEffectsPdr.notifier).rxLoudness(msg);
    }
  }
}

final audioEffectsPdr = StateNotifierProvider<AudioEffectsNotifier, AudioEffectsModel>((ref) {
  return AudioEffectsNotifier();
});

class Loundess {
  int freqL;
  int lvlL;
  int freqH;
  int lvlH;
  Loundess(this.freqL, this.lvlL, this.freqH, this.lvlH);
}

enum LoundessControl {
  packed,
  parram,
  source,
  freqLP,
  lvlLP,
  freqHP,
  lvlHP;
}

enum LoudnessParram {
  freqLP,
  lvlLP,
  freqHP,
  lvlHP;
}

class AudioEffectsModel {
  List<Loundess> loudness = [
    Loundess(15, 50, 14000, 50),
    Loundess(15, 50, 14000, 50),
    Loundess(15, 50, 14000, 50),
    Loundess(15, 50, 14000, 50),
    Loundess(15, 50, 14000, 50),
    Loundess(15, 50, 14000, 50),
  ];

  AudioEffectsModel({
    required this.loudness,
  });

  AudioEffectsModel copyWith({
    List<Loundess>? loudness,
  }) {
    return AudioEffectsModel(
      loudness: loudness ?? this.loudness,
    );
  }
}

class AudioEffectsNotifier extends StateNotifier<AudioEffectsModel> {
  AudioEffectsNotifier()
      : super(
          AudioEffectsModel(
            loudness: [
              Loundess(15, 50, 14000, 50),
              Loundess(15, 50, 14000, 50),
              Loundess(15, 50, 14000, 50),
              Loundess(15, 50, 14000, 50),
              Loundess(15, 50, 14000, 50),
              Loundess(15, 50, 14000, 50),
            ],
          ),
        );

  void loudness(DspSource source, Loundess newLoudness, LoudnessParram parram, {String freq = ""}) {
    if (freq.isNotEmpty) {}
    if (newLoudness.freqH < 7000) newLoudness.freqH = 7000;
    if (newLoudness.freqH > 20000) newLoudness.freqH = 20000;
    if (newLoudness.freqL > 60) newLoudness.freqH = 60;
    if (newLoudness.freqL < 10) newLoudness.freqH = 10;

    List<Loundess> loudness = state.loudness;
    loudness[source.index] = newLoudness;
    state = state.copyWith(loudness: loudness);

    if (parram == LoudnessParram.freqHP) {
      transmitter(
          "${RootPacked.sound.index} ${SoundPacked.loudness.index}=${source.index} ${parram.index} ${newLoudness.freqH}");
    } else if (parram == LoudnessParram.lvlHP) {
      transmitter(
          "${RootPacked.sound.index} ${SoundPacked.loudness.index}=${source.index} ${parram.index} ${newLoudness.lvlH}");
    } else if (parram == LoudnessParram.freqLP) {
      transmitter(
          "${RootPacked.sound.index} ${SoundPacked.loudness.index}=${source.index} ${parram.index} ${newLoudness.freqL}");
    } else if (parram == LoudnessParram.lvlLP) {
      transmitter(
          "${RootPacked.sound.index} ${SoundPacked.loudness.index}=${source.index} ${parram.index} ${newLoudness.lvlL}");
    }
  }

  void setDefault() {
    List<Loundess> loudness = [
      Loundess(15, 50, 14000, 50),
      Loundess(15, 50, 14000, 50),
      Loundess(15, 50, 14000, 50),
      Loundess(15, 50, 14000, 50),
      Loundess(15, 50, 14000, 50),
      Loundess(15, 50, 14000, 50),
    ];
    state = state.copyWith(loudness: loudness);
  }

  void rxLoudness(CommandMsg msg) {
    Loundess newLoudness = Loundess(
      int.parse(msg.split[LoundessControl.freqLP.index]),
      int.parse(msg.split[LoundessControl.lvlLP.index]),
      int.parse(msg.split[LoundessControl.freqHP.index]),
      int.parse(msg.split[LoundessControl.lvlHP.index]),
    );
    List<Loundess> loudness = state.loudness;
    loudness[msg.podParmetr] = newLoudness;
    state = state.copyWith(loudness: loudness);
  }
}
