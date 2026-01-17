import 'package:dixom/driver/device_rx_tx/transmiter.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:dixom/driver/device_rx_tx/command_msg.dart';
import 'package:dixom/main.dart';
import 'package:shared_preferences/shared_preferences.dart';
import 'sound_enum.dart';

const int volumeMasterDef = 20;
const int volumeMax = 60;
const int volumeMin = 0;
List<int> volume = volumeDefault;

class AudioOut {
  void setDefault() {
    appRef.read(audioOutPdr.notifier).setDefault;
  }

  void receiverVolume(CommandMsg msg) {
    /*   
     * Информацию о устройстве регуляторах громкости
     * Сообщение содержится в msg.split список List<String>  
     * Параметры отмеченные * константы
     *
     * 4 3 0 1 
     * | | | +-- 3 Уровень громкости 
     * | | +---- 2 Подпараметр номер регултора грмкости
     * | +------ 1 * Параметр volume
     * +-------- 0 * Пакет sound
     * 
     * отправка 4 3=0 3
     */
    appRef.read(audioOutPdr.notifier).getVolumeFromDevice(msg);
  }

  void receiverDelay(CommandMsg msg) {
    /*   
     * Информацию о устройстве регуляторах громкости
     * Сообщение содержится в msg.split список List<String>  
     * Параметры отмеченные * константы
     *
     * 4 3 0 0.00
     * | | | +-- 3 Задержка в миллисекундах
     * | | +---- 2 Подпараметр номер канала задержки
     * | +------ 1 * Параметр volume
     * +-------- 0 * Пакет sound
     * 
     * отправка 4 3=0 3
     */
    appRef.read(audioOutPdr.notifier).getDelayFromDevice(msg);
  }

  void receiverPhase(CommandMsg msg) {
    /*   
     * Информацию о устройстве регуляторах громкости
     * Сообщение содержится в msg.split список List<String>  
     * Параметры отмеченные * константы
     *
     * 4 3 0 0.00
     * | | | +-- 3 Задержка в миллисекундах
     * | | +---- 2 Подпараметр номер канала задержки
     * | +------ 1 * Параметр volume
     * +-------- 0 * Пакет sound
     * 
     * отправка 4 3=0 3
     */
    appRef.read(audioOutPdr.notifier).getPhaseFromDevice(msg);
  }
}

Map<int, AudioOutParam> getDefaultVolume() {
  return {
    Volume.ch1L.index: AudioOutParam(0, volumeDefault[Volume.ch1L.index]),
    Volume.ch2R.index: AudioOutParam(1, volumeDefault[Volume.ch2R.index]),
    Volume.ch3L.index: AudioOutParam(2, volumeDefault[Volume.ch3L.index]),
    Volume.ch4R.index: AudioOutParam(3, volumeDefault[Volume.ch4R.index]),
    Volume.ch5L.index: AudioOutParam(4, volumeDefault[Volume.ch5L.index]),
    Volume.ch6R.index: AudioOutParam(5, volumeDefault[Volume.ch6R.index]),
    Volume.ch7L.index: AudioOutParam(6, volumeDefault[Volume.ch7L.index]),
    Volume.ch8R.index: AudioOutParam(7, volumeDefault[Volume.ch8R.index]),
    Volume.ch9L.index: AudioOutParam(8, volumeDefault[Volume.ch9L.index]),
    Volume.ch10R.index: AudioOutParam(9, volumeDefault[Volume.ch10R.index]),
    Volume.ch11L.index: AudioOutParam(10, volumeDefault[Volume.ch11L.index]),
    Volume.ch12R.index: AudioOutParam(11, volumeDefault[Volume.ch12R.index]),
    Volume.spdifOutL.index: AudioOutParam(12, volumeDefault[Volume.spdifOutL.index]),
    Volume.spdifOutR.index: AudioOutParam(13, volumeDefault[Volume.spdifOutR.index]),
    Volume.master.index: AudioOutParam(14, volumeDefault[Volume.master.index]),
    Volume.ch1L_2R.index: AudioOutParam(15, volumeDefault[Volume.ch1L_2R.index]),
    Volume.ch3L_4R.index: AudioOutParam(16, volumeDefault[Volume.ch3L_4R.index]),
  };
}

