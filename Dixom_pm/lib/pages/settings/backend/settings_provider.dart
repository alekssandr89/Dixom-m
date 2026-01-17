import 'package:dixom/driver/device_rx_tx/command_msg.dart';
import 'package:dixom/driver/device_rx_tx/exchange_param.dart';
import 'package:dixom/driver/device_rx_tx/transmiter.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'settings_const.dart';
import 'settings_model.dart';

final senttingsPrdr = StateNotifierProvider<SettingsModelNotifier, SettingsModel>((ref) => SettingsModelNotifier());

class SettingsModelNotifier extends StateNotifier<SettingsModel> {
  SettingsModelNotifier() : super(const SettingsModel());

  void updateUi() {
    int updateUi = state.update;
    updateUi++;
    state = state.copyWith(
      update: updateUi,
    );
  }

  void setDefault() {
    for (int i = 0; i < generalSettings.length; i++) {
      generalSettings[i].data = defaultGenralSettings[i];
    }
    updateUi();
  }

  void setSettins(int param, int data) {
    generalSettings[param].data = data;
    transmitter("1 2=$param $data");
    updateUi();
  }
}

final rtcPrdr = StateNotifierProvider<RtcModelNotifier, RtcModel>((ref) => RtcModelNotifier());

class RtcModelNotifier extends StateNotifier<RtcModel> {
  RtcModelNotifier() : super(RtcModel(dataTime: DateTime.now()));

  void setCurrentDataTime() {
    DateTime currentDT = DateTime.now();
    transmitter(
        "${RootPacked.general.index} ${General.dataTime.index}=${currentDT.hour} ${currentDT.minute} ${currentDT.second} ${currentDT.weekday} ${currentDT.day} ${currentDT.month} ${currentDT.year}");
    getTemperatureAndDataTime();
  }

  void getTemperatureAndDataTime() {
    transmitter("${RootPacked.general.index} ${General.dataTime.index} ");
    transmitter("${RootPacked.general.index} ${General.rtcTemperature.index} ");
  }

  void rxTemperature(CommandMsg msg) {
    double temperature = double.parse(msg.split[3]);
    state = state.copyWith(temperature: temperature);
  }

  void rxDataTime(CommandMsg msg) {
    DateTime rxDataTime = DateTime(
      int.parse(msg.split[RxDataTime.year.index]),
      int.parse(msg.split[RxDataTime.month.index]),
      int.parse(msg.split[RxDataTime.day.index]),
      int.parse(msg.split[RxDataTime.hour.index]),
      int.parse(msg.split[RxDataTime.minut.index]),
      int.parse(msg.split[RxDataTime.second.index]),
    );
    state = state.copyWith(dataTime: rxDataTime);
  }
}
