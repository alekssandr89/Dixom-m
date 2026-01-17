const int updateRestar = 10; //время перезагрузка платформы

const stableUrlDixomM = "https://dixom.ru/Dixom/Firmware/Dixom_m/Stable/FrimwareList.json";
const stableUrlDixomC12 = "https://dixom.ru/Dixom/Firmware/Dixom_c12/Stable/FrimwareList.json";
const stableUrlDixomHiResAudio = "https://dixom.ru/Dixom/Firmware/Dixom_HiResAudio/Stable/FrimwareList.json";

const experimentalUrlDixomM = "https://dixom.ru/Dixom/Firmware/Dixom_m/Stable/FrimwareList.json";
const experimentalUrlDixomC12 = "https://dixom.ru/Dixom/Firmware/Dixom_c12/Stable/FrimwareList.json";
const experimentalUrlDixomHiResAudio = "https://dixom.ru/Dixom/Firmware/Dixom_HiResAudio/Stable/FrimwareList.json";

/*Расположение информация о прошивке в файле (после split)*/
class LocatInfoInFile {
  static const int type = 0;
  static const int model = 1;
  static const int revPcb = 2;
  static const int param = 3;
  static const int version = 4;
  static const int dataTime = 5;
  static const int size = 6;
  static const int crc = 7;
  static const int appAddr = 8;
  static const int eraseSector = 9;
  static const int eraseCount = 10;
}

enum UpdateStatus {
  updatePreparationDone("Подготовка к обновлению завершена"),
  succesVerified("Прошивка успешно проверена"),
  isCorrupted("Прошивка повреждена"),
  uploadError("Ошибка загрузки прошивки"),
  uploadSucces("Прошивка успешно загружена"),
  memoryChipReady("Микросхема флэш-памяти готова"),
  memoryChipNotFound("Флэш-чип не найден"),
  reboot("Перезагрузка"),
  cancel("Обновление отменено"),
  updateSystemUnlock("Система обновления разблокирована"),
  updateSystemBlocked("Система обновления заблокирована"),
  noBootloaderUpdateRequired("Не требуется обновление загрузчика"),
  bootloaderUpdatedSucces("Загрузчик обновлен успешно"),
  bootloaderUpdateFail("Ошибка обновления загрузчика"),
  notReadyForUpdate("Не готов к обновлению прошивки"),
  systemCommandsAreBlocked("Системные команды обновления заблокированы"),
  succesDelivery("Успешная доставка пакета обновления"),
  okClearedSpiFlashMemory("Очистил флэш-память spi"),
  errorClearedSpiFlashMemory("Очистки флэш-памятьи spi"),
  errorWhilePreparingUpdate("Ошибка при подготовке обновления"),
  errorDelivery("Ошибка при доставке пакета обновления"),
  appNoUpdateRequired("Обновление не требуется"), //относится к приложению
  appUpdateRequired("Требуется обновление"), //относится к приложению
  appDownload("Скачиваю прошивку из интернета..."), //относится к приложению
  appUpload("Загружаю прошивку в платформу..."), //относится к приложению
  appUnlock("Разблокировка системы обновления..."), //относится к приложению
  appUpdateBootloader("Обновляю загрузчик..."), //относится к приложению
  appDone("Перезагрузка для завершения обновления через... "), //относится к приложению
  appPreparation("Подготовка к обновлению..."), //относится к приложению
  appCheck("Проверка прошивки на ошибки..."), //относится к приложению
  appDownloadError("Похоже нет подключения к интернету или сервер Dixom не доступен"); //относится к приложению

  final String decoding;
  const UpdateStatus(this.decoding);
}

enum UpdateBtnAction {
  getJson("Проверить обновления"),
  download("Загрузить"),
  cancellation("Отменить"),
  bootloaderUpdate("Обновить загрузчик"),
  disable("Кнопка отключена");

  final String decoding;
  const UpdateBtnAction(this.decoding);
}

const String frimwareModelTest = '''
{  
   "Type"               :"Platform",
   "Device"             :"Dixom-m",
   "Release"            :"Stable",
   "FrimwareList":[    
        {
        "Version"             :"4.025",
        "Date"                :"01.10.2022",
        "Crc32"               :"1787637685",
        "Size"                :"485632",
        "URL"                 :"https://dixom.ru",
        "Information"         :"Добавлена "  
        },
    
        {
        "Version"             :"4.023",
        "Date"                :"30.09.2022",
        "Crc32"               :"927954348",
        "Size"                :"112640",
        "URL"                 :"https://dixom.ru",
        "Information"         :"Добавлена"  
        }
    ]
}
''';
