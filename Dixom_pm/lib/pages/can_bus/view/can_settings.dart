import 'package:dixom/components/widgets/connection.dart';
import 'package:dixom/dataBase/app_constants.dart';
import 'package:flutter/gestures.dart';
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

import '../backend/can_const.dart';
import '../backend/can_model.dart';
import '../backend/can_provider.dart';
import '../backend/vehicle/car_list.dart';

void canSetup(BuildContext context) {
  showDialog(
    context: context,
    builder: (BuildContext context) {
      return AlertDialog(
        backgroundColor: ColorPanel.background,
        title: const Text("Параметры can шины"),
        content: const SizedBox(height: 430, width: 450, child: CanSetup()),
        actions: <Widget>[
          TextButton(
            child: const Text('Закрыть'),
            onPressed: () {
              Navigator.of(context).pop();
            },
          ),
        ],
      );
    },
  );
}

class CanSetup extends StatefulWidget {
  const CanSetup({super.key});

  @override
  State<CanSetup> createState() => _CanSetupState();
}

class _CanSetupState extends State<CanSetup> {
  @override
  void initState() {
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    return Consumer(builder: (context, ref, child) {
      CanStateModel canState = ref.watch(canStatePrdr);
      return SingleChildScrollView(
        dragStartBehavior: DragStartBehavior.start,
        child: Column(
          children: [
            SettingsChecked(
              parrametr: "Отправка в CAN шину",
              description: "Разрешает отправлять данные из платформы в CAN шину. На вашу ответсвенность!",
              state: canState.txPermision,
              onTap: () {
                ref.read(canStatePrdr.notifier).setTxPermision();
              },
            ),
            const Divider(),
            SettingsChecked(
              parrametr: "Автоподключение",
              description: "Авотматическое подключение к CAN шине при зауске платформы.",
              state: canState.autoConnect,
              onTap: () {
                ref.read(canStatePrdr.notifier).setAutoConnect();
              },
            ),
            const Divider(),
            Row(
              children: [
                const Expanded(child: Text("Can кадров в пакете:")),
                Expanded(
                  flex: 2,
                  child: DropdownButton<int>(
                    value: canState.framesInPacked,
                    onChanged: (newIndex) {
                      ref.read(canStatePrdr.notifier).setFramesInPacked(newIndex);
                    },
                    items: framesInPacked.asMap().entries.map<DropdownMenuItem<int>>((entry) {
                      final int index = entry.key;
                      final String value = entry.value;
                      return DropdownMenuItem<int>(
                        value: index,
                        child: Text(value),
                      );
                    }).toList(),
                  ),
                )
              ],
            ),
            Row(
              children: [
                const Expanded(child: Text("Скорость:")),
                Expanded(
                  flex: 2,
                  child: DropdownButton<int>(
                    value: canState.speed,
                    onChanged: (newIndex) {
                      ref.read(canStatePrdr.notifier).setSpeed(newIndex);
                    },
                    items: canSpeed.asMap().entries.map<DropdownMenuItem<int>>((entry) {
                      final int index = entry.key;
                      final String value = entry.value;
                      return DropdownMenuItem<int>(
                        value: index,
                        child: Text(value),
                      );
                    }).toList(),
                  ),
                )
              ],
            ),
            const Divider(),
            Row(
              children: [
                const Expanded(child: Text("Марка:")),
                Expanded(
                  flex: 2,
                  child: DropdownButton<int>(
                    value: ref.read(canStatePrdr).manufacturer,
                    onChanged: (newIndex) {
                      ref.read(canStatePrdr.notifier).setCarManufacturer(newIndex);
                    },
                    items: carManufacturer.asMap().entries.map<DropdownMenuItem<int>>((entry) {
                      return DropdownMenuItem<int>(
                        value: entry.key,
                        child: Text(entry.value.name),
                      );
                    }).toList(),
                  ),
                )
              ],
            ),
            Row(
              children: [
                const Expanded(child: Text("Модель:")),
                Expanded(
                  flex: 2,
                  child: Consumer(builder: (context, ref, child) {
                    CarManufacturer carMan = carManufacturer[ref.read(canStatePrdr).manufacturer];
                    return DropdownButton<int>(
                      value: ref.read(canStatePrdr).model,
                      onChanged: (newIndex) {
                        ref.read(canStatePrdr.notifier).setCarModel(newIndex);
                      },
                      items: carMan.list.asMap().entries.map<DropdownMenuItem<int>>((entry) {
                        return DropdownMenuItem<int>(
                          value: entry.key,
                          child: Text(entry.value.name),
                        );
                      }).toList(),
                    );
                  }),
                )
              ],
            ),
            Row(
              children: [
                const Expanded(child: Text("Поколение:")),
                Expanded(
                  flex: 2,
                  child: Consumer(builder: (context, ref, child) {
                    CarModel model =
                        carManufacturer[ref.read(canStatePrdr).manufacturer].list[ref.read(canStatePrdr).model];
                    return DropdownButton<int>(
                      value: ref.read(canStatePrdr).generation,
                      onChanged: (newIndex) {
                        ref.read(canStatePrdr.notifier).setCarGeneration(newIndex);
                      },
                      items: model.list.asMap().entries.map<DropdownMenuItem<int>>((entry) {
                        return DropdownMenuItem<int>(
                          value: entry.key,
                          child: Text(entry.value.generation),
                        );
                      }).toList(),
                    );
                  }),
                )
              ],
            ),
          ],
        ),
      );
    });
  }
}

/**
 * flutter
 * 
 * есть список из моделей автомобилей
 */
