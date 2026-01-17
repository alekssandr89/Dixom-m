import 'package:dixom/components/canvas/button/button.dart';
import 'package:dixom/components/widgets/dialog.dart';
import 'package:dixom/dataBase/app_constants.dart';
import 'package:dixom/pages/audio/backend/filter.dart';
import 'package:dixom/pages/audio/view/filter/overlay.dart';
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

Container filterConfig(BuildContext context, WidgetRef watch, int currentChannel, double hight) {
  return Container(
    width: 105,
    height: hight + 60,
    padding: const EdgeInsets.all(2.0),
    decoration: BoxDecoration(
      borderRadius: const BorderRadius.all(Radius.circular(10)),
      border: Border.all(
        color: ColorControls.buttonShineUp,
      ),
    ), //             <--- BoxDecoration here
    child: Column(
      mainAxisAlignment: MainAxisAlignment.spaceAround,
      children: [
        Consumer(builder: (context, watch, child) {
          bool myData = watch.watch(soundEqProvider[currentChannel].select((value) => value.bypass));
          return SizedBox(
            width: 90,
            height: 50,
            child: Button(
                onChange: (ClickInfo info) {
                  watch.read(soundEqProvider[currentChannel].notifier).updateState();
                },
                lable: "EQ ",
                lableSize: 13,
                clickInfo: ClickInfo(state: myData),
                mode: BtuttonMode.shine,
                buttonOnColor: ColorDisplay.graphChannelLine[currentChannel]),
          );
        }),
        SizedBox(
          width: 90,
          height: 50,
          child: Button(
              onChange: (ClickInfo info) {
                if (overlayState == false) {
                  overlayEntry = createOverlayEntry();
                  Overlay.of(context).insert(overlayEntry);
                  overlayState = true;
                } else {
                  overlayEntry.remove();
                  overlayState = false;
                }
              },
              lable: "Настройка",
              lableSize: 13,
              //clickInfo: ClickInfo(state: myData.eq[band].status),
              mode: BtuttonMode.standart,
              buttonOnColor: ColorDisplay.graphChannelLine[currentChannel]),
        ),
        Container(
          width: 95,
          height: 110,
          padding: const EdgeInsets.all(2.0),
          decoration: BoxDecoration(
            borderRadius: const BorderRadius.all(Radius.circular(10)),
            border: Border.all(
              color: ColorControls.buttonShineUp,
            ),
          ),
          child: Column(
            children: [
              const Text("Сброс"),
              const SizedBox(height: 6),
              SizedBox(
                width: 75,
                height: 30,
                child: Button(
                    onChange: (ClickInfo info) async {
                      if (await confirmationDialog(context,
                              description: "Настройки эквалайзера выбранного канала будут сброшены на стандартные") ==
                          true) {
                        watch.read(soundEqProvider[currentChannel].notifier).defaultEq();
                      }
                    },
                    lable: "EQ",
                    lableSize: 13,
                    //clickInfo: ClickInfo(state: myData.eq[band].status),
                    mode: BtuttonMode.standart,
                    buttonOnColor: ColorDisplay.graphChannelLine[currentChannel]),
              ),
              const SizedBox(height: 10),
              SizedBox(
                width: 75,
                height: 30,
                child: Button(
                    onChange: (ClickInfo info) async {
                      if (await confirmationDialog(context,
                              description:
                                  "Настройки срезов кроссовера выбранного канала будут сброшены на стандартные") ==
                          true) {
                        watch.read(soundEqProvider[currentChannel].notifier).defaultCrossover();
                      } else {}
                    },
                    lable: "Срезы",
                    lableSize: 13,
                    //clickInfo: ClickInfo(state: myData.eq[band].status),
                    mode: BtuttonMode.standart,
                    buttonOnColor: ColorDisplay.graphChannelLine[currentChannel]),
              ),
            ],
          ),
        ),
      ],
    ),
  );
}
