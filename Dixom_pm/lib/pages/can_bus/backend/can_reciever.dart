import 'package:dixom/driver/device_rx_tx/command_msg.dart';
import 'package:dixom/dataBase/device_constant.dart';
import 'package:dixom/main.dart';
import 'can_const.dart';
import 'can_model.dart';
import 'can_provider.dart';

enum ParceCanState {
  packed, // 0  Пакет External Control
  param, // 1 *Параметр button
  state, // 2
  framesInPacked, // 3
  speed, // 4
  txPermision, // 5
  autoConnect, // 6
  dataOutPermision; // 7
}

class CanBus {
  void setDefault() {}

  void receiver(CommandMsg msg) {
    if (msg.parametr == CanBusPacked.message.index) {
      /*
      3 2 0 data
      | | |   +----------3 данные сообщения
      | | +--------------2 message parametr
      | +----------------1 параметр message
      +------------------0 пакет can
       */

      if (msg.podParmetr == CanMessageParam.frame.index) {
        canFrameParce(msg);
      }
    } else if (msg.parametr == CanBusPacked.settings.index) {
      /*
      3 2 0 0 
      | | | +------------3 data 
      | | +--------------2 settings parametr
      | +----------------1 параметр settings
      +------------------0 пакет can
       */
      canSettings(msg);
    } else if (msg.parametr == CanBusPacked.status.index) {
      /*
      3 3 1 7 3 1 0 0 0 0 0
      | | | | | | | | | | +----10 Reserved
      | | | | | | | | | +------9 Reserved
      | | | | | | | | +--------8 Reserved
      | | | | | | | +----------7 CanDataOutPermision
      | | | | | | +------------6 CanAutoConnect
      | | | | | +--------------5 CanTxPermision
      | | | | +----------------4 CanSpeed
      | | | +------------------3 CanMsgCount
      | | +--------------------2 Status
      | +----------------------1 параметр status
      +------------------------0 пакет can
       */
      canState(msg);
    } else if (msg.parametr == CanBusPacked.speed.index) {
      /*
      3 4 3
      | | +--------------2 speed 
      | +----------------1 параметр speed
      +------------------0 пакет can
       */
    } else if (msg.parametr == CanBusPacked.filter.index) {
      /*
      3 5 0 1 0 0 0 0 0 0 0 0
      | | | | | | | | | | | +--10 MaskIdLow
      | | | | | | | | | | +----9 MaskIdHigh
      | | | | | | | | | +------8 IdLow
      | | | | | | | | +--------7 IdHigh
      | | | | | | |+-----------6 IdType
      | | | | | +--------------5 Scale
      | | | | +----------------4 Mode
      | | | +------------------3 Activation
      | | +--------------------2 FilterNum
      | +----------------------1 параметр filter
      +------------------------0 пакет can
      */
    } else if (msg.parametr == CanBusPacked.car.index) {
      /*
      3 6 0 0 0 0 0 0 0
      | | | | | | | | +--------7 HandlebarSide Сторона руля
      | | | | | | | +----------4 DriveUnit Привод
      | | | | | | +------------6 Transmission Трансмиссия
      | | | | | +--------------5 Year Год 
      | | | | +----------------4 Generation Поколение
      | | | +------------------3 Model Модель
      | | +--------------------2 Manufacturer Производитель
      | +----------------------1 параметр car
      +------------------------0 пакет can
      */
      canCar(msg);
    }
  }
}

List<CanFrame> canMessageList = [];
Map<int, CanFrame> canMessageMap = {};

