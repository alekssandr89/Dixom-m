import 'package:dixom/driver/device_rx_tx/transmiter.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:dixom/driver/device_rx_tx/command_msg.dart';
import 'package:dixom/main.dart';
import 'package:shared_preferences/shared_preferences.dart';
import 'biquadratic.dart';
import 'sound.dart';
import 'sound_enum.dart';

class SoundFilter {
  void setDefault() {
    for (int i = 0; i < soundEqProvider.length; i++) {
      appRef.read(soundEqProvider[i].notifier).defaultAll();
    }
  }

  /*
   * Сообщение содержится в msg.split список List<String>  
   * Параметры отмеченные * константы
   *
   */
  void receiverEq(CommandMsg msg) {
    /*   
     * Информацию о эквалайзерах
     * 
     * 4 20 1 5 1 1 4.00 260 2.50
     * | |  | | | |  |    |   +---- 8 добротность q 0.2 - 2
     * | |  | | | |  |    +-------- 7 Частота freq 20 - 20000
     * | |  | | | |  +------------- 6 Усиление boost -15 + 10
     * | |  | | | +---------------- 5 Тип фильтра по умолчанию peak
     * | |  | | +------------------ 4 Состояние полосы 1 вкл 0 откл state
     * | |  | +-------------------- 3 номер полосы эквалайзера band
     * | |  +---------------------- 2 Канал аудиовыхода channel
     * | +------------------------- 1 Параметр* equaluzer
     * +--------------------------- 0 Пакет* sound
     * 
     * отправка 4 20=1 5 1 1 4.00 260 2.50 1 (если в конце добавить 1 будут
     * возвращаться изменёные данные)
     * будет выставлено изменение эквалайзера 1-го канала 5-й полосы
     * 
     */
    appRef.read(soundEqProvider[msg.podParmetr].notifier).updateEq(msg);
  }

  void receiverCrossover(CommandMsg msg) {
    /*   
     * Информацию о кроссовере
     *
     * 4 23 0 0 0 5 70 2 0.71
     * |  | | | | | |  |  +---Добротность
     * |  | | | | | |  +------Порядок
     * |  | | | | | +---------Частота
     * |  | | | | +-----------Тип фильтра
     * |  | | | +-------------Состояние 1 включен, 0 отключен
     * |  | | +---------------Полоса кроссовера 1 LowPass 0 HiPass (можно менять)
     * |  | +-----------------Подпараметр. Канал
     * |  +-------------------Парметр*. Кроссовер
     * +----------------------Пакет*. Sound   
     * 
     * отправка 4 23=0 0 0 5 70 2 0.71 1 (если в конце добавить 1 будут
     * возвращаться изменёные данные)
     * будет выставлено изменение эквалайзера 1-го канала
     * 
     */
    appRef.read(soundEqProvider[msg.podParmetr].notifier).updateCrossover(msg);
  }

  void receiverGroup(CommandMsg msg) {
    /*   
     * Информацию о группировке
     *
     * 4 14 0 1  
     * |  | | +---------------Номер группы
     * |  | +-----------------Канал который хочет сгруппироваться
     * |  +-------------------Парметр*. Группировка канала
     * +----------------------Пакет*. Sound   
     * 
     * отправка 4 14=0 0. 0-й канал добвлен в группу 0, Если отправить 255 то группы не будет.
     * будет выставлено изменение эквалайзера 1-го канала
     * 
     */
    appRef.read(deviceSoundPrdr.notifier).getGrouped(msg);
  }

  void receiverGain(CommandMsg msg) {
    /*   
     * Информацию о группировке
     *
     * 4 14 0 0.0
     * |  | | +---------------Номер гейн
     * |  | +-----------------Канал
     * |  +-------------------Парметр*. Гейн
     * +----------------------Пакет*. Sound   
     * 
     * отправка 4 14=0 0. 0-й канал добвлен в группу 0, Если отправить 255 то группы не будет.
     * будет выставлено изменение эквалайзера 1-го канала
     * 
     */
    appRef.read(soundEqProvider[msg.podParmetr].notifier).getGain(msg);
  }

