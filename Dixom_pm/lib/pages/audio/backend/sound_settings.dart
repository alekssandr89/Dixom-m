import 'package:dixom/driver/device_rx_tx/command_msg.dart';
import 'package:dixom/driver/device_rx_tx/transmiter.dart';
import 'package:dixom/main.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

class SoundSettings {
  void setDefault() {
    appRef.read(soundSettingsPrdr.notifier).setDefault;
  }

  void receiver(CommandMsg msg) {
    /*   
     * Информацию о настройках звука
     * Сообщение содержится в msg.split список List<String>  
     * Параметры отмеченные * константы
     *
     * 4 2 0 1 
     * | | | +-- 3 Данные настройки
     * | | +---- 2 Подпараметр номер параметра настройки
     * | +------ 1 * Параметр volume
     * +-------- 0 * Пакет sound
     * 
     * отправка 4 3=0 3
     */
    appRef.read(soundSettingsPrdr.notifier).update(msg);
  }
}

final soundSettingsPrdr = StateNotifierProvider<SoundSettingsNotifier, SoundSettingsModel>((ref) {
  return SoundSettingsNotifier();
});

class SoundSettingsModel {
  List<int> settings;

  SoundSettingsModel({
    List<int> settings = soundSettingsDefault,
  }) : settings = List<int>.from(settings);

  SoundSettingsModel setVolume(int settingsNum, int settings) {
    this.settings[settingsNum] = settings;
    //TODO надо попробовать оптимизировать, происходит копирование всех регуляторов громкости
    return SoundSettingsModel(settings: this.settings);
  }
}

///Параметры отправки данных из устройства
enum SoundSettingsExch {
  packed,
  parametr,
  settingsNum,
  settignsData,
}

class SoundSettingsNotifier extends StateNotifier<SoundSettingsModel> {
  SoundSettingsNotifier() : super(SoundSettingsModel());
  void update(CommandMsg msg) {
    int settingsNum = int.parse(msg.split[SoundSettingsExch.settingsNum.index]);
    int settings = int.parse(msg.split[SoundSettingsExch.settignsData.index]);
    state = state.setVolume(settingsNum, settings);
  }

  void updateUi(int settingsNum, int settings) {
    //Обновляем интерфейс
    state = state.setVolume(settingsNum, settings);
    //Отправляем новые данные в аудиоплатформу
    transmitter("4 2=$settingsNum $settings");
  }

  void setLoudness() {
    //Обновляем интерфейс
    if (state.settings[SoundCfg.volumeControlMode.index] != 2) {
      state = state.setVolume(SoundCfg.volumeControlMode.index, 2);
    } else {
      state = state.setVolume(SoundCfg.volumeControlMode.index, 1);
    }
    //Отправляем новые данные в аудиоплатформу
    transmitter("4 2=${SoundCfg.volumeControlMode.index} ${state.settings[SoundCfg.volumeControlMode.index]}");
  }

  void setLevelDetector() {
    //Обновляем интерфейс
    if (state.settings[SoundCfg.levelDetector.index] == 0) {
      state = state.setVolume(SoundCfg.levelDetector.index, 1);
    } else {
      state = state.setVolume(SoundCfg.levelDetector.index, 0);
    }
    //Отправляем новые данные в аудиоплатформу
    transmitter("4 2=${SoundCfg.levelDetector.index} ${state.settings[SoundCfg.levelDetector.index]}");
  }

  void setDefault() {
    state = SoundSettingsModel();
  }
}

enum SoundCfg {
  eqStandardPresets,
  dspPresset,
  volumeControlMode,
  selfbot,
  dspInitialization,
  source,
  routingUsb,
  routingBluetooth,
  routingAux,
  routingRadio,
  routingSpdif,
  routingMic,
  presetRoutingUser1,
  presetRoutingUser2,
  presetRoutingUser3,
  presetRoutingFlat,
  presetRoutingBass,
  presetRoutingBassExtreme,
  presetRoutingBassAndHigh,
  presetRoutingClassic,
  presetRoutingDancing,
  presetRoutingSpeaker,
  presetRoutingLive,
  presetRoutingMedium,
  presetRoutingSoft,
  presetRoutingSoftBass,
  presetRoutingSoftHigh,
  loudnessUsb,
  loudnessBluetooth,
  loudnessRadio,
  loudnessSpdif,
  loudnessMic,
  levelDetector,
  volumLevelPoewrUp,
  tempHysteresis,
  tempOS,
}

const List<int> soundSettingsDefault = [
  0, // 0 EqStandardPresets
  0, // 1 DspPresset
  2, // 2 Vo 2lumeControlMode
  0, // 3 Selfbot
  1, // 4 DspInitialization
  0, // 5 SoundSource
  1, // 6 Sound_Routing_USB
  1, // 7 Sound_Routing_Bluetooth
  1, // 8 Sound_Routing_Aux
  1, // 9 Sound_Routing_Radio
  1, // 10 Sound_Routing_Spdif
  1, // 11 Sound_Routing_Mic
  1, // 12 Preset_Routing_User1
  1, // 13 Preset_Routing_User2
  1, // 14 Preset_Routing_User3
  1, // 15 Preset_Routing_Flat
  1, // 16 Preset_Routing_Bass
  1, // 17 Preset_Routing_BassExtreme
  1, // 18 Preset_Routing_BassAndHigh
  1, // 19 Preset_Routing_Classic
  1, // 20 Preset_Routing_Dancing
  1, // 21 Preset_Routing_Speaker
  1, // 22 Preset_Routing_Live
  1, // 23 Preset_Routing_Medium
  1, // 24 Preset_Routing_Soft
  1, // 25 Preset_Routing_SoftBass
  1, // 26 Preset_Routing_SoftHigh
  1, // 27 LoudnessUsb
  1, // 28 LoudnessBluetooth
  1, // 29 LoudnessRadio
  1, // 30 LoudnessSpdif
  1, // 31 LoudnessMic
  0, // 32 LevelDetector
  20, // 33 VolumLevelPoewrUp
  60, // 34 TempHysteresis
  70, // 35 TempOS
];