List<double> getDefaultDelay() {
  List<double> delay = [];
  for (int i = 0; i <= 14; i++) {
    delay.add(0);
  }
  return delay;
}

List<bool> getDefaultPhase() {
  List<bool> phase = [];
  for (int i = 0; i <= 14; i++) {
    phase.add(false);
  }
  return phase;
}

List<int> getDefaultGroup() {
  List<int> group = [];
  for (int i = 0; i <= 14; i++) {
    group.add(0);
  }
  return group;
}

final audioOutPdr = StateNotifierProvider<AudioOutModelNotifier, AudioOutModel>((ref) {
  return AudioOutModelNotifier(
    getDefaultVolume(),
    getDefaultDelay(),
    getDefaultPhase(),
    getDefaultGroup(),
  );
});

enum VolControl {
  packed, // 0
  parametr, // 1
  volumeNum, // 2
  volumeData, // 3
}

enum DelayControl {
  packed, // 0
  parametr, // 1
  channel, // 2
  delay, // 3
}

enum PhaseControl {
  packed, // 0
  parametr, // 1
  channel, // 2
  phase, // 3
}

final audioOutControlPdr = StateNotifierProvider<AudioOutControlNotifier, AudioOutControlModel>((ref) {
  return AudioOutControlNotifier();
});

class AudioOutControlModel {
  Volume currentControl;
  int encoderValue = 0;
  AudioOutControlModel({
    this.currentControl = Volume.ch1L,
    this.encoderValue = 0,
  });

  AudioOutControlModel copyWith({
    Volume? currentControl,
    int? encoderValue,
  }) {
    return AudioOutControlModel(
      currentControl: currentControl ?? this.currentControl,
      encoderValue: encoderValue ?? this.encoderValue,
    );
  }
}

class AudioOutParam {
  int index;
  int value;
  int oldValue;
  bool isMute;
  AudioOutParam(this.index, this.value, {this.oldValue = 0, this.isMute = false});
}

class AudioOutControlNotifier extends StateNotifier<AudioOutControlModel> {
  AudioOutControlNotifier() : super(AudioOutControlModel());

  void setCurrentChannel(Volume channel) {
    state = state.copyWith(currentControl: channel);
  }

  void setEncodrValue(int value) {
    if (value > 100) value = 0;
    if (value < 0) value = 100;
    state = state.copyWith(encoderValue: value);
  }
}

class AudioOutModel {
  Map<int, AudioOutParam> volume;
  List<double> delay;
  List<bool> phase;
  List<int> group;
  AudioOutModel({
    required this.volume,
    required this.delay,
    required this.phase,
    required this.group,
  });

  AudioOutModel copyWith({
    Map<int, AudioOutParam>? volume,
    List<double>? delay,
    List<bool>? phase,
    List<int>? group,
  }) {
    return AudioOutModel(
      volume: volume ?? this.volume,
      delay: delay ?? this.delay,
      phase: phase ?? this.phase,
      group: group ?? this.group,
    );
  }
}

String channelGroupSave = "CHANNEL_GROUP_SAVE";

class AudioOutModelNotifier extends StateNotifier<AudioOutModel> {
  AudioOutModelNotifier(
    Map<int, AudioOutParam> volume,
    List<double> delay,
    List<bool> phase,
    List<int> group,
  ) : super(AudioOutModel(volume: volume, delay: delay, phase: phase, group: group));

  void getVolumeFromDevice(CommandMsg msg) {
    int key = int.parse(msg.split[VolControl.volumeNum.index]);
    int value = int.parse(msg.split[VolControl.volumeData.index]);

    if (state.volume[key] == null) {
      //Если нет данных то добавляем
      AudioOutParam param = AudioOutParam(state.volume.length, value);
      state.volume[key] = param;
    } else {
      //Если есть то изменяем
      state.volume[key] = AudioOutParam(state.volume[key]!.index, value);
    }

    state = state.copyWith(volume: state.volume);
  }

