import 'package:dixom/driver/device_rx_tx/transmiter.dart';
import 'package:dixom/pages/audio/backend/sound_effect.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:dixom/main.dart';

import '../../../driver/device_rx_tx/command_msg.dart';
import 'audio_out.dart';
import 'filter.dart';
import 'sound_settings.dart';
import 'source.dart';

//List<int> levlDetectorData = List<int>.filled(40, 0);
Uint8List levlDetectorData = Uint8List(50);

class Sound {
  AudioOut audioOut = AudioOut();
  SoundSource soundSource = SoundSource();
  SoundSettings soundSettings = SoundSettings();
  SoundFilter soundFilter = SoundFilter();
  AudioEffects audioEffects = AudioEffects();

  void setDefault() {
    audioOut.setDefault();
    soundSource.setDefault();
    soundSettings.setDefault();
    soundFilter.setDefault();
    audioEffects.setDefault();
  }

  void receiver(CommandMsg msg) {
    if (msg.parametr == SoundPacked.message.index) {
      if (msg.parametr == 0) {
        for (int i = 0; i < msg.buffer.length; i++) {
          levlDetectorData[i] = msg.buffer[i];
        }

        appRef.read(levelDetectorPrdr.notifier).getLevelDetector();
        //  appRef.read(levelDetectorPrdr.notifier).getLevelDetector(msg.buffer[7].toInt(), msg.buffer[7].toInt());
      }
    } else if (msg.parametr == SoundPacked.notification.index) {
    } else if (msg.parametr == SoundPacked.equalizer.index) {
      soundFilter.receiverEq(msg);
    } else if (msg.parametr == SoundPacked.crossover.index) {
      soundFilter.receiverCrossover(msg);
    } else if (msg.parametr == SoundPacked.groupChannel.index) {
      soundFilter.receiverGroup(msg);
    } else if (msg.parametr == SoundPacked.gain.index) {
      soundFilter.receiverGain(msg);
    } else if (msg.parametr == SoundPacked.baypassEq.index) {
      soundFilter.receiverBypass(msg);
    } else if (msg.parametr == SoundPacked.sourceEq.index) {
      soundSettings.receiver(msg);
    } else if (msg.parametr == SoundPacked.settings.index) {
      soundSettings.receiver(msg);
    } else if (msg.parametr == SoundPacked.volume.index) {
      audioOut.receiverVolume(msg);
    } else if (msg.parametr == SoundPacked.delay.index) {
      audioOut.receiverDelay(msg);
    } else if (msg.parametr == SoundPacked.phase.index) {
      audioOut.receiverPhase(msg);
    } else if (msg.parametr == SoundPacked.source.index) {
      soundSource.receiver(msg);
    } else if (msg.parametr == SoundPacked.loudness.index) {
      audioEffects.receiver(msg);
    }
  }
}

final levelDetectorPrdr = StateNotifierProvider<LevelDetectorModelNotifier, LevelDetectorModel>((ref) {
  Uint8List buffer = Uint8List(0);
  return LevelDetectorModelNotifier(buffer);
});

class LevelDetectorModel {
  int ldUpdate;
  LevelDetectorModel({
    this.ldUpdate = 0,
  });

  LevelDetectorModel copyWith({
    int? ldUpdate,
  }) {
    return LevelDetectorModel(
      ldUpdate: ldUpdate ?? this.ldUpdate,
    );
  }
}

class LevelDetectorModelNotifier extends StateNotifier<LevelDetectorModel> {
  LevelDetectorModelNotifier(Uint8List buffer) : super(LevelDetectorModel());

  void getLevelDetector() {
    int ldUpdate = state.ldUpdate;
    ldUpdate++;
    state = state.copyWith(ldUpdate: ldUpdate);
  }
}

