import 'package:dixom/driver/device_rx_tx/command_msg.dart';
import 'package:dixom/main.dart';

import 'settings_const.dart';
import 'settings_provider.dart';

class GeneralSettings {
  void setDefault() {
    appRef.read(senttingsPrdr.notifier).setDefault();
  }

  void receiver(CommandMsg msg) {
    /*
      1 2 0 0
      | | | +------------3 settings data
      | | +--------------2 settings parametr
      | +----------------1 параметр settings
      +------------------0 пакет general
    */
    appRef.read(senttingsPrdr.notifier).setSettins(msg.podParmetr, int.parse(msg.split[RxSettings.data.index]));
  }
}

class Rtc {
  void setDefault() {
    appRef.read(senttingsPrdr.notifier).setDefault();
  }

  void receiver(CommandMsg msg) {
    if (msg.podParmetr == RtcPodparam.dataTime.index) {
      /*
      1 11 0 2 54 23 1 1 1 2020 'Data Time'
      |  | | |  |  | | | |   |       +------10 вспомогательная приписка
      |  | | |  |  | | | |   +---------------9 год
      |  | | |  |  | | | +-------------------8 месяц
      |  | | |  |  | | +---------------------7 дата
      |  | | |  |  | +-----------------------6 день недели
      |  | | |  |  +-------------------------5 секунды
      |  | | |  +----------------------------4 минуты
      |  | | +-------------------------------3 часы
      |  | +---------------------------------2 *подпараметр дата и время
      |  +------------------------------------1 *параметр settings
      +--------------------------------------0 *пакет general
      */
      appRef.read(rtcPrdr.notifier).rxDataTime(msg);
    } else if (msg.podParmetr == RtcPodparam.temperature.index) {
      appRef.read(rtcPrdr.notifier).rxTemperature(msg);
    }
  }
}