  void receiverBypass(CommandMsg msg) {
    /*   
     * Информацию о группировке
     *
     * 4 19 0 1  
     * |  | | +---------------Состояние байпасса 1 включен, 0 отключен
     * |  | +-----------------Канал
     * |  +-------------------Парметр*. Байпас эквалайзера
     * +----------------------Пакет*. Sound   
     * 
     * отправка 4 14=0 0. 0-й канал добвлен в группу 0, Если отправить 255 то группы не будет.
     * будет выставлено изменение эквалайзера 1-го канала
     * 
     */
    appRef.read(soundEqProvider[msg.podParmetr].notifier).getBypass(msg);
  }
}

//Функция-генератор для создания провайдера
StateNotifierProvider<DspEqNotifier, DspFilterModel> createFilterNotifier(
    int channel, int groupChannel, ChannelId channelId, GroupId groupId, List<DspFilter> def) {
  return StateNotifierProvider<DspEqNotifier, DspFilterModel>(
      (ref) => DspEqNotifier(channel, groupChannel, channelId, groupId, def));
}

//Функция-генератор для создания провайдера
final List<StateNotifierProvider<DspEqNotifier, DspFilterModel>> soundEqProvider = [
  createFilterNotifier(0, 1, ChannelId.channel_1, GroupId.group_1, eqDefault),
  createFilterNotifier(1, 0, ChannelId.channel_2, GroupId.group_1, eqDefault),
  createFilterNotifier(2, 3, ChannelId.channel_3, GroupId.group_2, eqDefault),
  createFilterNotifier(3, 2, ChannelId.channel_4, GroupId.group_2, eqDefault),
  createFilterNotifier(4, 5, ChannelId.channel_5, GroupId.group_3, eqDefault),
  createFilterNotifier(5, 4, ChannelId.channel_6, GroupId.group_3, eqDefault),
  createFilterNotifier(6, 7, ChannelId.channel_7, GroupId.group_4, eqDefault),
  createFilterNotifier(7, 6, ChannelId.channel_8, GroupId.group_4, eqDefault),
  createFilterNotifier(8, 9, ChannelId.channel_9, GroupId.group_5, eqDefault),
  createFilterNotifier(9, 8, ChannelId.channel_10, GroupId.group_5, eqDefault),
  createFilterNotifier(10, 11, ChannelId.channel_11, GroupId.group_6, eqDefault),
  createFilterNotifier(11, 10, ChannelId.channel_12, GroupId.group_6, eqDefault),
  createFilterNotifier(12, 13, ChannelId.spdifL, GroupId.spdif, eqDefault),
  createFilterNotifier(13, 12, ChannelId.spdifR, GroupId.spdif, eqDefault),
];

//Параметры отправки данных из устройства
enum EqExch {
  packed,
  parametr,
  channel,
  band,
  bandState,
  type,
  boost,
  frequency,
  q,
}

enum CrossExch {
  packed, //Пакет Sound
  parametr, //Парметр кроссовер
  channel, //Канал
  band, //Полоса кроссовера 1 LowPass 0 HiPass
  state, //Состояние 1 включен, 0 отключен
  type, //Тип фильтра
  frequency, //Частота
  order, //Порядок
  q, //Добротность
}

enum GainExch {
  packed, //Пакет Sound
  parametr, //Парметр кроссовер
  channel, //Канал
  gain, //Усиление
}

enum BypassExch {
  packed, //Пакет Sound
  parametr, //Парметр кроссовер
  channel, //Канал
  bypass, //Байпас
}

const String visabylityPref = "visabylityPref";

class DspFilterModel {
  List<DspFilter> eq; //Параметры полос
  List<int> x = []; //Данные для отрисовки графика по x
  List<double> y = []; //Данные для отрисовки графика по Y
  DspFilter hiPass; //Хайпас фильтр
  DspFilter lowPass; //Ловпасс фильтр
  bool bypass; //состояние филтра
  bool visibility; //отображение на графике
  final int channel; //номер канала
  int groupChannel; //номер группы в канале
  int groupDelay; //номер группы задержек
  double delay; //Задержка канала
  double gain; //гейн канала
  bool invert; //фаза
  String channelName; //Имя канала
  String groupName; //Имя канала
  int currentControl; //Указатель, какая полоса настравивается
  ChannelId channelId;
  GroupId groupId;