  void setVolumeOnDevice(int key, int value, {bool isMute = false}) {
    //if (state.volume[key] is AudioOutParam) {
      if (value > 60) value = 60;
      if (value < 0) value = 0;
    
      AudioOutParam param =
          AudioOutParam(state.volume[key]!.index, value, oldValue: state.volume[key]!.value, isMute: isMute);

      state.volume[key] = param;
      state = state.copyWith(volume: state.volume);
      //Отправляем новые данные в аудиоплатформу
      transmitter("4 3=$key $value");
   // }
  }

  void getDelayFromDevice(CommandMsg msg) {
    int channel = int.parse(msg.split[DelayControl.channel.index]);
    double delay = double.parse(msg.split[DelayControl.delay.index]);
    state.delay[channel] = delay;
    state = state.copyWith(delay: state.delay);
  }

  void getPhaseFromDevice(CommandMsg msg) {
    int channel = int.parse(msg.split[PhaseControl.channel.index]);
    int delay = int.parse(msg.split[PhaseControl.phase.index]);
    state.phase[channel] = delay == 0 ? false : true;
    state = state.copyWith(phase: state.phase);
  }

  void setDelayOnDevice(int channel, double delay) {
    if (delay > 30) delay = 30;
    if (delay < 0) delay = 0;

    double dif = state.delay[channel] - delay;

    if (state.group[channel] != 0) {
      for (int i = 0; i < state.group.length; i++) {
        if (state.group[channel] == state.group[i]) {
          state.delay[i] = state.delay[i] - dif;
          if (state.delay[i] > 30) state.delay[i] = 30;
          if (state.delay[i] < 0) state.delay[i] = 0;
          transmitter("4 21=$i ${state.delay[i].toStringAsFixed(2)}");
        }
      }
    } else {
      state.delay[channel] = state.delay[channel] - dif;
      if (state.delay[channel] > 30) state.delay[channel] = 30;
      if (state.delay[channel] < 0) state.delay[channel] = 0;
      transmitter("4 21=$channel ${delay.toStringAsFixed(2)}");
    }
    state = state.copyWith(delay: state.delay);
  }

  void loadGroup(int channel, int group) {
    state.group[channel] = group;
    state = state.copyWith(group: state.group);
  }

  Future<void> setGroup(int channel, int group) async {
    SharedPreferences pref = await SharedPreferences.getInstance();
    state.group[channel] = group;
    state = state.copyWith(group: state.group);
    pref.setInt("$channelGroupSave$channel", state.group[channel]);
  }

  void setPhase(int channel) {
    state.phase[channel] = state.phase[channel] == true ? false : true;
    state = state.copyWith(phase: state.phase);
    transmitter("4 24=$channel ${state.phase[channel] == true ? "1" : "0"}");
  }

  void setVolume(int key, int volume) {
    state.volume[key]?.value = volume;
    state = state.copyWith(volume: state.volume);
  }

  void setDefault() {
    state = state.copyWith(
      volume: getDefaultVolume(),
      delay: getDefaultDelay(),
      phase: getDefaultPhase(),
    );
  }
}

