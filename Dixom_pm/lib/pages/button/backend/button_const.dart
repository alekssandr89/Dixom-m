enum ExternalControlPacked {
  message("Message"), //0 сообщения пакета звук
  notification("Notification"), // 1 Уведомления
  settings("Settings"), //2 Настройки
  button("Button"), // 3 настроенная кнпка
  newButton("New button"), // 4 новая кнопка
  btnName("Button name"); // 5 имя кнопки

  final String decoding;
  const ExternalControlPacked(this.decoding);
}

enum ParceOneButton {
  packed, //0  Пакет External Control
  param, //1  *Параметр button
  channel, //2  Канал
  id, //3 Значение ADC 10 бит (ID кнопки)
  interval, //4 Интервал (погрешность АЦП)
  action, //5  Действие при клике 1
  delayAction, //6  Действие при удерживании 1
  doublAction, //7  Действие при двойном клике 1
  delayTiming, //8  Таймер для удерживания
  dobleTiming, //9 Таймер для двойного клика
  action2, //10 Действие при клике 2
  delayAtion2, //11 Действие при удерживании 2
  doubleAction2, //12 Действие при двойном клике 2
  param1, //13 Параметр 1
  param2, //14 Параметр 2
  param3, //15 Параметр 3
  param4, //16 Параметр 4
}
