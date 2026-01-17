// ignore_for_file: public_member_api_docs, sort_constructors_first
import 'package:dixom/components/widgets/connection.dart';

enum RtcPodparam {
  dataTime,
  temperature,
}

enum RxDataTime {
  packed,
  paramert,
  podparametr,
  hour,
  minut,
  second,
  day,
  date,
  month,
  year,
}

enum RxSettings {
  packed,
  settings,
  paramert,
  data,
}

final List<int> defaultGenralSettings = [
  1, // 0   Status Play on Start dixom
  0, // 1   Status Play x2 on Start dixom
  1, // 2   Status stop music power down: 1 on 0 off
  0, // 3   Command stop: 0 Stop 1 Pause
  1, // 4   Status sleep on power down platform: 1 on 0 off
  1, // 5   Command sleep: 0 PowerDown 1 Sleep
  1, // 6   Status Don't Sleep
  3, // 7   Command Don't Sleep
  0, // 8   Status Vbus on input usb
  0, // 9   Mode on power usb1
  3, // 10  Mode on power usb2
  0, // 11  RearSignal 0 Analog 1 Can
  0, // 12  Alt tab camera off
  0, // 13  Play music camera OFF
  2, // 14  Rear Camera On Volume Default
  20, // 15  Rear Camera minimum volume
  1, // 16  Usb sound card state
  0, // 17  Status Automatic sound card polling
  0, // 18  Platform language
  0, // 19 TriggerOnHolding
  0, // 20
  0, // 21 TriggerOnHolding
  0, // 22
  1, // 23 TriggerOnOffNoSave
  0, // 24
  0, // 25  Type Text LCD
  0, // 26  Type Oled LCD
  0, // 27  Type Nextion LCD
  0, // 28  Status Nextion LCD
  16, // 29 MaxBrightness
  0, // 30
  110, // 31  remot voltage level
  0, // 32  volume up usb connect
  1, // 33  Current select interface mode
  0, // 34  Setting_manual_tx_usb_hid
  0, // 35  Setting_manual_tx_usb_cdc
  0, // 36  Setting_manual_tx_usb_cdc
];
final List<int> defaultGenralTimer = [
  0,
  7000,
  120000,
  400,
  400,
  2000,
  2000,
  1000,
  2000,
];

class Parameter {
  String title;
  String description;
  String measure;
  int data;
  int dataMin;
  int dataMax;
  int delimiter;
  SettingsType type;
  List<String> param;
  Parameter({
    required this.title,
    required this.description,
    required this.data,
    required this.type,
    required this.param,
    this.dataMin = 0,
    this.dataMax = 60,
    this.measure = "",
    this.delimiter = 0,
  });
}

List<Parameter> generalTimer = [
  // 0
  Parameter(
    title: "Нжаимать Play при включении",
    description:
        'После зауска платформы будет автоматичческий будет отправлена HID команда play для начала воспроизведения мультимедия',
    data: defaultGenralTimer[0],
    type: SettingsType.slider,
    param: [],
  ),
];

