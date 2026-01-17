import 'package:dixom/driver/device_rx_tx/command_msg.dart';
import 'package:dixom/driver/device_rx_tx/transmiter.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

import 'button_model.dart';
import 'button_reciever.dart';

final externalControlPrdr =
    StateNotifierProvider<ExternalControlModelNotifier, ExternalControlModel>((ref) => ExternalControlModelNotifier());

class ExternalControlModelNotifier extends StateNotifier<ExternalControlModel> {
  ExternalControlModelNotifier() : super(const ExternalControlModel());

  void getButton(CommandMsg msg) {
    // oneButton.add(value)
    btUpdate();
  }

  void deleteButton(int index, OneButton button) {
    //для удаления кноки нужно отправить в выбранное ID действие 0
    String cmd = "6 3=${button.channel} ${button.id} 0 0 0 0 0 ";
    transmitter(cmd);
    buttonList.removeAt(index);
    btUpdate();
  }

  void setButton(int index, OneButton button) {
    buttonList[index] = button;
    String cmd =
        "6 3=${button.channel} ${button.id} ${button.interval} ${button.action} ${button.delayAction} ${button.doublAction} ${button.delayTiming} ${button.dobleTiming} ${button.action2} ${button.delayAtion2} ${button.doubleAction2} ${button.param1} ${button.param2} ${button.param3} ${button.param3}";
    transmitter(cmd);
    btUpdate();
  }

  void btUpdate() {
    int btUpdate = state.btUpdate;
    btUpdate++;
    state = state.copyWith(
      btUpdate: btUpdate,
    );
  }

  void updateModel(ExternalControlModel model) {
    state = state.copyWith();
  }
}
