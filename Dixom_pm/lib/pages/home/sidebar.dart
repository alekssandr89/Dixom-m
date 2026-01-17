import 'package:dixom/components/canvas/button/button.dart';
import 'package:dixom/dataBase/app_constants.dart';
import 'package:dixom/dataBase/pages.dart';
import 'package:flutter/material.dart';

class Menu extends StatelessWidget {
  const Menu({super.key});

  @override
  Widget build(BuildContext context) {
    const btnPading = 20;
    return Drawer(
      backgroundColor: ColorPanel.background,
      child: LayoutBuilder(
        builder: (BuildContext context, BoxConstraints constraints) {
          return Center(
            child: SingleChildScrollView(
              physics: const BouncingScrollPhysics(),
              child: Column(
                children: [
                  const SizedBox(height: 30),
                  //const Text("DIXOM", style: TextStyle(fontSize: 20)),
                  const Text("МЕНЮ"),
                  const SizedBox(height: 30),
                  SizedBox(
                    width: constraints.maxWidth - btnPading,
                    height: 40,
                    child: Button(
                        onChange: (ClickInfo info) {
                          Navigator.pushNamed(context, AppPage.home);
                        },
                        lable: 'Дом',
                        // lableSize: 13,
                        clickInfo: const ClickInfo(state: false),
                        mode: BtuttonMode.standart,
                        buttonOnColor: ColorControls.buttonBlue),
                  ),
                  const SizedBox(height: 15),
                  SizedBox(
                    width: constraints.maxWidth - btnPading,
                    height: 40,
                    child: Button(
                        onChange: (ClickInfo info) {
                          Navigator.pushNamed(context, AppPage.myCar);
                        },
                        lable: 'Моя машина',
                        // lableSize: 13,
                        clickInfo: const ClickInfo(state: false),
                        mode: BtuttonMode.standart,
                        buttonOnColor: ColorControls.buttonBlue),
                  ),
                  const SizedBox(height: 15),
                  SizedBox(
                    width: constraints.maxWidth - btnPading,
                    height: 40,
                    child: Button(
                        onChange: (ClickInfo info) {
                          Navigator.pushNamed(context, AppPage.loudness);
                        },
                        lable: 'Тонкомпенсация',
                        // lableSize: 13,
                        clickInfo: const ClickInfo(state: false),
                        mode: BtuttonMode.standart,
                        buttonOnColor: ColorControls.buttonBlue),
                  ),
                  const SizedBox(height: 15),
                  SizedBox(
                    width: constraints.maxWidth - btnPading,
                    height: 40,
                    child: Button(
                        onChange: (ClickInfo info) {
                          Navigator.pushNamed(context, AppPage.volumeAndDelay);
                        },
                        lable: 'Аудио выходы',
                        // lableSize: 13,
                        clickInfo: const ClickInfo(state: false),
                        mode: BtuttonMode.standart,
                        buttonOnColor: ColorControls.buttonBlue),
                  ),
                  const SizedBox(height: 15),
                  SizedBox(
                    width: constraints.maxWidth - btnPading,
                    height: 40,
                    child: Button(
                        onChange: (ClickInfo info) {
                          Navigator.pushNamed(context, AppPage.channelFilter);
                        },
                        lable: 'Аудио фильтры',
                        // lableSize: 13,
                        clickInfo: const ClickInfo(state: false),
                        mode: BtuttonMode.standart,
                        buttonOnColor: ColorControls.buttonBlue),
                  ),
                  const SizedBox(height: 15),
                  SizedBox(
                    width: constraints.maxWidth - btnPading,
                    height: 40,
                    child: Button(
                        onChange: (ClickInfo info) {
                          Navigator.pushNamed(context, AppPage.externalControl);
                        },
                        lable: 'Кнопки и энкодеры',
                        // lableSize: 13,
                        clickInfo: const ClickInfo(state: false),
                        mode: BtuttonMode.standart,
                        buttonOnColor: ColorControls.buttonBlue),
                  ),
                  const SizedBox(height: 15),
                  SizedBox(
                    width: constraints.maxWidth - btnPading,
                    height: 40,
                    child: Button(
                        onChange: (ClickInfo info) {
                          Navigator.pushNamed(context, AppPage.canBus);
                        },
                        lable: 'Can шина',
                        // lableSize: 13,
                        clickInfo: const ClickInfo(state: false),
                        mode: BtuttonMode.standart,
                        buttonOnColor: ColorControls.buttonBlue),
                  ),
                  const SizedBox(height: 15),
                  SizedBox(
                    width: constraints.maxWidth - btnPading,
                    height: 40,
                    child: Button(
                        onChange: (ClickInfo info) {
                          Navigator.pushNamed(context, AppPage.fmRadio);
                        },
                        lable: 'Fm radio',
                        clickInfo: const ClickInfo(state: false),
                        mode: BtuttonMode.standart,
                        buttonOnColor: ColorControls.buttonBlue),
                  ),
                  const SizedBox(height: 15),
                  SizedBox(
                    width: constraints.maxWidth - btnPading,
                    height: 40,
                    child: Button(
                        onChange: (ClickInfo info) {
                          Navigator.pushNamed(context, AppPage.connection);
                        },
                        lable: 'Подключение',
                        // lableSize: 13,
                        clickInfo: const ClickInfo(state: false),
                        mode: BtuttonMode.standart,
                        buttonOnColor: ColorControls.buttonBlue),
                  ), 
                  const SizedBox(height: 15),
                  SizedBox(
                    width: constraints.maxWidth - btnPading,
                    height: 40,
                    child: Button(
                        onChange: (ClickInfo info) {
                          Navigator.pushNamed(context, AppPage.baseSettings);
                        },
                        lable: 'Настройки',
                        // lableSize: 13,
                        clickInfo: const ClickInfo(state: false),
                        mode: BtuttonMode.standart,
                        buttonOnColor: ColorControls.buttonBlue),
                  ),
                  const SizedBox(height: 15),
                  SizedBox(
                    width: constraints.maxWidth - btnPading,
                    height: 40,
                    child: Button(
                        onChange: (ClickInfo info) {
                          Navigator.pushNamed(context, AppPage.test);
                        },
                        lable: 'Для тестирвоания',
                        // lableSize: 13,
                        clickInfo: const ClickInfo(state: false),
                        mode: BtuttonMode.standart,
                        buttonOnColor: ColorControls.buttonBlue),
                  ),
                ],
              ),
            ),
          );
        },
      ),
    );
  }
}
