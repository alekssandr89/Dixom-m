class DeviceInfo {
  const DeviceInfo(this.vid, this.pid, this.model);
  final int vid;
  final int pid;
  final String model;
}

class FrameEncoding {
  static const int utf8Whole = 33; // '!',
  static const int utf8PartStart = 34; // '"',
  static const int utf8Part = 35; // '#',
  static const int utf8PartEnd = 36; // '$',

  static const int base64Whole = 37; // '%',
  static const int base64PartStart = 38; // '&',
  static const int base64Part = 39; // '\'',
  static const int base64PartEnd = 40; // '(',

  static const int byteWhole = 41; // ')',
  static const int bytePartStart = 42; // '*',
  static const int bytePart = 43; // '+',
  static const int bytePartEnd = 44; // ',',
}

class DeviceFilter {
  static final List<DeviceInfo> deviceList = [
    const DeviceInfo(61525, 6, "Dixom-m"),
    const DeviceInfo(61525, 7, "Dixom-m Bootloader"),
    const DeviceInfo(61525, 8, "Dixom-c12")
  ];
}

enum InterfaceState {
  status("Общее состояние подключения"),
  statusIn("Состояние подключения вход"),
  statusOut("Состоялние подключения выход"),
  succesIn("Успешная доставка вход"),
  succesOut("Успешная доставка выход"),
  errorIn("Ошибка доставка"),
  errorOut("Общее состояние подключения");

  final String decoding;
  const InterfaceState(this.decoding);
}

///Физические интерфейсы к которым можно подключаться
enum ExchangeInterface {
  usbHid("Usb-Hid"), //0 Способ обмена данными Usb Custom HID
  usbCdc("Usb-Cdc"), //1 Способ обмена данными Usb CDC
  bluetoothSpp("Bluetooth"), //2 Способ обмена данными Bluetooth SPP
  reserved1("Reserved"), //3 Способ обмена данными Bluetooth SPP
  reserved2("Reserved"), //4 Способ обмена данными Bluetooth SPP
  reserved3("Reserved"), //5 Способ обмена данными Bluetooth SPP
  none("---"); //6 Способ обмена данными Bluetooth SPP

  final String decoding;
  const ExchangeInterface(this.decoding);
}

const List<ExchangeInterface> exchangeInterface = ExchangeInterface.values;

///Состояние приложения
enum DeviceStates {
  initialization("Загрузка"), //0
  readyForWork("Готово к работе"), //1
  safeMode("Безопасный режим"), //2
  dataSave("Сохранение настроек"), //3
  waitingForShutdown("Ожидание отключения"), //4
  firmwareUpdate("Обновление прошивки"), //5
  bootLoader("Режиме загрузчика"), //6
  debugMode("Режим разработчка"); //7

  final String decoding;
  const DeviceStates(this.decoding);
}

///Состояние приложения
enum AppStates {
  resume("Запущено"), //0
  inactive("Не активно"), //1
  paused("Приостановлено"), //2
  deatached("Закрыто"); //3

  final String decoding;
  const AppStates(this.decoding);
}

enum WorkState {
  notInitialized("Не инициализирован"), //0
  connected("Подключено"), //1
  connecting("Подключение..."), //2
  disconnected("Отключен"), //3
  disconnecting("Отключение..."), //4
  notFound("Платформа не найдена.."), //5
  available("Доступен"), //6
  notAvailable("Платформа не доступена..."), //7
  ok("Удачно"), //8
  error("Ошибка"), //9
  enable("Включено"), //10
  disable("Отключено"), //11
  changed("Изменено"), //12
  noPermission("Нет разрешнеия"), //13
  notSupported("Не поддерживается"), //14
  complete("Полный"), //15
  notComplete("Не полный"), //16
  bluetoothOff("Bluetooth не включен"); //17

  final String decoding;
  const WorkState(this.decoding);
}

const List<WorkState> workState = WorkState.values;
