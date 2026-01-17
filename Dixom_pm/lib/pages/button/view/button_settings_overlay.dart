// ignore_for_file: public_member_api_docs, sort_constructors_first
import 'dart:math';
import 'package:dixom/main.dart';
import 'package:flutter/material.dart';
import 'package:dixom/components/canvas/knop/knop_view.dart';
import 'package:dixom/dataBase/app_constants.dart';
import 'package:dixom/pages/button/external_control.dart';

import '../backend/button_model.dart';
import '../backend/button_provider.dart';
import 'button_action.dart';

late OneButton button;

final List<DropdownMenuItem<int>> btnList = btnAction.entries
    .map(
      (entry) => DropdownMenuItem<int>(
        value: entry.key,
        child: Text(entry.value.action),
      ),
    )
    .toList();

Row actionBtn(String action, int selectedValue, Function(int?) onChanged) {
  return Row(
    children: [
      Expanded(child: Text(action)),
      Expanded(
        flex: 2,
        child: DropdownButton<int>(
          value: selectedValue,
          onChanged: onChanged,
          items: btnList,
        ),
      )
    ],
  );
}

Column valueSetup(String param, int value, Function(EncoderState) onChanged) {
  return Column(
    children: [
      Text(param),
      const SizedBox(height: 16),
      SizedBox(
        width: 130,
        height: 130,
        child: Stack(children: [
          KnopView(
            minAngle: (0 * pi) / 180,
            maxAngle: (360 * pi) / 180,
            steps: 30,
            controlMode: ControlMode.encoder,
            onChange: (double value) {},
            onDirection: onChanged,
          ),
          Center(
            child: Container(
              width: 50,
              height: 50,
              margin: const EdgeInsets.symmetric(horizontal: 3),
              padding: const EdgeInsets.symmetric(horizontal: 3, vertical: 2),
              decoration: BoxDecoration(borderRadius: BorderRadius.circular(50), color: ColorDisplay.backgroundOn),
              child: Center(
                child: Text(
                  value.toString(),
                  style: const TextStyle(fontSize: 19, color: ColorDisplay.lable),
                ),
              ),
            ),
          ),
        ]),
      ),
    ],
  );
}

void buttontSettings(BuildContext context, int index, OneButton btn) {
  showDialog(
    context: context,
    builder: (BuildContext context) {
      return AlertDialog(
        backgroundColor: ColorPanel.background,
        title: Text("Настройки кнопки: ${getChannel(btn.channel, btn.id)}"),
        content: SizedBox(height: 500, width: 500, child: BtnSetup(btn: btn)),
        actions: <Widget>[
          TextButton(
            child: const Text("?"),
            onPressed: () {
              answerToQuestions(context);
            },
          ),
          TextButton(
            child: const Text('Сбросить по умолчанию'),
            onPressed: () {},
          ),
          TextButton(
            child: const Text('Отменить'),
            onPressed: () {
              Navigator.of(context).pop();
            },
          ),
          TextButton(
            child: const Text('Применить'),
            onPressed: () {
              appRef.read(externalControlPrdr.notifier).setButton(index, button);
              Navigator.of(context).pop();
            },
          ),
        ],
      );
    },
  );
}

class BtnSetup extends StatefulWidget {
  final OneButton btn;
  const BtnSetup({super.key, required this.btn});

  @override
  State<BtnSetup> createState() => _BtnSetupState();
}

class _BtnSetupState extends State<BtnSetup> {
  @override
  void initState() {
    super.initState();
    button = widget.btn;
  }

  @override
  Widget build(BuildContext context) {
    return Column(
      children: [
        actionBtn("Одиночный клик", button.action, (newValue) {
          setState(() {
            button.action = newValue as int;
          });
        }),
        actionBtn("Удерживание", button.delayAction, (newValue) {
          setState(() {
            button.delayAction = newValue as int;
          });
        }),
        actionBtn("Двойной клик", button.doublAction, (newValue) {
          setState(() {
            button.doublAction = newValue as int;
          });
        }),
        actionBtn("Одиночный клик 2", button.action2, (newValue) {
          setState(() {
            button.action2 = newValue as int;
          });
        }),
        actionBtn("Удерживание 2", button.delayAtion2, (newValue) {
          setState(() {
            button.delayAtion2 = newValue as int;
          });
        }),
        actionBtn("Двойной клик 2", button.doubleAction2, (newValue) {
          setState(() {
            button.doubleAction2 = newValue as int;
          });
        }),
        const SizedBox(height: 20),
        Row(
          mainAxisAlignment: MainAxisAlignment.spaceAround,
          children: [
            /* valueSetup("Диапазон", button.interval, (newValue) {
              setState(() {
                if (newValue == EncoderState.left) {
                  button.interval -= 1;
                } else {
                  button.interval += 1;
                }
              });
            }),*/
            valueSetup("Таймер удержания", button.delayTiming, (newValue) {
              setState(() {
                if (newValue == EncoderState.left) {
                  button.delayTiming -= 1;
                } else {
                  button.delayTiming += 1;
                }
              });
            }),
            valueSetup("Таймер двоного клика", button.dobleTiming, (newValue) {
              setState(() {
                if (newValue == EncoderState.left) {
                  button.dobleTiming -= 1;
                } else {
                  button.dobleTiming += 1;
                }
              });
            }),
          ],
        )
      ],
    );
  }
}

void answerToQuestions(BuildContext context) {
  showDialog(
    context: context,
    builder: (BuildContext context) {
      return AlertDialog(
        backgroundColor: ColorPanel.background,
        content: const Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text("Одиночный клик: Выполняется после однократного нажатия на кнопу"),
            Divider(),
            Text("Двойной клик: Выполняется после двухкратного нажатия на кнопку"),
            Divider(),
            Text("Удерживание: Выполняется после удержания клавиши"),
            Divider(),
            Text("Одиночный клик 2: Выполниться сразу полсе события одиночного клика"),
            Divider(),
            Text("Двойной клик 2: Выполниться сразу полсе события двойного клика"),
            Divider(),
            Text("Удерживание 2: Выполниться сразу полсе события удерживание"),
            Divider(),
            Text(
                "Диапазон: В случае использования АЦП возникают погрешности измерения аналогового сигнала, поэтому для аналоговых или как ещё говорят резистивных кнопок необходимо указывать на диапазон значений в которых кнопка будет срабатывать. По умолчанию необходимо отправять диапазон с значением 0, контроллер автоматический пересчитает необходимый диапазон для кнопок. "),
            Divider(),
            Text(
                "Таймер двойного клика: После одинарного клика платформа будет ждать указанное время, и если указаное время не прошло и вы нажали кнопку ещё раз, то сработает событие двойной клик."),
            Divider(),
            Text(
                "Таймер удерживания: После нажатия кнопки и удерживании запускается таймер, и если спустя указанное время кнопка не будет опущена то сработает действие удерживания"),
          ],
        ),
        actions: <Widget>[
          TextButton(
            child: const Text('Понятно'),
            onPressed: () {
              Navigator.of(context).pop();
            },
          ),
        ],
      );
    },
  );
}
