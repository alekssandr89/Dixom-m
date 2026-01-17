import 'package:dixom/dataBase/app_constants.dart';
import 'package:dixom/driver/device_rx_tx/command_msg.dart';
import 'package:dixom/dataBase/device_constant.dart';
import 'package:dixom/driver/device_rx_tx/transmiter.dart';
import 'package:dixom/pages/top_bar/log/backend/device_log.dart';
import 'package:dixom/pages/top_bar/lower_menu.dart';
import 'package:dixom/components/utilities/responsive.dart';
import 'package:flutter/material.dart';

import '../../../../driver/device_rx_tx/exchange_param.dart';

class PlatformLogView extends StatefulWidget {
  const PlatformLogView({
    super.key,
  });

  @override
  State<PlatformLogView> createState() => _PlatformLogViewState();
}

class _PlatformLogViewState extends State<PlatformLogView> {
  List<ExchangeDetailLogModel> detailLog = [];
  List<ExchangeTotalLogModel> totalLog = [totalLogDefault()];

  @override
  void initState() {
    super.initState();
    getInfo();
  }

  bool getState = true;

  void getInfo() async {
    getState = false;

    final List<ExchangeDetailLogModel> newDetailLog = await deviceResponse<ExchangeDetailLogModel>(
      request: deatiledLogCmd(),
      function: deatiledLogFilter,
    );

    final List<ExchangeTotalLogModel> newTotalLog = await deviceResponse<ExchangeTotalLogModel>(
      standard: totalLogDefault(),
      request: totalLogCmd(),
      function: totalLogFilter,
    );

    setState(() {
      totalLog = newTotalLog;
      detailLog = newDetailLog;
    });
    getState = true;
  }

  @override
  Widget build(BuildContext context) {
    return Container(
      padding: const EdgeInsets.all(AppIndents.pading),
      decoration: const BoxDecoration(
        color: AppColor.secondary,
      ),
      child: Column(
        children: [
          Row(
            mainAxisAlignment: MainAxisAlignment.spaceAround,
            children: [
              ElevatedButton(
                  onPressed: () {
                    if (getState == true) {
                      getInfo();
                    }
                  },
                  child: const Text("Получить")),
              if (totalLog[0].presOfError == "0")
                const Text(
                  "Ошибок нет",
                  style: TextStyle(fontSize: 18, color: Colors.green),
                ),
              if (totalLog[0].presOfError == "1")
                const Text(
                  "Есть ошибки",
                  style: TextStyle(fontSize: 18, color: Colors.red),
                ),
              Text(
                "Интерфейсы ${totalLog[0].interfaceCnt}",
                style: const TextStyle(fontSize: 18),
              ),
              if (!Responsive.isMobile(context))
                Text(
                  "Ошибки ${totalLog[0].error}",
                  style: const TextStyle(fontSize: 18),
                ),
              if (!Responsive.isMobile(context))
                Text(
                  "Успешно ${totalLog[0].ok}",
                  style: const TextStyle(fontSize: 18),
                ),
            ],
          ),
          const SizedBox(
            height: 16,
          ),
          Expanded(
            child: SingleChildScrollView(
              child: Column(
                children: [
                  Table(
                    border:
                        TableBorder.all(color: Colors.grey, borderRadius: const BorderRadius.all(Radius.circular(10))),
                    columnWidths: const {
                      0: FixedColumnWidth(50),
                      1: FlexColumnWidth(3),
                    },
                    children: [
                      TableRow(decoration: const BoxDecoration(), children: [
                        statusTableTextHeader("Код"),
                        statusTableTextHeader("Описание"),
                        statusTableTextHeader("Ошибки"),
                        statusTableTextHeader("Занят"),
                        statusTableTextHeader("Таймаут"),
                        statusTableTextHeader("Удачно"),
                        statusTableTextHeader("Пропуск"),
                      ]),
                      for (final item in detailLog)
                        TableRow(
                          children: [
                            statusTableTextParam(item.code),
                            statusTableTextParam(item.description),
                            statusTableTextParam(item.error),
                            statusTableTextParam(item.busy),
                            statusTableTextParam(item.timeout),
                            statusTableTextParam(item.ok),
                            statusTableTextParam(item.queueTimeout),
                          ],
                        ),
                    ],
                  ),
                ],
              ),
            ),
          ),
        ],
      ),
    );
  }
}

String totalLogCmd() {
  return "${RootPacked.general.index} ${General.log.index} ${ExchangeLog.totalInfo.index}";
}

ExchangeTotalLogModel totalLogDefault() {
  return ExchangeTotalLogModel(
    presOfError: "0",
    interfaceCnt: "0",
    error: "0",
    busy: "0",
    timeout: "0",
    ok: "0",
    queueTimeout: "0",
    noConnection: "0",
  );
}

WorkState totalLogFilter(CommandMsg msg, List<ExchangeTotalLogModel> response) {
  //Если пакет относиться к general и параметр log
  if (msg.packed == RootPacked.general.index && msg.parametr == General.log.index) {
    //Если подараметр подробная информация то ложим данные в список
    try {
      if (msg.podParmetr == ExchangeLog.totalInfo.index) {
        response[0] = ExchangeTotalLogModel(
          presOfError: msg.split[TotalLog.presOfError.index],
          interfaceCnt: msg.split[TotalLog.interfaceCnt.index],
          error: msg.split[TotalLog.error.index],
          busy: (msg.split[TotalLog.busy.index]),
          timeout: msg.split[TotalLog.timeout.index],
          ok: msg.split[TotalLog.ok.index],
          queueTimeout: msg.split[TotalLog.queueTimeOut.index],
          noConnection: msg.split[TotalLog.moConnection.index],
        );
        return WorkState.complete; //прекращаем ожидание
      }
    } catch (e) {}
  }
  return WorkState.notComplete;
}