const List<int> volumeDefault = [
  volumeMasterDef, //0 General master volume
  volumeMin, //1 Max volume bluetooth
  volumeMin, //2 Max volume aux
  volumeMin, //3 Max volume radio
  volumeMin, //4 Max volume spdif
  volumeMin, //5 Min volume usb
  volumeMin, //6 Min volume bluetooth
  volumeMin, //7 Min volume aux
  volumeMin, //8 Min volume radio
  volumeMin, //9 Min volume spdif
  volumeMax, //10 Master volume dsp
  volumeMax, //11 volume usb
  volumeMax, //12 volume usb L
  volumeMax, //13 volume usb R
  volumeMin, //14 volume bluetooth
  volumeMax, //15 volume bluetooth L
  volumeMax, //16 volume bluetooth R
  volumeMin, //17 volume aux
  volumeMax, //18 volume aux L
  volumeMax, //19 volume aux R
  volumeMin, //20 volume radio
  volumeMax, //21 volume radio L
  volumeMax, //22 volume radio R
  volumeMin, //23 volume spdif
  volumeMax, //24 volume spdif L
  volumeMax, //25 volume spdif R
  volumeMin, //26 volume microphone
  volumeMax, //27 volume microphone L
  volumeMax, //28 volume microphone R
  volumeMax, //29 volume channel 1L-2R
  volumeMax, //30 volume channel 1L
  volumeMax, //31 volume channel 2R
  volumeMax, //32 volume channel 3L-4R
  volumeMax, //33 volume channel 3L
  volumeMax, //34 volume channel 4R
  volumeMax, //35 volume channel 5L-6R
  volumeMax, //36 volume channel 5L
  volumeMax, //37 volume channel 6R
  volumeMax, //38 volume channel 7L-8R
  volumeMax, //39 volume channel 7L
  volumeMax, //40 volume channel 8R
  volumeMax, //41 volume channel 9L-10R
  volumeMax, //42 volume channel 9L
  volumeMax, //43 volume channel 10R
  volumeMax, //44 volume channel 11L-12R
  volumeMax, //45 volume channel 11L
  volumeMax, //46 volume channel 12R
  volumeMax, //47 volume channel SPDIFL-SPDIFR
  volumeMax, //48 volume channel SPDIFL
  volumeMax, //49 volume channel SPDIFR

  volumeMax, //50 volume usb L channel 1L
  volumeMin, //51 volume usb L channel 2R
  volumeMax, //52 volume usb L channel 3L
  volumeMin, //53 volume usb L channel 4R
  volumeMax, //54 volume usb L channel 5L
  volumeMin, //55 volume usb L channel 6R
  volumeMax, //56 volume usb L channel 7L
  volumeMin, //57 volume usb L channel 8R
  volumeMax, //58 volume usb L channel 9L
  volumeMin, //59 volume usb L channel 10R
  volumeMax, //60 volume usb L channel 11L
  volumeMin, //61 volume usb L channel 12R
  volumeMax, //62 volume usb L channel SPDIFL
  volumeMin, //63 volume usb L channel SPDIFR
  volumeMin, //64 volume usb R channel 1L
  volumeMax, //65 volume usb R channel 2R
  volumeMin, //66 volume usb R channel 3L
  volumeMax, //67 volume usb R channel 4R
  volumeMin, //68 volume usb R channel 5L
  volumeMax, //69 volume usb R channel 6R
  volumeMin, //70 volume usb R channel 7L
  volumeMax, //71 volume usb R channel 8R
  volumeMin, //72 volume usb R channel 9L
  volumeMax, //73 volume usb R channel 10R
  volumeMin, //74 volume usb R channel 11L
  volumeMax, //75 volume usb R channel 12R
  volumeMin, //76 volume usb R channel SPDIFL
  volumeMax, //77 volume usb R channel SPDIFR

  volumeMax, //78 volume bluetooth L channel 1L
  volumeMin, //79 volume bluetooth L channel 2R
  volumeMax, //80 volume bluetooth L channel 3L
  volumeMin, //81 volume bluetooth L channel 4R
  volumeMax, //82 volume bluetooth L channel 5L
  volumeMin, //83 volume bluetooth L channel 6R
  volumeMax, //84 volume bluetooth L channel 7L
  volumeMin, //85 volume bluetooth L channel 8R
  volumeMax, //86 volume bluetooth L channel 9L
  volumeMin, //87 volume bluetooth L channel 10R
  volumeMax, //88 volume bluetooth L channel 11L
  volumeMin, //89 volume bluetooth L channel 12R
  volumeMax, //90 volume bluetooth L channel SPDIFL
  volumeMin, //91 volume bluetooth L channel SPDIFR
  volumeMin, //92 volume bluetooth R channel 1L
  volumeMax, //93 volume bluetooth R channel 2R
  volumeMin, //94 volume bluetooth R channel 3L
  volumeMax, //95 volume bluetooth R channel 4R
  volumeMin, //96 volume bluetooth R channel 5L
  volumeMax, //97 volume bluetooth R channel 6R
  volumeMin, //98 volume bluetooth R channel 7L
  volumeMax, //99 volume bluetooth R channel 8R
  volumeMin, //100 volume bluetooth R channel 9L
  volumeMax, //101 volume bluetooth R channel 10R
  volumeMin, //102 volume bluetooth R channel 11L
  volumeMax, //103 volume bluetooth R channel 12R
  volumeMin, //104 volume bluetooth R channel SPDIFL
  volumeMax, //105 volume bluetooth R channel SPDIFR

  volumeMax, //106 volume aux L channel 1L
  volumeMin, //107 volume aux L channel 2R
  volumeMax, //108 volume aux L channel 3L
  volumeMin, //109 volume aux L channel 4R
  volumeMax, //110 volume aux L channel 5L
  volumeMin, //111 volume aux L channel 6R
  volumeMax, //112 volume aux L channel 7L
  volumeMin, //113 volume aux L channel 8R
  volumeMax, //114 volume aux L channel 9L
  volumeMin, //115 volume aux L channel 10R
  volumeMax, //116 volume aux L channel 11L
  volumeMin, //117 volume aux L channel 12R
  volumeMax, //118 volume aux L channel SPDIFL
  volumeMin, //119 volume aux L channel SPDIFR
  volumeMin, //120 volume aux R channel 1L
  volumeMax, //121 volume aux R channel 2R
  volumeMin, //122 volume aux R channel 3L
  volumeMax, //123 volume aux R channel 4R
  volumeMin, //124 volume aux R channel 5L
  volumeMax, //125 volume aux R channel 6R
  volumeMin, //126 volume aux R channel 7L
  volumeMax, //127 volume aux R channel 8R
  volumeMin, //128 volume aux R channel 9L
  volumeMax, //129 volume aux R channel 10R
  volumeMin, //130 volume aux R channel 11L
  volumeMax, //131 volume aux R channel 12R
  volumeMin, //132 volume aux R channel SPDIFL
  volumeMax, //133 volume aux R channel SPDIFR

  volumeMax, //134 volume radio L channel 1L
  volumeMin, //135 volume radio L channel 2R
  volumeMax, //136 volume radio L channel 3L
  volumeMin, //137 volume radio L channel 4R
  volumeMax, //138 volume radio L channel 5L
  volumeMin, //139 volume radio L channel 6R
  volumeMax, //140 volume radio L channel 7L
  volumeMin, //141 volume radio L channel 8R
  volumeMax, //142 volume radio L channel 9L
  volumeMin, //143 volume radio L channel 10R
  volumeMax, //144 volume radio L channel 11L
  volumeMin, //145 volume radio L channel 12R
  volumeMax, //146 volume radio L channel SPDIFL
  volumeMin, //147 volume radio L channel SPDIFR
  volumeMin, //148 volume radio R channel 1L
  volumeMax, //149 volume radio R channel 2R
  volumeMin, //150 volume radio R channel 3L
  volumeMax, //151 volume radio R channel 4R
  volumeMin, //152 volume radio R channel 5L
  volumeMax, //153 volume radio R channel 6R
  volumeMin, //154 volume radio R channel 7L
  volumeMax, //155 volume radio R channel 8R
  volumeMin, //156 volume radio R channel 9L
  volumeMax, //157 volume radio R channel 10R
  volumeMin, //158 volume radio R channel 11L
  volumeMax, //159 volume radio R channel 12R
  volumeMin, //160 volume radio R channel SPDIFL
  volumeMax, //161 volume radio R channel SPDIFR

  volumeMax, //162 volume spdif L channel 1L
  volumeMin, //163 volume spdif L channel 2R
  volumeMax, //164 volume spdif L channel 3L
  volumeMin, //165 volume spdif L channel 4R
  volumeMax, //166 volume spdif L channel 5L
  volumeMin, //167 volume spdif L channel 6R
  volumeMax, //168 volume spdif L channel 7L
  volumeMin, //169 volume spdif L channel 8R
  volumeMax, //170 volume spdif L channel 9L
  volumeMin, //171 volume spdif L channel 10R
  volumeMax, //172 volume spdif L channel 11L
  volumeMin, //173 volume spdif L channel 12R
  volumeMax, //174 volume spdif L channel SPDIFL
  volumeMin, //175 volume spdif L channel SPDIFR
  volumeMin, //176 volume spdif R channel 1L
  volumeMax, //177 volume spdif R channel 2R
  volumeMin, //178 volume spdif R channel 3L
  volumeMax, //179 volume spdif R channel 4R
  volumeMin, //182 volume spdif R channel 5L
  volumeMax, //181 volume spdif R channel 6R
  volumeMin, //182 volume spdif R channel 7L
  volumeMax, //183 volume spdif R channel 8R
  volumeMin, //184 volume spdif R channel 9L
  volumeMax, //185 volume spdif R channel 10R
  volumeMin, //186 volume spdif R channel 11L
  volumeMax, //187 volume spdif R channel 12R
  volumeMin, //188 volume spdif R channel SPDIFL
  volumeMax, //189 volume spdif R channel SPDIFR

  volumeMax, //190 volume microphone L channel 1L
  volumeMin, //191 volume microphone L channel 2R
  volumeMax, //192 volume microphone L channel 3L
  volumeMin, //193 volume microphone L channel 4R
  volumeMax, //194 volume microphone L channel 5L
  volumeMin, //195 volume microphone L channel 6R
  volumeMax, //196 volume microphone L channel 7L
  volumeMin, //197 volume microphone L channel 8R
  volumeMax, //198 volume microphone L channel 9L
  volumeMin, //199 volume microphone L channel 10R
  volumeMax, //200 volume microphone L channel 11L
  volumeMin, //201 volume microphone L channel 12R
  volumeMax, //202 volume microphone L channel SPDIFL
  volumeMin, //203 volume microphone L channel SPDIFR
  volumeMin, //204 volume microphone R channel 1L
  volumeMax, //205 volume microphone R channel 2R
  volumeMin, //206 volume microphone R channel 3L
  volumeMax, //207 volume microphone R channel 4R
  volumeMin, //208 volume microphone R channel 5L
  volumeMax, //209 volume microphone R channel 6R
  volumeMin, //210 volume microphone R channel 7L
  volumeMax, //211 volume microphone R channel 8R
  volumeMin, //212 volume microphone R channel 9L
  volumeMax, //213 volume microphone R channel 10R
  volumeMin, //214 volume microphone R channel 11L
  volumeMax, //215 volume microphone R channel 12R
  volumeMin, //216 volume microphone R channel SPDIFL
  volumeMax, //217 volume microphone R channel SPDIFR

  volumeMax, //218 volume beep
  volumeMax, //219 volume beep channel 1L
  volumeMax, //220 volume beep channel 2R
  volumeMax, //221 volume beep channel 3L
  volumeMax, //222 volume beep channel 4R
  volumeMax, //223 volume beep channel 5L
  volumeMax, //224 volume beep channel 6R
  volumeMax, //225 volume beep channel 7L
  volumeMax, //226 volume beep channel 8R
  volumeMax, //227 volume beep channel 9L
  volumeMax, //228 volume beep channel 10R
  volumeMax, //229 volume beep channel 11L
  volumeMax, //230 volume beep channel 12R
  volumeMax, //231 volume beep channel SPDIFL
  volumeMax, //232 volume beep channel SPDIFR

  volumeMax, //233 volume noise
  volumeMax, //234 volume noise channel 1L
  volumeMax, //235 volume noise channel 2R
  volumeMax, //236 volume noise channel 3L
  volumeMax, //237 volume noise channel 4R
  volumeMax, //238 volume noise channel 5L
  volumeMax, //239 volume noise channel 6R
  volumeMax, //240 volume noise channel 7L
  volumeMax, //241 volume noise channel 8R
  volumeMax, //242 volume noise channel 9L
  volumeMax, //243 volume noise channel 10R
  volumeMax, //244 volume noise channel 11L
  volumeMax, //245 volume noise channel 12R
  volumeMax, //246 volume noise channel SPDIFL
  volumeMax, //247 volume noise channel SPDIFR
];
