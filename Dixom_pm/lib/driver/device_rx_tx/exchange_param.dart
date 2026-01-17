///Основные пакеты команд аудиоплатформы
enum RootPacked {
  /// 0 системные сообщения, обновление, информация о версии
  system("System"),

  /// 1 общие параметры
  general("General"),

  /// 2 Шлюс для передачи данных между различными интерфейсами
  gateway("Gateway"),

  /// 3 Кан шина
  canBus("Can Bus"),

  /// 4 Настройки звука
  sound("Sound"),

  /// 5 Параметры bluetooth
  bluetooth("Bluetooth"),

  /// 6 Настройки внешних кнопок управления
  extControl("External Control"),

  ///7 Настройки и параметры ФМ радио модуля
  radio("FM Radio");

  final String decoding;
  const RootPacked(this.decoding);
}

///Параметры пакета система "System" 0
enum SystemPacked {
  fuUsbSoundCard("Update sound card"), //0 Обновление звуковой карты
  notification("Notification"), // 1 Уведомления
  settings("System settings"), //2 Настройки
  information("Firmware information"), // 3 Информация о прошивке
  accesControl("Acces control"), //4 Настройка доступа к системе обновления
  txControl("Data out control"), //5 Управление вывод данных из устрйоства
  restart("Device restart"), //6 Перезагрузка устройства
  fuDownload("Download firmware"), // 7 загрузка прошивки в устройство
  fuPrepairingUpdate("Preparing update"), //8 Подготовка к обновлению
  fuCompleteUpdate("Update comlete"), // 9 Завершение обновления
  fuCheckFirmware("Check firmware"), // 10 Проверка прошивки
  fuCancel("Update cansel"), // 11 Отмена обновления
  fuBootloader("Update bootloader"), // 12 Обновление загрузчика
  spiFlashState("Check spi flash state"), //13 Доступность флешь памяти
  spiFlashErase("Erase spi flash"), //14 Очистка флешь памяти
  licenseKey("Set license key"), //15 Ввод ключа активации аудиоплатформы
  pcbRevision("Set pcb revision"); //16 Ввод ревизии печатной платы

  final String decoding;
  const SystemPacked(this.decoding);
}

///Параметры пакета общие "General" 1
enum General {
  message("Сообщения"), //0
  notification("Уведомления"), // 1
  settings("Общие настройки"), //2
  noSaveSettings("Общие настройки без сохранения"), //3
  showModified("Показать все изменения"), // 4
  dataSave("Сохранение настроек"), //5
  dataDefaul("Сброс настроек"), //6
  dacRestart("Перезагрузка ЦАП"), //7
  log("Журнал обмена"), // 8
  register("Управление регистрами"), //9
  timer("Настройки таймеров"), //10
  dataTime("Настройка даты и времени"), //11
  i2c1Scan("Сканирование шины I2C1"), //12
  i2c2Scan("Сканирование шины I2C2"), //13
  iwdg("Управление сторожевым таймером"), //14
  spi("Управление шиной SPI"), //15
  action("Выполнение действий"), //16
  usbPwrControl("Управление питанием USB протов"), //17
  spiStatus("Состояние шины SPI"), //18
  testReciveDataLost("Тест интерфесов обмена"), //19
  rtcTemperature("Датчик температуры в RTC"); //20

  final String decoding;
  const General(this.decoding);
}