String deatiledLogCmd() {
  return "${RootPacked.general.index} ${General.log.index} ${ExchangeLog.detailInfo.index}";
}

WorkState deatiledLogFilter(CommandMsg msg, List<ExchangeDetailLogModel> response) {
  //Если пакет относиться к general и параметр log
  if (msg.packed == RootPacked.general.index && msg.parametr == General.log.index) {
    //Если подараметр подробная информация то ложим данные в список

    try {
      if (msg.podParmetr == ExchangeLog.detailInfo.index) {
        ExchangeDetailLogModel interface = ExchangeDetailLogModel(
          code: msg.split[DetailLog.cod.index],
          description: ExchnageLogCode.values[int.parse(msg.split[DetailLog.cod.index])].decoding,
          error: msg.split[DetailLog.error.index],
          busy: (msg.split[DetailLog.busy.index]),
          timeout: msg.split[DetailLog.timeout.index],
          ok: msg.split[DetailLog.ok.index],
          queueTimeout: msg.split[DetailLog.queueTimeOut.index],
          noConnection: msg.split[DetailLog.moConnection.index],
        );
        response.add(interface);
        return WorkState.notComplete;
        //Если подпараметр конец выгрузки лога завершаем заполнение
      } else if (msg.podParmetr == ExchangeLog.done.index) {
        return WorkState.complete;
      }
    } catch (e) {
      print("");
    }
  }
  return WorkState.notComplete;
}

class ExchangeTotalLogModel {
  final String presOfError; //Наличие ошибок хотябы в одном интерфейсе
  final String interfaceCnt; //Количество активных интерфейсов
  final String error; // Общее количество обмена с ошибкой
  final String busy; // Общее количество занятых интерфейсов
  final String timeout; // общее количество вышедших таймаутов
  final String ok; // общее количество успешного обмена
  final String queueTimeout; // Общее количество таймаута очереди
  final String noConnection; //общее количество из за потери связи

  ExchangeTotalLogModel({
    required this.presOfError,
    required this.interfaceCnt,
    required this.error,
    required this.busy,
    required this.timeout,
    required this.ok,
    required this.queueTimeout,
    required this.noConnection,
  });
}

class ExchangeDetailLogModel {
  final String code; // код интерфеса ExchnageLogCode
  final String description; // Описание ошибки
  final String error; // Количество ошибок при обмене
  final String busy; // количество был занят
  final String timeout; //количество вышло время ожидания
  final String ok; //количество успешных обменов
  final String queueTimeout; //количество пропущенных очередей
  final String noConnection; // количество ошибок из за разрыва связи

  ExchangeDetailLogModel({
    required this.code,
    required this.description,
    required this.error,
    required this.busy,
    required this.timeout,
    required this.ok,
    required this.queueTimeout,
    required this.noConnection,
  });
}

enum ExchnageLogCode {
  empty("Пустой"), //0
  dacWrite("Запись в ЦАП"), //1
  dacRead("Чтение из ЦАП"), //2
  dspWrite("Запись в DSP"), //3
  dspRead("Чтение из DSP"), //4
  eepromWrite("Запись в EEPROM"), //5
  eepromRead("Чтение из EEPROM"), //6
  flashWrite("Звись в SPI FLASH"), //7
  flashRead("Чтение из SPI FLASH"), //8
  i2c2Write("Запись в I2C2"), //9
  i2c2Read("Чтение из I2C2"), //10
  spi2Write("Запись в SPI2"), //11
  spi2Read("Чтение из SPI2"), //12
  radioWrite("Запись в ФМ радио"), //13
  radioRead("Чтение из ФМ радио"), //14
  i2c1Write("Запись в I2C1"), //15
  i2c1Read("Чтение из I2C1"), //16
  usbSoundWrite("Запись в USB звуковую карту"), //17
  usbSoundRead("Чтение из USB звуковой карты"), //18
  displayLcdWrite("Запись в LCD дисплей"), //19
  displayOledWrite("Запись в OLED дисплей"), //20
  rtcWrite("Запись в RTC"), //21
  rtcRead("Чтение из RTC"), //22
  usbHidWrite("Запись в USB HID"), //23
  usbCdcWrite("Чтение из USB CDC"), //24
  uart1BluetoothRead("Чтение из UART1 Bluetooth"), //25
  uart1BluetoorhWrite("Запись в UART1 Bluetooth"), //26
  uart2RfRead("Чтение из UART2"), //27
  uart2RfWrite("Запись в UART2"), //28
  uart3ArduinoRead("Чтение из UART3 Arduino"), //29
  uart3ArduinoWrite("Запись в UART3 Arduino"), //30
  uart5NextionRead("Чтение из UART5 Nextion"), //31
  uart5NextionWrite("Запись в UART5 Nextion"), //32
  miniDspWrite("Запись в mini DSP"), //33
  miniDspRead("Чтение из mini DSP"), //34
  stmFlashLock("Разблокировка Flash Stm32"), //35
  stmFlashUnlock("Блокировка Flash Stm32"), //36
  stmFlshErase("Очистка Flash Stm32"), //37
  stmFlashWrite("Запись в Flash Stm32"), //39
  stmFlashRead("Чтение из Flash Stm32"), //38
  canSend("Отправка в CAN шину"), //40
  canRead("Чтение из CAN шины"); //41

  final String decoding;
  const ExchnageLogCode(this.decoding);
}
