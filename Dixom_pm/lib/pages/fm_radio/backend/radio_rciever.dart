import 'package:dixom/driver/device_rx_tx/command_msg.dart';
import 'package:dixom/main.dart';
import 'radio_const.dart';
import 'radio_provider.dart';

class FmRadio {
  void setDefault() {}

  void receiver(CommandMsg msg) {
    if (msg.parametr == FmRadioPacked.message.index) {
      if (msg.parametr == FmRadioMessage.serviceInfo.index) {
        appRef.read(fmRadioPrdr.notifier).getServiceInfo(msg);
        /*
        7 0 0 0 0 0 0 0 0
        | | | | | | | | +--8  FREQOFF
        | | | | | | | +----7  STEREO_BLEND
        | | | | | | +------6  STEREO
        | | | | | +--------5  MULT
        | | | | +----------4  SNR
        | | | +------------3  RSSI
        | | +--------------2  *Подпараметр Сервисное сообщение
        | +----------------1  *Параметр Сообщение
        +------------------0  *Пакет FM Radio
       */
      }
    } else if (msg.parametr == FmRadioPacked.settings.index) {
      /*
        7 2 1 10 0 1 0 0 0
        | | | |  | | | | +--9  RDS
        | | | |  | | | +----8  TA
        | | | |  | | +------7  PTY
        | | | |  | +--------6  Band
        | | | |  +----------5  STEREO
        | | | +-------------4  AutoData
        | | +---------------2  Spasing 10 5
        | +-----------------1  *Параметр настройки
        +-------------------0  *Пакет FM Radio
       */
    } else if (msg.parametr == FmRadioPacked.currentPreset.index) {
      /*
      7 3 0 106.40
      | | |  +-----3  Частота FM волны
      | | +--------2   Текущий пресет
      | +----------1  *Параметр текущий пресет 
      +------------0  *Пакет FM Radio
       */
    } else if (msg.parametr == FmRadioPacked.listPreset.index) {
      /*
      7 4 0 0.00
      | | |  +-----3  Частота FM волны пресета
      | | +--------2   Номер ячейки пресета от 1 до 10.
      | +----------1  *Параметр пресетов
      +------------0  *Пакет FM Radio
       */
      //radioPresetList
    } else if (msg.parametr == FmRadioPacked.frequnci.index) {
      /*
      7 5 106.40
      | |   +------3  Текущая частота FM волны
      | +----------1  *Параметр текущая частота
      +------------0  *Пакет FM Radio
       */
      appRef.read(fmRadioPrdr.notifier).getFreqency(double.parse(msg.split[2]));
    } else if (msg.parametr == FmRadioPacked.chip.index) {
      /*
      7 6 2 1
      | | |  +-----3  Тип волны AM или FM
      | | +--------2  Тип микросхемы радио модуля
      | +----------1  *Параметр тип микрочипа
      +------------0  *Пакет FM Radio
       */
    }
  }
}