  DspFilterModel({
    List<DspFilter> eq = eqDefault,
    this.hiPass = hiPassDefault,
    this.lowPass = lowPassDefault,
    this.bypass = true,
    this.gain = 0.0,
    this.groupChannel = 255,
    this.groupDelay = 255,
    this.channel = 0,
    this.delay = 0,
    this.invert = false,
    this.channelName = "*",
    this.currentControl = 0,
    this.visibility = false,
    this.groupName = "*",
    required this.groupId,
    required this.channelId,
  }) : eq = List<DspFilter>.from(eq) {
    if (channelName == "*") {
      channelName = channelId.decoding;
    }

    if (groupName == "*") {
      groupName = groupId.decoding;
    }
  }

  DspFilterModel copyWith({
    List<DspFilter>? eq,
    DspFilter? hiPass,
    DspFilter? lowPass,
    bool? bypass,
    double? gain,
    int? groupDelay,
    int? groupChannel,
    int? currentControl,
    List<double>? x,
    List<double>? y,
    bool? visibility,
    int? channel,
    String? channelName,
    String? groupName,
    double? delay,
    bool? invert,
  }) {
    return DspFilterModel(
      eq: eq ?? this.eq,
      hiPass: hiPass ?? this.hiPass,
      lowPass: lowPass ?? this.lowPass,
      bypass: bypass ?? this.bypass,
      gain: gain ?? this.gain,
      groupChannel: groupChannel ?? this.groupChannel,
      groupDelay: groupDelay ?? this.groupDelay,
      currentControl: currentControl ?? this.currentControl,
      visibility: visibility ?? this.visibility,
      channel: channel ?? this.channel,
      channelName: channelName ?? this.channelName,
      channelId: channelId,
      groupId: groupId,
      groupName: groupName ?? this.groupName,
      delay: delay ?? this.delay,
      invert: invert ?? this.invert,
    );
  }

  DspFilterModel updateFilter(int band, DspFilter filter) {
    eq[band] = filter;
    return copyWith(
      eq: eq,
      currentControl: band,
    );
  }
}

class DspEqNotifier extends StateNotifier<DspFilterModel> {
  DspEqNotifier(int channe, int groupChannel, ChannelId channelId, GroupId groupId, List<DspFilter> eq)
      : super(DspFilterModel(
          groupChannel: groupChannel,
          channel: channe,
          eq: eq,
          channelId: channelId,
          groupId: groupId,
        ));

  void updateEq(CommandMsg msg) {
    int band = int.parse(msg.split[EqExch.band.index]);
    state = state.updateFilter(
      band,
      DspFilter(
        int.parse(msg.split[EqExch.frequency.index]),
        double.parse(msg.split[EqExch.q.index]),
        double.parse(msg.split[EqExch.boost.index]),
        DspFilterType.peak,
        int.parse(msg.split[EqExch.frequency.index]),
        msg.split[EqExch.bandState.index] == "1" ? true : false,
      ),
    );
  }

  void defaultAll() {
    state = state.copyWith(
      eq: eqDefault,
      hiPass: hiPassDefault,
      lowPass: lowPassDefault,
      bypass: true,
      gain: 0.0,
      delay: 0,
      invert: false,
    );
  }

  //Изменение имени канала
  void copyState(List<DspFilter> eq, DspFilter hiPass, DspFilter lowPass, bool bypass, double gain) {
    state = state.copyWith(
      eq: eq,
      hiPass: hiPass,
      lowPass: lowPass,
      bypass: bypass,
      gain: gain,
    );
  }

  //Копирование провайдеров для группировки
  void copyToGroupedProvider() {
    appRef.read(soundEqProvider[state.groupChannel].notifier).copyState(
          state.eq,
          state.hiPass,
          state.lowPass,
          state.bypass,
          state.gain,
        );
  }

  //Изменение имени канала
  void updateChannelName(String name) {
    state = state.copyWith(channelName: name);
  }

  //Изменение имени группы
  void updateGroupName(String name) {
    state = state.copyWith(groupName: name);
  }