///Основные пакеты команд аудиоплатформы
enum SoundPacked {
  message("Message"), //0 сообщения пакета звук
  notification("Notification"), // 1 Уведомления
  settings("Sound settings"), //2 Настройки
  volume("Volume"), // 3 ругуляторы громкости
  loudness("Loudness"), // 4  тонкомпенсация
  volControl("Volume Control"), // 5 Режим регулятора грмомкости
  source("Source"), // 6 ругуляторы громкости
  sourceControl("Source control"), // 7 ругуляторы громкости
  sourceEq("Source equalizer"), // 8 ругуляторы громкости
  preset("Preset"), // 9 Пресеты звука
  presetSC("Preset switch control"), // 10 Сетап переключеня с кноки
  presetSave("Save preset"), //11 * сохранение пресета (только команда)
  presetlabne("Preset lable"), // 12 Название пресета
  gain("Gain"), // 13 усиление
  groupChannel("Group channel"), // 14 группировка каналов
  copyChannel("Copy channel"), // 15 Копирование каналов
  groupName("Group name"), // 16 Название группы
  chanelName("Channel name"), // 17 Название канала
  equalizer("Equalizer"), // 18 ругуляторы громкости
  baypassEq("Bypass eq"), // 19 Обходной путь эквалайзера
  crossover("Crossover"), // 20 настроки кроссовера
  delay("Delay"), // 21 звуковые задержки
  delayGrpupName("Delay group name"), // 22 Название группы
  delayGroup("Delay group"), // 23 Группа задержек
  phase("Phase"), // 24 Управление фазой
  noise("InvNoiseert"), // 25 Управление фазой
  sinGenerator("Sinus generator"), // 26 генератор синуса
  phatStereo("Phat stereo"), // 27 эфект расширенного стерео
  wideStereo("Wide stereo"), // 28 эфект шумного звука
  supperBass("Supper bass"), // 29 эффект суппер баса
  sdsd("Volume"); // 3 ругуляторы громкости

  final String decoding;
  const SoundPacked(this.decoding);
}

enum CurrentFilterType {
  equalizer,
  lowPass,
  hiPass;
}

final deviceSoundPrdr = StateNotifierProvider<SoundModelNotifier, SoundModel>((ref) => SoundModelNotifier());

@immutable
class SoundModel {
  final int currentChannel;
  final CurrentFilterType currentFilterType;
  final List<bool> isGrouped;

  const SoundModel({
    this.isGrouped = const [true, true, true, true, true, true, true, true],
    this.currentChannel = 0,
    this.currentFilterType = CurrentFilterType.equalizer,
  });

  SoundModel copyWith({
    int? currentChannel,
    CurrentFilterType? currentFilterType,
    List<bool>? isGrouped,
  }) {
    return SoundModel(
      currentChannel: currentChannel ?? this.currentChannel,
      currentFilterType: currentFilterType ?? this.currentFilterType,
      isGrouped: isGrouped ?? this.isGrouped,
    );
  }
}

enum GroupExch {
  packed, //Пакет Sound
  parametr, //Парметр кроссовер
  group, //Номер группы
  state, //Состояние
}

class SoundModelNotifier extends StateNotifier<SoundModel> {
  SoundModelNotifier() : super(const SoundModel());

  void update(int currentChannel, CurrentFilterType currentFilterType) {
    state = state.copyWith(
      currentChannel: currentChannel,
      currentFilterType: currentFilterType,
      isGrouped: state.isGrouped,
    );
  }

  void getGrouped(CommandMsg msg) {
    List<bool> grouped = List<bool>.filled(8, false);
    for (int i = 0; i < state.isGrouped.length; i++) {
      grouped[i] = state.isGrouped[i];
    }
    if (msg.split[GroupExch.state.index] == "0") {
      grouped[msg.podParmetr] = false;
    } else {
      grouped[msg.podParmetr] = true;
    }
    state = state.copyWith(
      currentChannel: state.currentChannel,
      currentFilterType: state.currentFilterType,
      isGrouped: grouped,
    );
  }

  void updateGroup(int channel) {
    int group = channel ~/ 2;
    if (group > 7) {
      return;
    }
    List<bool> grouped = List<bool>.filled(8, false);
    for (int i = 0; i < state.isGrouped.length; i++) {
      grouped[i] = state.isGrouped[i];
    }
    String groupedState = "0";
    if (state.isGrouped[group]) {
      grouped[group] = false;
    } else {
      grouped[group] = true;
      groupedState = "1";
    }

    int groupedChannel = appRef.read(soundEqProvider[channel]).groupChannel;

    appRef.read(soundEqProvider[channel].notifier).copyToGroupedProvider();

    if (channel < groupedChannel) {
      groupedChannel = state.currentChannel;
    }

    state = state.copyWith(
      currentChannel: groupedChannel,
      currentFilterType: state.currentFilterType,
      isGrouped: grouped,
    );

    transmitter("4 14=$group $groupedState $channel");
  }

  void updateUi(int currentChannel, {CurrentFilterType currentFilterType = CurrentFilterType.equalizer}) {
    //Обновляем интерфейс
    state = state.copyWith(
      currentChannel: currentChannel,
      currentFilterType: currentFilterType,
      isGrouped: state.isGrouped,
    );
  }

  void setCrssover(CurrentFilterType currentFilterType) {
    state = state.copyWith(
      currentChannel: state.currentChannel,
      currentFilterType: currentFilterType,
      isGrouped: state.isGrouped,
    );
  }
}
