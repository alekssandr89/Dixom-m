import 'package:dixom/components/canvas/button/button.dart';
import 'package:dixom/components/canvas/slider/slider_view.dart';
import 'package:dixom/dataBase/app_constants.dart';
import 'package:dixom/pages/audio/backend/filter.dart';
import 'package:dixom/pages/audio/backend/sound.dart';
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

Widget gain(WidgetRef watch, int currentChannel, double hight) {
  return Column(
    children: [
      Text(
        watch.read(soundEqProvider[currentChannel]).gain.toString(),
        style: const TextStyle(color: Color.fromARGB(255, 190, 190, 190)),
      ),
      const SizedBox(height: 5),
      SizedBox(
        width: 50,
        height: hight,
        child: SliderView(
          active: true,
          max: 15,
          min: -20,
          accuracy: 0,
          shineColor: ColorDisplay.graphChannelLine[currentChannel],
          value: watch.read(soundEqProvider[currentChannel]).gain,
          onChange: (double value) {
            watch.read(soundEqProvider[currentChannel].notifier).updateGain(value);
          },
        ),
      ),
      const SizedBox(height: 10),
      const Text("Gain"),
      const SizedBox(height: 15),
    ],
  );
}

Column equalizer(DspFilterModel myData, int band, WidgetRef watch, int currentChannel, double hight) {
  CurrentFilterType type = watch.watch(deviceSoundPrdr).currentFilterType;

  String formatNumber(int number) {
    if (number >= 1000) {
      double num = number / 1000.0;
      return '${num.toStringAsFixed(num.truncateToDouble() == num ? 0 : 1)}k';
    }
    return number.toString();
  }

  return Column(
    children: [
      Text(
        formatNumber(myData.eq[band].freq),
        style: const TextStyle(color: Color.fromARGB(255, 190, 190, 190)),
      ),
      const SizedBox(height: 5),
      SizedBox(
        width: 41,
        height: hight,
        child: SliderView(
          active: type != CurrentFilterType.equalizer
              ? false
              : watch.read(soundEqProvider[currentChannel]).currentControl == band,
          max: 15,
          min: -20,
          accuracy: 0,
          shineColor: ColorDisplay.graphChannelLine[currentChannel],
          value: myData.eq[band].boost,
          onChange: (double value) {
            watch.read(soundEqProvider[currentChannel].notifier).updateUiEq(band, boost: value);
            watch.read(deviceSoundPrdr.notifier).updateUi(currentChannel);
          },
        ),
      ),
      const SizedBox(height: 10),
      SizedBox(
        width: 41,
        height: 25,
        child: Button(
            onChange: (ClickInfo info) {
              watch.read(deviceSoundPrdr.notifier).updateUi(currentChannel);
              if (info.state) {
                watch.read(soundEqProvider[currentChannel].notifier).updateUiEq(band, status: false);
              } else {
                watch.read(soundEqProvider[currentChannel].notifier).updateUiEq(band, status: true);
              }
            },
            lable: (band + 1).toString(),
            lableSize: 13,
            clickInfo: ClickInfo(state: myData.eq[band].status),
            mode: BtuttonMode.shine,
            buttonOnColor: ColorDisplay.graphChannelLine[currentChannel]),
      ),
      const SizedBox(height: 15),
    ],
  );
}