List<Parameter> generalSettings = [
  // 0
  Parameter(
    title: "Нжаимать Play при включении",
    description:
        'После зауска платформы будет автоматичческий будет отправлена HID команда play для начала воспроизведения мультимедия',
    data: defaultGenralSettings[0],
    type: SettingsType.togle,
    param: [],
  ),
  // 1
  Parameter(
    title: "Нжаимать Play при включении 2й раз",
    description: 'На некоторых планшетах отправки одного плей не достаточно, необходимо отправить 2-й раз',
    data: defaultGenralSettings[1],
    type: SettingsType.togle,
    param: [],
  ),
  // 2
  Parameter(
    title: "Остановка музыки",
    description: 'Перед отключением платформы, отправит HID команду stop для остановки возспроизведения мультимедия',
    data: defaultGenralSettings[2],
    type: SettingsType.togle,
    param: [],
  ),
  // 3
  Parameter(
    title: "Выбор команды stop",
    description: 'Тип оставновки музыки',
    data: defaultGenralSettings[3],
    type: SettingsType.list,
    param: ["Stop", "Pause"],
  ),
  // 4
  Parameter(
    title: "Сон планшета",
    description: 'При отключении платормы будет отправлена HID команда для отправки планшета в сон',
    data: defaultGenralSettings[4],
    type: SettingsType.togle,
    param: [],
  ),
  // 5
  Parameter(
    title: "Выбор команды сна планшета",
    description: 'Тип HID команды сон для отправки планшета в сон',
    data: defaultGenralSettings[6],
    type: SettingsType.list,
    param: ["PowerDown", "Sleep"],
  ),
  // 6
  Parameter(
    title: "Не давать планшету засыпать",
    description: 'Платформа будет через определённое время оправлять команду не спать, чтоб не потухал дисплей',
    data: defaultGenralSettings[6],
    type: SettingsType.togle,
    param: [],
  ),
  // 7
  Parameter(
    title: "Выбор команды не спать",
    description: 'Тип HID команды не спать',
    data: defaultGenralSettings[7],
    type: SettingsType.list,
    param: ["Клавиша F7", "Движение мыши", "Клавиша WakeUp", "Клавиша F8"],
  ),
  // 8
  Parameter(
    title: "Будить планшет зарядкой",
    description:
        'При включении платформа переведёт OTG режим в режим зарядки. И заустит планшет зарядкой. (Будет работать если используется 5ти проводный OTG провод, с обычным OTG проводом не работает)',
    data: defaultGenralSettings[8],
    type: SettingsType.togle,
    param: [],
  ),
  // 9
  Parameter(
    title: "Режим работы USB порта 1",
    description: 'Подача питания на USB порт',
    data: defaultGenralSettings[9],
    type: SettingsType.list,
    param: ["Отключен", "Включен", "Включение по таймеру", "Камера заднего хода"],
  ),
  // 10
  Parameter(
    title: "Режим работы USB порта 2",
    description: 'Подача питания на USB порт',
    data: defaultGenralSettings[10],
    type: SettingsType.list,
    param: ["Отключен", "Включен", "Включение по таймеру", "Камера заднего хода"],
  ),
  // 11
  Parameter(
    title: "Сигнал камеры заднего хода",
    description: 'Отпределяет, откуда брать сигнал камеры заднего хода',
    data: defaultGenralSettings[11],
    type: SettingsType.list,
    param: ["+12 на провод сигнала кзх", "CAN шина"],
  ),
  // 12
  Parameter(
    title: "Переключене приложения КЗХ",
    description:
        'Платформа нажмёт ALT + TAB для переключение приложения камезы заднего хода, после отключения сигнала КЗХ',
    data: defaultGenralSettings[12],
    type: SettingsType.togle,
    param: [],
  ),
  // 13
  Parameter(
    title: "Нажат Play после отключения КЗХ",
    description: 'На некоторых планшетах останавливается воспроизведение музыки после отключения камеры заднего хода',
    data: defaultGenralSettings[13],
    type: SettingsType.togle,
    param: [],
  ),
  // 14
  Parameter(
    title: "Делитель громкости при включении КЗХ",
    description: 'Делить текущую громкость на указанное чило для уменьшения громкости при движении назад',
    data: defaultGenralSettings[14],
    dataMin: 1,
    dataMax: 5,
    type: SettingsType.slider,
    param: [],
  ),
  // 15
  Parameter(
    title: "Минимальная громкость при КЗХ",
    description: 'Минимальная громкость, ниже которой не будет уменьшаться грмокость при движении назад',
    data: defaultGenralSettings[15],
    type: SettingsType.slider,
    param: [],
  ),
  // 16
  Parameter(
    title: "USB Звуковая карта",
    description: 'Управление состоянием USB Звуковой карты Dixom Hi-Res Audio',
    data: defaultGenralSettings[16],
    type: SettingsType.togle,
    param: [],
  ),
  // 17
  Parameter(
    title: "Опрос USB Звуковой карты",
    description: 'В реальном времени опрашивает состояние звуковой карты (не рекомендуется включать. Иногда одвисает)',
    data: defaultGenralSettings[17],
    type: SettingsType.togle,
    param: [],
  ),
  // 18
  Parameter(
    title: "Язык платформы",
    description: 'Внутренний язык платформы (сделано на будущее)',
    data: defaultGenralSettings[18],
    type: SettingsType.list,
    param: ["Английский"],
  ),
  // 19
  Parameter(
    title: "Режи работы триггера 1",
    description: 'Режим раоты тригера канал 1',
    data: defaultGenralSettings[19],
    type: SettingsType.list,
    param: ["Включить при удерживании", "переключать без сохранения", "переключать с сохранением"],
  ),
  // 20
  Parameter(
    title: "Состояние триггера 1",
    description: 'Отоброжает состояние триггера в реальном времени',
    data: defaultGenralSettings[20],
    type: SettingsType.togle,
    param: [],
  ),
  // 21
  Parameter(
    title: "Режи работы триггера 2",
    description: 'Режим раоты тригера канал 2',
    data: defaultGenralSettings[21],
    type: SettingsType.list,
    param: ["Включить при удерживании", "переключать без сохранения", "переключать с сохранением"],
  ),
  // 22
  Parameter(
    title: "Состояние триггера 2",
    description: 'Отоброжает состояние триггера в реальном времени',
    data: defaultGenralSettings[22],
    type: SettingsType.togle,
    param: [],
  ),
  // 23
  Parameter(
    title: "Режи работы триггера 3",
    description: 'Режим раоты тригера канал 3',
    data: defaultGenralSettings[23],
    type: SettingsType.list,
    param: ["Включить при удерживании", "переключать без сохранения", "переключать с сохранением"],
  ),
  // 24
  Parameter(
    title: "Состояние триггера 3",
    description: 'Отоброжает состояние триггера в реальном времени',
    data: defaultGenralSettings[24],
    type: SettingsType.togle,
    param: [],
  ),
  // 25
  Parameter(
    title: "Тема текстового дисплея ",
    description: 'Тема отображения информации на текстовом дисплее',
    data: defaultGenralSettings[25],
    type: SettingsType.list,
    param: ["Тема 1", "Тема 2", "Тема 3"],
  ),
  // 26
  Parameter(
    title: "Тема oled дисплея ",
    description: 'Тема отображения информации на oled дисплее',
    data: defaultGenralSettings[26],
    type: SettingsType.list,
    param: ["Тема 1", "Тема 2", "Тема 3"],
  ),
  // 27
  Parameter(
    title: "Тема nextion дисплея ",
    description: 'Тема отображения информации на nextion дисплее',
    data: defaultGenralSettings[27],
    type: SettingsType.list,
    param: ["Тема 1", "Тема 2", "Тема 3"],
  ),
  // 28
  Parameter(
    title: "Состояние дислея Nextion",
    description: 'Если отключен то данные на дислей некстион отправляться не будут',
    data: defaultGenralSettings[28],
    type: SettingsType.togle,
    param: [],
  ),
  // 29
  Parameter(
    title: "Яркость дисплея",
    description: 'Выставляет яркость дислеев',
    data: defaultGenralSettings[29],
    dataMax: 16,
    type: SettingsType.slider,
    param: [],
  ),
  // 30
  Parameter(
    title: "Режим яркости",
    description: 'Управление яркость, можно в ручную можно чтоб он выставлялся автоматический (в разработке)',
    data: defaultGenralSettings[30],
    type: SettingsType.list,
    param: ["Ручной", "Автоматический"],
  ),
  // 31
  Parameter(
    title: "Напряжение отключения АСС",
    description:
        'Если напряжение упадёт ниже указанного значения, то АСС будет отключено для защиты аккумуляторов от глубокого раряда',
    data: defaultGenralSettings[31],
    measure: "Вольт",
    dataMin: 100,
    dataMax: 130,
    delimiter: 1,
    type: SettingsType.slider,
    param: [],
  ),
  // 32
  Parameter(
    title: "Увеличивать грмокость",
    description:
        'Некоторые планшеты при подключении внешней usb звуковой карты выставляют громкость на ~70%, если у вас так то включите эту функцию',
    data: defaultGenralSettings[32],
    type: SettingsType.togle,
    param: [],
  ),
  // 33
  Parameter(
    title: "Режим выбора интерфейса отравки данных",
    description:
        'Параметр отределяет куда отправлять данные при обмене данными с нескольими интерфейсами USB-HID, USB-CDC, Bluetooh',
    data: defaultGenralSettings[33],
    type: SettingsType.list,
    param: ["Ручной", "Автоматический"],
  ),
  // 34
  Parameter(
    title: "Принудительная отравка на USB-HID",
    description: 'Если есть подключение даныые будут отравлены обязательно',
    data: defaultGenralSettings[34],
    type: SettingsType.togle,
    param: [],
  ),

  // 35
  Parameter(
    title: "Принудительная отравка на USB-CDC",
    description: 'Если есть подключение даныые будут отравлены обязательно',
    data: defaultGenralSettings[35],
    type: SettingsType.togle,
    param: [],
  ),
  // 36
  Parameter(
    title: "Принудительная отравка на BLUETOOTH",
    description: 'Если есть подключение даныые будут отравлены обязательно',
    data: defaultGenralSettings[36],
    type: SettingsType.togle,
    param: [],
  ),
];
