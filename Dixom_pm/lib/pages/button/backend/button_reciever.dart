import 'package:dixom/main.dart';
import '../../../driver/device_rx_tx/command_msg.dart';
import 'button_const.dart';
import 'button_model.dart';
import 'button_provider.dart';

List<OneButton> buttonList = [];

class ExternalControl {
  void setDefault() {
    buttonList.clear();
    appRef.read(externalControlPrdr.notifier).btUpdate();
  }

  void receiver(CommandMsg msg) {
    if (msg.parametr == ExternalControlPacked.message.index) {
    } else if (msg.parametr == ExternalControlPacked.notification.index) {
    } else if (msg.parametr == ExternalControlPacked.button.index) {
      /*
      6 3 4 0 150 104 107 0 500 350 0 0 0 0 0 0 0
      | | | |  |   |   |  |  |   |  | | | | | | +----16 Параметр 4
      | | | |  |   |   |  |  |   |  | | | | | +------15 Параметр 3
      | | | |  |   |   |  |  |   |  | | | | +--------14 Параметр 2
      | | | |  |   |   |  |  |   |  | | | +----------13 Параметр 1
      | | | |  |   |   |  |  |   |  | | +------------12 Действие при удерживании 2
      | | | |  |   |   |  |  |   |  | +--------------11 Действие при двойном клике 2
      | | | |  |   |   |  |  |   |  +----------------10 Действие при клике 2
      | | | |  |   |   |  |  |   +-------------------9 Таймер для двойного клика
      | | | |  |   |   |  |  +-----------------------8  Таймер для удерживания
      | | | |  |   |   |  +--------------------------7  Действие при удерживании
      | | | |  |   |   +-----------------------------6  Действие при двойном клике
      | | | |  |   +---------------------------------5  Действие при клике
      | | | |  +-------------------------------------4  Интервал (погрешность АЦП)
      | | | +----------------------------------------3  Значение ADC 10 бит (ID кнопки) 
      | | +------------------------------------------2  Канал
      | +--------------------------------------------1  *Параметр button 
      +----------------------------------------------0  *Пакет External Control
       */
      parceOneButton(msg);
    } else if (msg.parametr == ExternalControlPacked.newButton.index) {
      /*
      6 4 0 150
      | | | +----3  Значение ADC 10 бит (ID кнопки) 
      | | +------2  Канал
      | +--------1  *Параметр button 
      +----------0  *Пакет External Control
       */
      parceNewButton(msg);
    } else if (msg.parametr == ExternalControlPacked.btnName.index) {}
  }
}

void parceOneButton(CommandMsg msg) {
  OneButton button = OneButton(
    channel: int.parse(msg.split[ParceOneButton.channel.index]),
    id: int.parse(msg.split[ParceOneButton.id.index]),
    interval: int.parse(msg.split[ParceOneButton.interval.index]),
    action: int.parse(msg.split[ParceOneButton.action.index]),
    delayAction: int.parse(msg.split[ParceOneButton.delayAction.index]),
    doublAction: int.parse(msg.split[ParceOneButton.doublAction.index]),
    delayTiming: int.parse(msg.split[ParceOneButton.delayTiming.index]),
    dobleTiming: int.parse(msg.split[ParceOneButton.dobleTiming.index]),
    action2: int.parse(msg.split[ParceOneButton.action2.index]),
    delayAtion2: int.parse(msg.split[ParceOneButton.delayAtion2.index]),
    doubleAction2: int.parse(msg.split[ParceOneButton.doubleAction2.index]),
    param1: int.parse(msg.split[ParceOneButton.param1.index]),
    param2: int.parse(msg.split[ParceOneButton.param2.index]),
    param3: int.parse(msg.split[ParceOneButton.param3.index]),
    param4: int.parse(msg.split[ParceOneButton.param4.index]),
  );

  bool addStatus = true;
  for (int i = 0; i < buttonList.length; i++) {
    if (buttonList[i].channel == button.channel && buttonList[i].id == button.id) {
      addStatus = false;
    }
  }
  if (addStatus == true || buttonList.isEmpty) {
    //Добавление кнопки
    buttonList.add(button);
    appRef.read(externalControlPrdr.notifier).btUpdate();
  }
}

void parceNewButton(CommandMsg msg) {
  OneButton button = OneButton(
    channel: int.parse(msg.split[ParceOneButton.channel.index]),
    id: int.parse(msg.split[ParceOneButton.id.index]),
    interval: 0,
    action: 0,
    delayAction: 0,
    doublAction: 0,
    delayTiming: 500,
    dobleTiming: 350,
    action2: 0,
    delayAtion2: 0,
    doubleAction2: 0,
    param1: 0,
    param2: 0,
    param3: 0,
    param4: 0,
  );
  bool addStatus = true;
  for (int i = 0; i < buttonList.length; i++) {
    if (buttonList[i].channel == button.channel &&
        (buttonList[i].id - 15 < button.id && buttonList[i].id + 15 > button.id)) {
      addStatus = false;
      buttonList[i] = button;
    }
  }
  if (addStatus == true || buttonList.isEmpty) {
    //Добавление кнопки
    buttonList.add(button);
  }
  appRef.read(externalControlPrdr.notifier).btUpdate();
}
