import 'can_model.dart';

enum CanBusPacked {
  message("Message"), //0 сообщения пакета can
  notification("Notification"), // 1 Уведомления
  settings("Settings"), //2 Настройки
  status("Status"), // 3 Состояние can шины
  speed("Speed"), // 4 скорость can шины
  filter("Filter"), // 5 фильтр
  car("Car"); // 5 автомобиль

  final String decoding;
  const CanBusPacked(this.decoding);
}

List<CarGeneration> defCarGeneration = [CarGeneration("Поколение")];
List<CarModel> defCarModel = [CarModel("Модель", defCarGeneration)];

enum CanMessageParam {
  frame, //0 can кадр
}

///Тип кан кадра
enum CanIdType {
  standart,
  extended,
}

///Тип кан кадра
enum ViewMode {
  list,
  monitor,
}

enum CanSettings {
  /// 0 Максимальное количество can кадров в одном пакете
  framesInPacked("Maximum number of messages in one batch"),

  /// 1 Скорость шины can
  busSpeed("CanSpeed default 100kBit/s"),

  /// 2 Разрешение писать данные в Can шину
  txPermision("Can listen only"),

  /// 3 Автоматическое пдключение к платформе при запуске системы
  autoConnect("Auto connect 0 no, 1 yes"),

  /// 4 Разрешает передавать нанные из контроллера в программу dixom pm
  dataOutPermision("can data out permision"); // 9

  final String decoding;
  const CanSettings(this.decoding);
}

/*     | | | | | | | | +--------7 HandlebarSide Сторона руля
      | | | | | | | +----------4 DriveUnit Привод
      | | | | | | +------------6 Transmission Трансмиссия
      | | | | | +--------------5 Year Год 
      | | | | +----------------4 Generation Поколение
      | | | +------------------3 Model Модель
      | | +--------------------2 Manufacturer Производитель
      | +----------------------1 параметр car
      +------------------------0 пакет can*/
enum CanCarParram {
  packed("Пакет"),
  parram("Параметр"),
  manufacturer("Производитель"),
  model("Модель"),
  generation("Поколение"),
  year("Год выпуска"),
  transmission("Трансмисия"),
  driveUnit(" "),
  handlebarSide(" ");

  final String decoding;
  const CanCarParram(this.decoding);
}

const defCanSpeedIndex = 3;
final List<String> canSpeed = [
  "10 kBit/c",
  "20 kBit/c",
  "50 kBit/c",
  "100 kBit/c",
  "125 kBit/c",
  "250 kBit/c",
  "500 kBit/c",
  "800 kBit/c",
  "1000 kBit/c",
  "Custom kBit/c",
];

const defFramesInPackedIndex = 6;
final List<String> framesInPacked = [
  "1 кадр в 1 пакете",
  "2 кадра в 1 пакете",
  "3 кадра в 1 пакете",
  "4 кадра в 1 пакете",
  "До 5 кадров в 1 пакете",
  "До 6 кадров в 1 пакете",
  "До 7 кадров в 1 пакете",
];