  //Сохранение имени канала в памяти
  Future<void> saveChannelName() async {
    SharedPreferences pref = await SharedPreferences.getInstance();
    pref.setString(state.channelId.decoding, state.channelName);
  }

  //Сохранение имени группы в памяти
  Future<void> saveGroupName() async {
    SharedPreferences pref = await SharedPreferences.getInstance();
    pref.setString(state.groupId.decoding, state.groupName);
  }

  void updateCrossover(CommandMsg msg) {
    DspFilter filter = DspFilter(
      int.parse(msg.split[CrossExch.frequency.index]),
      double.parse(msg.split[CrossExch.q.index]),
      0,
      DspFilterType.values[int.parse(msg.split[CrossExch.type.index])],
      int.parse(msg.split[CrossExch.order.index]),
      msg.split[CrossExch.state.index] == "1" ? true : false,
    );

    if (msg.split[CrossExch.band.index] == "1") {
      state = state.copyWith(lowPass: filter);
    } else {
      state = state.copyWith(hiPass: filter);
    }
  }

  void updateGain(double? gain) {
    state = state.copyWith(gain: gain);
    transmitter("4 13=${state.channel} ${state.gain.toStringAsFixed(2)}");
  }

  void updateState() {
    String bypassState = "0";
    if (state.bypass) {
      state = state.copyWith(bypass: false);
      bypassState = "1";
    } else {
      state = state.copyWith(bypass: true);
    }
    transmitter("4 19=${state.channel} $bypassState");
  }

  void defaultEq() {
    state = DspFilterModel(
      hiPass: state.hiPass,
      lowPass: state.lowPass,
      groupChannel: state.groupChannel,
      groupDelay: state.groupDelay,
      channel: state.channel,
      delay: state.delay,
      invert: state.invert,
      channelName: state.channelName,
      currentControl: state.currentControl,
      visibility: state.visibility,
      channelId: state.channelId,
      groupName: state.groupName,
      groupId: state.groupId,
    );
    transmitter("4 ${SoundPacked.equalizer.index}~${state.channel}");
  }

  void defaultCrossover() {
    state = DspFilterModel(
      eq: state.eq,
      gain: state.gain,
      bypass: state.bypass,
      groupChannel: state.groupChannel,
      groupDelay: state.groupDelay,
      channel: state.channel,
      delay: state.delay,
      invert: state.invert,
      channelName: state.channelName,
      currentControl: state.currentControl,
      visibility: state.visibility,
      channelId: state.channelId,
      groupName: state.groupName,
      groupId: state.groupId,
    );
    transmitter("4 ${SoundPacked.crossover.index}~${state.channel}");
  }

  Future<void> updateVisabylity() async {
    bool visabylity = false;
    if (state.visibility) {
      visabylity = false;
    } else {
      visabylity = true;
    }
    SharedPreferences pref = await SharedPreferences.getInstance();
    String send = "$visabylityPref${state.channel}";
    pref.setBool(send, visabylity);
    state = state.copyWith(visibility: visabylity);
  }

  void setVisabylity(bool visabylity) {
    state = state.copyWith(visibility: visabylity);
  }

  void updateUiEq(int band, {int? freq, double? q, double? boost, int? order, bool? status}) {
    //Обновляем интерфейс
    if (freq != null) {
      if (freq > 20000) freq = 20000;
      if (freq < 20) freq = 20;
    }

    if (boost != null) {
      if (boost > 15) boost = 15;
      if (boost < -20) boost = -20;
    }

    if (q != null) {
      if (q > 6) q = 6;
      if (q < 0.1) q = 0.1;
    }

    state = state.updateFilter(
      band,
      DspFilter(
        freq ?? state.eq[band].freq,
        q ?? state.eq[band].q,
        boost ?? state.eq[band].boost,
        DspFilterType.peak,
        2,
        status ?? state.eq[band].status,
      ),
    );
    String cmd =
        "4 ${SoundPacked.equalizer.index}=${state.channel} $band ${state.eq[band].status == true ? "1" : "0"} ${DspFilterType.peak.index} ${state.eq[band].boost.toStringAsFixed(2)} ${state.eq[band].freq} ${state.eq[band].q.toStringAsFixed(2)}";
    //4 20=1 5 1 1 4.00 260 2.50 Отправляем данные в платформу
    //|  | | | | |  |    |   +---Добротность
    //|  | | | | |  |    +-------Частота
    //|  | | | | |  +------------Усиление
    //|  | | | | +---------------Тип фильтра (для эквалайзера всегда peak)
    //|  | | | +-----------------Состояние полосы 1 включен, 0 отключен
    //|  | | +-------------------Полоса эквализации
    //|  | +---------------------Канал
    //|  +-----------------------Парметр кроссовер
    //+--------------------------Пакет Sound
    transmitter(cmd);
  }

