import 'package:dixom/driver/device_rx_tx/command_msg.dart';
import 'package:dixom/driver/device_rx_tx/transmiter.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'radio_model.dart';

final fmRadioPrdr = StateNotifierProvider<FmRadioModelNotifier, FmRadioModel>((ref) => FmRadioModelNotifier());

class FmRadioModelNotifier extends StateNotifier<FmRadioModel> {
  FmRadioModelNotifier() : super(const FmRadioModel(presetList: []));

  void setFreqency(double value) {
    if (!checkEquality(value, state.freqency)) {
      state = state.copyWith(freqency: (value * 10).round() / 10);
      transmitter("7 5=${state.freqency}");
    }
  }

  void getFreqency(double value) {
    state = state.copyWith(freqency: value);
  }

  bool checkEquality(double num1, double num2) {
    // Округляем числа до сотых
    double roundedNum1 = (num1 * 10).round() / 10;
    double roundedNum2 = (num2 * 10).round() / 10;
    // Сравниваем округленные числа
    return roundedNum1 == roundedNum2;
  }

  void getServiceInfo(CommandMsg msg) {
    state = state.copyWith(
      rssi: int.parse(msg.split[3]),
      snr: int.parse(msg.split[4]),
      mult: int.parse(msg.split[5]),
      stereo: int.parse(msg.split[6]),
      stereoBlend: int.parse(msg.split[7]),
      freqOff: int.parse(msg.split[8]),
    );
  }

  void radioManualSearchUp() {
    transmitter("7 8=1");
  }

  void radioManualSearchDown() {
    transmitter("7 8=0");
  }

  void radioAutoSearchUp() {
    transmitter("7 7=1");
  }

  void radioAutoSearchDown() {
    transmitter("7 7=0");
  }

  void btUpdate() {}
}
