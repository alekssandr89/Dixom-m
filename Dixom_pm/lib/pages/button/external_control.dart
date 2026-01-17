import 'package:dixom/components/canvas/button/button.dart';
import 'package:dixom/driver/device_rx_tx/transmiter.dart';
import 'package:flutter/material.dart';
import 'package:dixom/dataBase/app_constants.dart';
import 'package:dixom/pages/top_bar/top_bar.dart';
import 'package:dixom/pages/home/sidebar.dart';
import 'package:dixom/components/utilities/responsive.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

import 'backend/button_provider.dart';
import 'backend/button_reciever.dart';
import 'view/button_action.dart';
import 'view/button_settings_overlay.dart';

class ExternalControlPage extends StatefulWidget {
  const ExternalControlPage({super.key});

  @override
  State<ExternalControlPage> createState() => _ExternalControlPageState();
}

String getStrAction(int action) {
  if (btnAction.containsKey(action)) {
    return btnAction[action]!.action;
  } else {
    return btnAction[0]!.action;
  }
}

String getStrDescription(int description) {
  if (btnAction.containsKey(description)) {
    return btnAction[description]!.description;
  } else {
    return btnAction[0]!.description;
  }
}

String getChannel(int channel, int id) {
  String channelStr = "";
  if (channel == 0) {
    String encChannel = "1";
    String encDirection = "лево";
    if (id == 1500 || id == 2000) {
      encChannel = "2";
    }
    if (id == 1000 || id == 2000) {
      encDirection = "право";
    }
    channelStr = "Энкодер $encChannel $encDirection";
  } else if (channel > 0 && channel < 5) {
    channelStr = "АЦП канал $channel";
  } else {
    channelStr = "CAN";
  }
  return "$channelStr, ID $id";
}

//Показывает выбранный элемент в списке
int currentIndex = 0;

class _ExternalControlPageState extends State<ExternalControlPage> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      drawer: const Menu(),
      body: SafeArea(
        child: Container(
          color: ColorPanel.background,
          child: Row(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              //Боковое меню если это десктоп
              if (Responsive.isDesktop(context)) const Expanded(child: Menu()),
              Expanded(
                flex: 4,
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.start,
                  children: [
                    //Статус бар
                    const DixomAppBar(backState: false),
                    //Всё остальное простанство под кнопки
                    Expanded(
                      child: Container(
                        padding: const EdgeInsets.symmetric(horizontal: 20, vertical: 20),
                        child: body(),
                      ),
                    ),
                  ],
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }

  Column body() {
    return Column(
      children: [
        Expanded(
          child: Consumer(builder: (context, ref, child) {
            ref.watch(externalControlPrdr);
            if (buttonList.isEmpty) {
              return const Text("Кнопок нет...");
            } else {
              return ListView.builder(
                itemCount: buttonList.length,
                itemBuilder: (BuildContext context, int index) {
                  final button = buttonList[index];

                  return Container(
                    decoration: BoxDecoration(
                      border: Border.all(
                        color: currentIndex == index ? ColorControls.buttonBlue : Colors.grey,
                        width: 1.0,
                      ),
                      borderRadius: BorderRadius.circular(10.0),
                    ),
                    margin: const EdgeInsets.symmetric(vertical: 4.0),
                    child: ListTile(
                      title: Text(getChannel(button.channel, button.id)),
                      subtitle: Text(
                        'Одночный клик: ${getStrAction(button.action)} \nУдерживание ${button.delayTiming}мс: ${getStrAction(button.delayAction)} \nДвойной клик в течении ${button.dobleTiming}мс: ${getStrAction(button.doublAction)}',
                      ),
                      leading: MouseRegion(
                        cursor: SystemMouseCursors.click,
                        child: GestureDetector(
                          onTap: () {
                            // Обработка нажатия на иконку
                            currentIndex = index;
                            buttontSettings(context, index, button);
                          },
                          child: const Icon(Icons.settings),
                        ),
                      ),
                      trailing: MouseRegion(
                        cursor: SystemMouseCursors.click,
                        child: GestureDetector(
                          onTap: () {
                            currentIndex = 0;
                            ref.read(externalControlPrdr.notifier).deleteButton(index, button);
                          },
                          child: const Icon(Icons.delete),
                        ),
                      ),
                      onTap: () {
                        // Обработка нажатия на элемент списка
                        currentIndex = index;
                        ref.read(externalControlPrdr.notifier).btUpdate();
                      },
                    ),
                  );
                },
              );
            }
          }),
        ),
        const SizedBox(height: 16),
        Row(
          mainAxisAlignment: MainAxisAlignment.end,
          children: [
            SizedBox(
              width: 200,
              height: 35,
              child: Button(
                  onChange: (ClickInfo info) {
                    currentIndex = 0;
                    buttonList.clear();
                    transmitter("6 3~");
                    transmitter("6 3#");
                  },
                  lable: "Сбросить на стандартные",
                  lableSize: 13,
                  //clickInfo: ClickInfo(),
                  mode: BtuttonMode.standart,
                  buttonOnColor: ColorControls.buttonBlue),
            ),
            const SizedBox(width: 10),
            SizedBox(
              width: 200,
              height: 35,
              child: Button(
                  onChange: (ClickInfo info) {
                    currentIndex = 0;
                    buttonList.clear();
                    transmitter("6 3#");
                  },
                  lable: "Перезагрузить кнопки",
                  lableSize: 13,
                  //clickInfo: ClickInfo(),
                  mode: BtuttonMode.standart,
                  buttonOnColor: ColorControls.buttonBlue),
            ),
          ],
        ),
      ],
    );
  }
}
