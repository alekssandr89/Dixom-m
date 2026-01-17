import 'package:dixom/components/widgets/connection.dart';
import 'package:dixom/components/widgets/settings_widget.dart';
import 'package:dixom/dataBase/icons/custom_icons.dart';
import 'package:dixom/main.dart';
import 'package:flutter/material.dart';
import 'package:dixom/dataBase/app_constants.dart';
import 'package:dixom/pages/top_bar/top_bar.dart';
import 'package:dixom/pages/home/sidebar.dart';
import 'package:dixom/components/utilities/responsive.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

import 'backend/settings_const.dart';
import 'backend/settings_provider.dart';

class GeneralSettings extends StatelessWidget {
  const GeneralSettings({super.key});

  @override
  Widget build(BuildContext context) {
    appRef.read(rtcPrdr.notifier).getTemperatureAndDataTime();

    return Scaffold(
      drawer: const Menu(),
      body: SafeArea(
        child: Container(
          color: ColorPanel.background,
          child: Row(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              if (Responsive.isDesktop(context)) const Expanded(child: Menu()),
              Expanded(
                flex: 4,
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    const DixomAppBar(backState: false),
                    temperatureAndDataTime(),
                    Expanded(
                      child: Container(
                        padding: const EdgeInsets.symmetric(horizontal: 20, vertical: 20),
                        child: Consumer(builder: (context, ref, child) {
                          ref.watch(senttingsPrdr);
                          return settings(ref);
                        }),
                      ),
                    )
                  ],
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }

  ListView settings(WidgetRef ref) {
    return ListView.builder(
      itemCount: generalSettings.length,
      itemBuilder: (context, index) {
        final parameter = generalSettings[index];
        return Column(
          children: [
            if (parameter.type == SettingsType.togle)
              settingsTogle(
                parrametr: parameter.title,
                description: parameter.description,
                state: parameter.data == 0 ? false : true,
                onChange: (bool value) {
                  ref.read(senttingsPrdr.notifier).setSettins(index, value == true ? 1 : 0);
                },
              ),
            if (parameter.type == SettingsType.slider)
              settingsSlider(
                parrametr: parameter.title,
                description: parameter.description,
                value: parameter.data.toDouble(),
                valueMin: parameter.dataMin.toDouble(),
                valueMax: parameter.dataMax.toDouble(),
                onChange: (double value) {
                  ref.read(senttingsPrdr.notifier).setSettins(index, value.toInt().ceil());
                },
                measure: parameter.measure,
                delimiter: parameter.delimiter,
              ),
            if (parameter.type == SettingsType.list)
              settingsList(
                parrametr: parameter.title,
                description: parameter.description,
                value: parameter.data,
                list: parameter.param,
                onChange: (int value) {
                  ref.read(senttingsPrdr.notifier).setSettins(index, value);
                },
              ),
            const Divider(color: Colors.grey),
          ],
        );
      },
    );
  }
}

SizedBox temperatureAndDataTime() {
  const Color boardState = Color.fromARGB(255, 255, 255, 255);
  return SizedBox(
    height: 30,
    child: Row(
      mainAxisAlignment: MainAxisAlignment.spaceAround,
      children: [
        Tooltip(
          message: 'Температура базовой платы',
          child: Row(
            children: [
              const Icon(
                CustomIcons.temperature_low,
                color: boardState,
              ),
              const SizedBox(width: 5),
              Consumer(builder: (context, ref, child) {
                double temperature = ref.watch(rtcPrdr).temperature;
                return Text(
                  temperature.toStringAsFixed(1),
                  style: const TextStyle(
                    fontSize: 20,
                    color: boardState,
                  ),
                );
              }),
            ],
          ),
        ),
        Row(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Tooltip(
              message: 'Время и дата в аудиоплатформе',
              child: Consumer(builder: (context, ref, child) {
                DateTime dataTime = ref.watch(rtcPrdr).dataTime;
                return Text(
                  "${dataTime.hour}:${dataTime.minute}:${dataTime.second} ${dataTime.day}/${dataTime.month}/${dataTime.year}",
                  style: const TextStyle(
                    fontSize: 20,
                    color: boardState,
                    textBaseline: TextBaseline.ideographic,
                  ),
                );
              }),
            ),
            const SizedBox(width: 5),
            Tooltip(
              message: 'Установить текущее время в аудиоплатформу',
              child: Material(
                color: Colors.transparent, // Устанавливаем прозрачный цвет для фона
                child: InkWell(
                  onTap: () {
                    appRef.read(rtcPrdr.notifier).setCurrentDataTime();
                  },
                  borderRadius: BorderRadius.circular(30), // Радиус для эффекта
                  child: const Padding(
                    padding: EdgeInsets.all(8.0), // Добавляет пространство вокруг иконки
                    child: Icon(
                      size: 20,
                      CustomIcons.system_update_alt,
                      color: boardState,
                    ),
                  ),
                ),
              ),
            ),
          ],
        ),
        Tooltip(
          message: 'Обновить данные о температуре и времени',
          child: Material(
            color: Colors.transparent, // Устанавливаем прозрачный цвет для фона
            child: InkWell(
              onTap: () {
                appRef.read(rtcPrdr.notifier).getTemperatureAndDataTime();
              },
              borderRadius: BorderRadius.circular(30), // Радиус для эффекта
              child: const Padding(
                padding: EdgeInsets.all(8.0), // Добавляет пространство вокруг иконки
                child: Icon(
                  size: 20,
                  CustomIcons.sync_icon,
                  color: boardState,
                ),
              ),
            ),
          ),
        ),
      ],
    ),
  );
}
