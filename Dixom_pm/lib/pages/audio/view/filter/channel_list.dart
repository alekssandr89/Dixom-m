import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

import '../../../../components/canvas/button/button.dart';
import '../../../../dataBase/app_constants.dart';
import '../../../top_bar/device_info/backend/dac_info.dart';
import '../../backend/filter.dart';
import '../../backend/sound.dart';

SingleChildScrollView channelist(int currentChannel) {
  return SingleChildScrollView(
    scrollDirection: Axis.horizontal,
    child: Padding(
      padding: const EdgeInsets.only(left: 16, right: 10),
      child: Consumer(builder: (context, ref, child) {
        DacInfoModel dacInfo = ref.watch(dacInfoPrdr);
        int channelCount = int.parse(dacInfo.numChannel) * int.parse(dacInfo.numChips);

        ref.watch(deviceSoundPrdr).isGrouped[currentChannel ~/ 2];
        return Wrap(
          spacing: 10,
          runSpacing: 10,
          children: [
            for (int i = 0; i < channelCount; i++)
              if (!ref.read(deviceSoundPrdr).isGrouped[i ~/ 2])
                //Если канал не сгруппирован то выводим всё подряд
                channelButton(i)
              else if (i % 2 == 0)
                //Если сгруппирован то вывыодим только не чётные каналы
                channelButton(i),
            channelButton(soundEqProvider.length - 2),
            if (!ref.read(deviceSoundPrdr).isGrouped[6]) channelButton(soundEqProvider.length - 1),
          ],
        );
      }),
    ),
  );
}

Consumer channelButton(int channel) {
  return Consumer(
    builder: (context, watch, child) {
      bool visibility = watch.watch(soundEqProvider[channel].select((value) => value.visibility));
      int currentChannel = watch.watch(deviceSoundPrdr).currentChannel;
      String channelName = watch.watch(soundEqProvider[channel].select((value) => value.channelName));
      String groupName = watch.watch(soundEqProvider[channel].select((value) => value.groupName));
      bool isGrouped = watch.read(deviceSoundPrdr).isGrouped[channel ~/ 2];

      String widthTextCalc = "";

      if (isGrouped) {
        widthTextCalc = groupName;
      } else {
        widthTextCalc = channelName;
      }
      final textSpan = TextSpan(text: widthTextCalc, style: const TextStyle(fontSize: 14));
      final textPainter = TextPainter(
        text: textSpan,
        textDirection: TextDirection.ltr,
      );
      textPainter.layout();
      double textWidth = textPainter.width;

      return SizedBox(
        width: textWidth + 30,
        height: 35,
        child: Button(
          onChange: (ClickInfo info) {
            if (info.type == TapType.long) {
              watch.read(soundEqProvider[channel].notifier).updateVisabylity();
            } else {
              watch
                  .read(deviceSoundPrdr.notifier)
                  .updateUi(channel, currentFilterType: watch.read(deviceSoundPrdr).currentFilterType);
            }
          },
          lable: isGrouped ? groupName : channelName,
          lableSize: 13,
          clickInfo: ClickInfo(state: channel == currentChannel || visibility),
          mode: BtuttonMode.shine,
          buttonOnColor: ColorDisplay.graphChannelLine[channel],
        ),
      );
    },
  );
}
