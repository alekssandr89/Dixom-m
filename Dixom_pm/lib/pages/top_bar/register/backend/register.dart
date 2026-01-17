import 'package:dixom/driver/device_rx_tx/command_msg.dart';
import 'package:dixom/main.dart';

///Основные пакеты команд аудиоплатформы
enum Register {
  register("Чтение или запись в регистра"), //0
  setRegusterSucces("Регистер успешно выставлен"), // 1
  chipNotSupproted("Микросхема не поддерживается"), //2
  chipNotFound("Микросхема не найдера"); //2

  final String decoding;
  const Register(this.decoding);
}

/*
 * Подпараметры отмеченные ********** не имеют возможности записи, 
 * они необходимы исключительно в качестве уведомителей,  
 */

class RxRegister {
  void receiver(CommandMsg msg) {
    /*
     * сообщение придёт в список List<String> msg.split
     */
    if (msg.podParmetr == Register.register.index) {
      /*
       * Возвращает одно сообщение содержащее информацию о адресе и значении
       *
       * 1 9 0 0 0 'Register Control'
       * | | | | |         +------------ 4 String Просто информационное сообщение
       * | | | | +---------------------- 3 0 Значение регистра
       * | | | +------------------------ 3 0 Адрес регистра
       * | | +-------------------------- 2 0 Чтение или запись в регистра
       * | +---------------------------- 1 9 Параметр register
       * +------------------------------ 0 1 Пакет general      
       */
    } else if (msg.podParmetr == Register.setRegusterSucces.index) {
      /*
      * Уведомление, значение успешно записано в регистр
      *
      * 1 9 1 'Register set successfully'
      * | | |           +--------------- 4 String Просто информационное сообщение
      * | | **************************** 2 1 Регистер успешно выставлен
      * | +----------------------------- 1 9 Параметр register
      * +------------------------------- 0 1 Пакет general    
      */
    } else if (msg.podParmetr == Register.chipNotSupproted.index) {
      /*
      * Уведомление, в данный прошивке микросхема не поддерживается
      *
      * 1 9 2 'Сhip not supported'
      * | | |           +--------------- 4 String Просто информационное сообщение
      * | | **************************** 2 2 Микросхема не поддерживается
      * | +----------------------------- 1 9 Параметр register
      * +------------------------------- 0 1 Пакет general    
      */
    } else if (msg.podParmetr == Register.chipNotFound.index) {
      /*
      * Уведомление, микросхема на шине данныъх не найдена
      *
      * 1 9 3 'Chip not found on data bus'
      * | | |           +--------------- 4 String Просто информационное сообщение
      * | | **************************** 2 3 Микросхема не найдера
      * | +----------------------------- 1 9 Параметр register
      * +------------------------------- 0 1 Пакет general    
      */
      appRef.read(platformNotification.notifier).showNotification("Выбранная микросхема не найдена");
    }
  }
}

enum SetRegister {
  packed("Основной"), //0
  param("Журнал"), //1
  podParam("Общая информация"), //2
  addres("Адрес регистра"), //3
  value("Значение регистра"); //4

  final String decoding;
  const SetRegister(this.decoding);
}
