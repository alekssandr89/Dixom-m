import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

import '../../../../components/canvas/display/display_filter.dart';
import '../../../../dataBase/app_constants.dart';
import '../../../../main.dart';
import '../../backend/filter.dart';
import '../../backend/sound.dart';
import '../../backend/sound_enum.dart';

LayoutBuilder frequencyResponseGraph(double height, int currentChannel) {
  return LayoutBuilder(builder: (BuildContext context, BoxConstraints constraints) {
    return Row(
      children: [
        const SizedBox(width: 16),
        SizedBox(
          height: height,
          width: constraints.maxWidth - 32,
          child: Stack(
            children: [
              DisplayFilter(
                  lable: "",
                  filter: DspFilterModel(channelId: ChannelId.channel_1, groupId: GroupId.group_1),
                  drawingType: DrawingType.frame),
              for (var i = 0; i < soundEqProvider.length; i++)
                if (!appRef.read(deviceSoundPrdr).isGrouped[i ~/ 2])
                  //Если канал не сгруппирован то выводим всё подряд
                  Consumer(
                    builder: (context, ref, child) {
                      DspFilterModel filter = ref.watch(soundEqProvider[i]);
                      if (filter.visibility == true) {
                        return DisplayFilter(
                            lable: "",
                            filter: filter,
                            drawingType: DrawingType.graph,
                            color: ColorDisplay.graphChannelLine[i]);
                      } else {
                        return const SizedBox();
                      }
                    },
                  )
                else if (i % 2 == 0)
                  //Если сгруппирован то вывыодим только не чётные каналы
                  Consumer(
                    builder: (context, ref, child) {
                      DspFilterModel filter = ref.watch(soundEqProvider[i]);
                      if (filter.visibility == true) {
                        return DisplayFilter(
                            lable: "",
                            filter: filter,
                            drawingType: DrawingType.graph,
                            color: ColorDisplay.graphChannelLine[i]);
                      } else {
                        return const SizedBox();
                      }
                    },
                  ),
              Consumer(builder: (context, watch, child) {
                DspFilterModel filter = watch.watch(soundEqProvider[currentChannel]);
                return DisplayFilter(
                    lable: "",
                    filter: filter,
                    strokeWidth: 3,
                    drawingType: DrawingType.graph,
                    showCrossover: true,
                    color: ColorDisplay.graphChannelLine[currentChannel]);
              }),
            ],
          ),
        ),
      ],
    );
  });
}
