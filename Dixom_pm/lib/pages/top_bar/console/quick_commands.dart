import 'package:dixom/components/utilities/responsive.dart';
import 'package:dixom/driver/device_rx_tx/transmiter.dart';
import 'package:flutter/material.dart';
import '../../../dataBase/app_constants.dart';

class QuickCommands extends StatelessWidget {
  const QuickCommands({
    super.key,
  });

  @override
  Widget build(BuildContext context) {
    return SingleChildScrollView(
      child: Container(
        width: !Responsive.isMobile(context) ? 350 : double.infinity,
        padding: const EdgeInsets.only(left: AppIndents.pading, right: AppIndents.pading),
        margin: !Responsive.isMobile(context)
            ? const EdgeInsets.only(right: AppIndents.pading, bottom: AppIndents.pading)
            : const EdgeInsets.only(),
        decoration: BoxDecoration(
          color: AppColor.secondary,
        ),
        child: Column(
          children: [
            FastCommand(defCommand: "0 3 - Версия прошивки"),
            FastCommand(defCommand: "0 6 - Перезагрузка"),
            FastCommand(defCommand: "1 4 - Все изменения"),
            FastCommand(defCommand: "1 8 - Журнал обмена"),
            FastCommand(defCommand: "1 5 - Всё сохрнить"),
            FastCommand(defCommand: "1 12 - Сканировать I2C1"),
            FastCommand(defCommand: "1 13 - Сканировать I2C2"),
            FastCommand(defCommand: "1 7 - Перезагрузка ЦАП"),
            FastCommand(defCommand: "1 19 100 1 - Проерка интерфейса"),
            FastCommand(defCommand: "1 6=1 - сброс системных настроек"),
            FastCommand(defCommand: "1 6=2 - сброс внешних кнопок"),
            FastCommand(defCommand: "1 6=3 - сброс DSP"),
            FastCommand(defCommand: "1 6=5 - сброс FM Radio"),
            FastCommand(defCommand: "1 6=10 - сброс CAN шины"),
            FastCommand(defCommand: "4 2=2 2 - 0-DAC-1-OFF, 2-ON"),
            FastCommand(defCommand: "4 2=2 1 - 0-DAC-1-OFF, 2-ON"),
            FastCommand(defCommand: "4 11=0 - Прессет 0-3"),
            FastCommand(defCommand: ""),
            FastCommand(defCommand: ""),
            FastCommand(defCommand: ""),
            FastCommand(defCommand: ""),
            FastCommand(defCommand: ""),
          ],
        ),
      ),
    );
  }
}

class FastCommand extends StatelessWidget {
  FastCommand({
    super.key,
    required this.defCommand,
  });

  final String defCommand;
  final TextEditingController _controller = TextEditingController();

  @override
  Widget build(BuildContext context) {
    _controller.text = defCommand;
    return Row(
      children: [
        Expanded(
          child: TextField(
            controller: _controller,
            decoration: const InputDecoration(
              hintText: "Введите команду",
            ),
          ),
        ),
        const SizedBox(
          width: 10,
        ),
        ElevatedButton(
          onPressed: () {
            transmitter(_controller.text);
          },
          child: const Text('>'),
        ),
      ],
    );
  }
}
