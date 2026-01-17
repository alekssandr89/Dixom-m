enum FmRadioPacked {
  /// 0 сообщения пакета can
  message("Message"),

  /// 1 Уведомления
  notification("Notification"),

  /// 2 Настройки
  settings("Settings"),

  /// 3 Ткущий пресет
  currentPreset("Current preset"),

  /// 4 список пресетов
  listPreset("List preset"),

  /// 5 частота
  frequnci("Frequnci"),

  /// 6 тип микрочипа
  chip("Chip"),

  /// 7 Автоматический поиск станции
  autoSeach("Auto search"),

  /// 8 Ручной поиск станции
  manualSeach("Manual search"),

  /// 9 Сохранение пресета
  savePreset("Save preset"),

  /// 10 Автоматичское сохранение
  autoSavePreset("Auto save preset"),

  /// 11 Удаление пресета
  deletePreset("Delete preset"),

  /// 12 Получение всех изменёных данных
  getAllModifiti("Delete preset");

  final String decoding;
  const FmRadioPacked(this.decoding);
}

enum FmRadioMessage {
  /// 0 сервисная информация
  serviceInfo,
}