  void getGain(CommandMsg msg) {
    state = state.copyWith(gain: double.parse(msg.split[GainExch.gain.index]));
  }

  void getBypass(CommandMsg msg) {
    bool bypass = false;
    if (msg.split[BypassExch.bypass.index] == "0") {
      bypass = true;
    }
    state = state.copyWith(bypass: bypass);
  }

  void updateUiCrossover(
    DspFilterType fType, {
    int? freq,
    double? q,
    double? boost,
    int? order,
    bool? status,
    DspFilterType? type,
  }) {
    //Обновляем интерфейс

    if (fType == DspFilterType.lowPass) {
      appRef.read(deviceSoundPrdr.notifier).setCrssover(CurrentFilterType.lowPass);
    } else if (fType == DspFilterType.highPass) {
      appRef.read(deviceSoundPrdr.notifier).setCrssover(CurrentFilterType.hiPass);
    }

    if (freq != null && (freq < 20 || freq > 20000)) {
      if (freq < 20) freq = 20;
      if (freq > 20000) freq = 20000;
    }

    if (order != null) {
      if (order > 6) order = 6;
      if (order < 1) order = 1;
    }

    String cmd;
    if (fType == DspFilterType.lowPass ||
        fType == DspFilterType.besselLowPass ||
        fType == DspFilterType.butterworthLowPass ||
        fType == DspFilterType.linkwitzRileyLowPass) {
      DspFilter lowPass = DspFilter(
        freq ?? state.lowPass.freq,
        q ?? state.lowPass.q,
        boost ?? state.lowPass.boost,
        type ?? state.lowPass.type,
        order ?? state.lowPass.order,
        status ?? state.lowPass.status,
      );
      state = state.copyWith(lowPass: lowPass);
      cmd =
          "4 ${SoundPacked.crossover.index}=${state.channel} 1 ${lowPass.status == true ? "1" : "0"} ${lowPass.type.index} ${lowPass.freq} ${lowPass.order} ${lowPass.q.toStringAsFixed(2)}";
    } else {
      DspFilter hiPass = DspFilter(
        freq ?? state.hiPass.freq,
        q ?? state.hiPass.q,
        boost ?? state.hiPass.boost,
        type ?? state.hiPass.type,
        order ?? state.hiPass.order,
        status ?? state.hiPass.status,
      );
      state = state.copyWith(hiPass: hiPass);
      cmd =
          "4 ${SoundPacked.crossover.index}=${state.channel} 0 ${hiPass.status == true ? "1" : "0"} ${hiPass.type.index} ${hiPass.freq} ${hiPass.order} ${hiPass.q.toStringAsFixed(2)}";
    }
    //4 23=0 0 0 5 70 2 0.71 Отправляем данные в платформу
    //|  | | | | | |  |  +--- Добротность
    //|  | | | | | |  +-------Порядок
    //|  | | | | | +----------Частота
    //|  | | | | +------------Тип фильтра
    //|  | | | +--------------Состояние 1 включен, 0 отключен
    //|  | | +----------------Полоса кроссовера 1 LowPass 0 HiPass (можно менять)
    //|  | +------------------Канал
    //|  +--------------------Парметр кроссовер
    //+-----------------------Пакет Sound
    transmitter(cmd);
  }

  void setDefault() {
    state = DspFilterModel(channelId: state.channelId, groupId: state.groupId);
  }
}

enum EqChannel {
  source, //0
  channel1, //1
  channel2, //2
  channel3, //3
  channel4, //4
  channel5, //5
  channel6, //6
  channel7, //7
  channel8, //8
  channel9, //9
  channel10, //10
  channel11, //11
  channel12, //12
}