void canFrameParce(CommandMsg msg) {
  /**
   * В C струкутра dataInfo выгляди так.
   * struct dataInfo
    {
    	uint8_t         idType      :1;
    	uint8_t         Dlc         :4;
    	uint8_t         MsgCount    :3;
    }; 

   * Данные в приходят в одном пакете в байтовом формате до 63 байт
   * Первые 6 байт это "3 0 0 "
   * Далее 1 байт это стурктура dataInfo  которая содержит данные:
   *    1) idType - Тип сообщения, обычный 2 байта или расширенный 4 байта
   *    2) Dlc - Количество байт в кадре
   *    3) MsgCount - количество кадров в одном пакете, может быть до 7 кадров
   * Далее данные в байтах в указанном количестве Dlc, минимум 1 байт максимум 8
   * 
   * В общем пакет выгляди следующим образом   
   * 3 0 0 dataInfo byteBuff dataInfo byteBuff dataInfo byteBuff dataInfo byteBuff
   */
  int byteProcessed = 6;
  //Количество кадров в пакете
  int msgCount = msg.buffer[byteProcessed] >> 5 & 0x07; //00000111

  for (int i = 0; i < msgCount; i++) {
    CanIdType idType = msg.buffer[byteProcessed] & 0x01 == 0 //00000001
        ? CanIdType.standart
        : CanIdType.extended;
    int dlc = msg.buffer[byteProcessed] >> 1 & 0x0F; //00001111
    int id;
    byteProcessed++;

    if (idType == CanIdType.standart) {
      id = msg.buffer[byteProcessed] | msg.buffer[byteProcessed + 1] << 8;
      byteProcessed = byteProcessed + 2;
    } else {
      id = msg.buffer[byteProcessed] |
          msg.buffer[byteProcessed + 1] << 8 |
          msg.buffer[byteProcessed + 2] << 16 |
          msg.buffer[byteProcessed + 3] << 24;
      byteProcessed = byteProcessed + 4;
    }

    CanFrame message = CanFrame(
      id: id,
      dlc: dlc,
      framesCount: 0,
      idType: idType,
      period: const Duration(),
      buffer: msg.buffer.sublist(byteProcessed, byteProcessed + dlc),
      time: DateTime.now(),
    );

    if (canMessageMap.containsKey(id)) {
      message.framesCount = canMessageMap[id]!.framesCount + 1;
      message.period = DateTime.now().difference(canMessageMap[id]!.time);
    }
    canMessageMap[id] = message;
    appRef.read(canFramePrdr.notifier).frameUpdate();

    byteProcessed = byteProcessed + dlc;
  }
}

void canState(CommandMsg msg) {
  int frames = int.parse(msg.split[ParceCanState.framesInPacked.index]) - 1;
  if (frames < 0) frames = 0;
  appRef.read(canStatePrdr.notifier).copyWith(
        status: WorkState.values.elementAt(int.parse(msg.split[ParceCanState.state.index])),
        speed: int.parse(msg.split[ParceCanState.speed.index]),
        autoConnect: msg.split[ParceCanState.autoConnect.index] == "1" ? true : false,
        dataOutPermision: msg.split[ParceCanState.dataOutPermision.index] == "1" ? true : false,
        txPermision: msg.split[ParceCanState.txPermision.index] == "1" ? true : false,
        framesInPacked: frames,
      );
}

void canSettings(CommandMsg msg) {
  if (msg.podParmetr == CanSettings.autoConnect.index) {
  } else if (msg.podParmetr == CanSettings.autoConnect.index) {
  } else if (msg.podParmetr == CanSettings.autoConnect.index) {}
}

void canCar(CommandMsg msg) {
  /*
      3 6 0 0 0 0 0 0 0
      | | | | | | | | +--------7 HandlebarSide Сторона руля
      | | | | | | | +----------4 DriveUnit Привод
      | | | | | | +------------6 Transmission Трансмиссия
      | | | | | +--------------5 Year Год 
      | | | | +----------------4 Generation Поколение
      | | | +------------------3 Model Модель
      | | +--------------------2 Manufacturer Производитель
      | +----------------------1 параметр car
      +------------------------0 пакет can
      */
  appRef.read(canStatePrdr.notifier).copyWith(
        manufacturer: int.parse(msg.split[CanCarParram.manufacturer.index]),
        model: int.parse(msg.split[CanCarParram.model.index]),
        generation: int.parse(msg.split[CanCarParram.generation.index]),
      );
}
