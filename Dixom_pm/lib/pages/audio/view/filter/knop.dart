import 'dart:math';

import 'package:dixom/components/canvas/knop/knop_view.dart';
import 'package:dixom/dataBase/app_constants.dart';
import 'package:dixom/pages/audio/backend/filter.dart';
import 'package:dixom/pages/audio/backend/sound.dart';
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

Widget freqencyControl(WidgetRef watch, int currentChannel, CurrentFilterType fType) {
  return SizedBox(
    width: 150,
    height: 150,
    child: Stack(children: [
      KnopView(
        minAngle: (0 * pi) / 180,
        maxAngle: (360 * pi) / 180,
        steps: 20,
        controlMode: ControlMode.encoder,
        onChange: (double value) {},
        onDirection: (EncoderState value) {
          // Calculate the logarithmic coefficient based on the minimum and maximum values
          int band = watch.read(soundEqProvider[currentChannel]).currentControl;
          DspFilterModel myData = watch.read(soundEqProvider[currentChannel]);

          int freq = 0;

          if (fType == CurrentFilterType.equalizer) {
            freq = myData.eq[myData.currentControl].freq;
          } else if (fType == CurrentFilterType.hiPass) {
            freq = watch.read(soundEqProvider[currentChannel]).hiPass.freq;
          } else if (fType == CurrentFilterType.lowPass) {
            freq = watch.read(soundEqProvider[currentChannel]).lowPass.freq;
          }
          int logCoefficient = freq;

          if (freq < 100) {
            logCoefficient = 1;
          } else if (freq < 1000) {
            logCoefficient = 5;
          } else if (freq < 10000) {
            logCoefficient = 50;
          } else {
            logCoefficient = 500;
          }

          // Update freq based on the direction
          if (value == EncoderState.left) {
            freq -= logCoefficient;
          } else if (value == EncoderState.right) {
            freq += logCoefficient;
          }

          CurrentFilterType type = watch.read(deviceSoundPrdr).currentFilterType;

          if (type == CurrentFilterType.equalizer) {
            watch.read(soundEqProvider[currentChannel].notifier).updateUiEq(band, freq: freq);
          } else if (type == CurrentFilterType.hiPass) {
            watch
                .read(soundEqProvider[currentChannel].notifier)
                .updateUiCrossover(watch.read(soundEqProvider[currentChannel]).hiPass.type, freq: freq);
          } else if (type == CurrentFilterType.lowPass) {
            watch
                .read(soundEqProvider[currentChannel].notifier)
                .updateUiCrossover(watch.read(soundEqProvider[currentChannel]).lowPass.type, freq: freq);
          }
        },
      ),
      Center(
        child: Container(
          width: 45,
          height: 45,
          margin: const EdgeInsets.symmetric(horizontal: 3),
          padding: const EdgeInsets.symmetric(horizontal: 3, vertical: 2),
          decoration: BoxDecoration(borderRadius: BorderRadius.circular(50), color: ColorDisplay.backgroundOn),
          child: Consumer(builder: (context, watch, child) {
            DspFilterModel myData = watch.watch(soundEqProvider[currentChannel]);
            int freq = 0;
            if (fType == CurrentFilterType.equalizer) {
              freq = myData.eq[myData.currentControl].freq;
            } else if (fType == CurrentFilterType.hiPass) {
              freq = watch.watch(soundEqProvider[currentChannel]).hiPass.freq;
            } else if (fType == CurrentFilterType.lowPass) {
              freq = watch.watch(soundEqProvider[currentChannel]).lowPass.freq;
            }
            return Center(
              child: Text(
                freq.toString(),
                style: const TextStyle(color: ColorDisplay.lable, fontSize: 13),
              ),
            );
          }),
        ),
      ),
    ]),
  );
}

Widget boostControl(WidgetRef watch, int currentChannel, CurrentFilterType fType) {
  return SizedBox(
    width: 150,
    height: 150,
    child: Stack(
      children: [
        KnopView(
          minAngle: (0 * pi) / 180,
          maxAngle: (360 * pi) / 180,
          steps: 40,
          controlMode: ControlMode.encoder,
          onChange: (double value) {},
          onDirection: (EncoderState value) {
            int band = watch.read(soundEqProvider[currentChannel]).currentControl;
            DspFilterModel myData = watch.read(soundEqProvider[currentChannel]);
            double boost = myData.eq[myData.currentControl].boost;
            if (value == EncoderState.left) {
              boost -= 0.01;
            } else if (value == EncoderState.right) {
              boost += 0.01;
            }
            if (fType == CurrentFilterType.equalizer) {
              watch.read(soundEqProvider[currentChannel].notifier).updateUiEq(band, boost: boost);
            }
          },
        ),
        Center(
          child: Container(
            width: 45,
            height: 45,
            margin: const EdgeInsets.symmetric(horizontal: 3),
            padding: const EdgeInsets.symmetric(horizontal: 3, vertical: 2),
            decoration: BoxDecoration(borderRadius: BorderRadius.circular(50), color: ColorDisplay.backgroundOn),
            child: Consumer(
              builder: (context, watch, child) {
                DspFilterModel myData = watch.watch(soundEqProvider[currentChannel]);
                double boost = myData.eq[myData.currentControl].boost;
                return Center(
                  child: fType == CurrentFilterType.equalizer
                      ? Text(
                          boost.toStringAsFixed(2),
                          style: const TextStyle(color: ColorDisplay.lable, fontSize: 13),
                        )
                      : const Text(""),
                );
              },
            ),
          ),
        ),
      ],
    ),
  );
}

Widget qControl(WidgetRef watch, int currentChannel, CurrentFilterType fType) {
  return SizedBox(
    width: 150,
    height: 150,
    child: Stack(children: [
      KnopView(
        minAngle: (0 * pi) / 180,
        maxAngle: (360 * pi) / 180,
        steps: 20,
        controlMode: ControlMode.encoder,
        onChange: (double value) {},
        onDirection: (EncoderState value) {
          int band = watch.read(soundEqProvider[currentChannel]).currentControl;
          DspFilterModel myData = watch.read(soundEqProvider[currentChannel]);
          double q = myData.eq[myData.currentControl].q;
          if (value == EncoderState.left) {
            q -= 0.1;
          } else if (value == EncoderState.right) {
            q += 0.1;
          }
          if (fType == CurrentFilterType.equalizer) {
            watch.read(soundEqProvider[currentChannel].notifier).updateUiEq(band, q: q);
          }
        },
      ),
      Center(
        child: Container(
          width: 45,
          height: 45,
          margin: const EdgeInsets.symmetric(horizontal: 3),
          padding: const EdgeInsets.symmetric(horizontal: 3, vertical: 2),
          decoration: BoxDecoration(borderRadius: BorderRadius.circular(50), color: ColorDisplay.backgroundOn),
          child: Consumer(builder: (context, watch, child) {
            DspFilterModel myData = watch.watch(soundEqProvider[currentChannel]);
            double q = myData.eq[myData.currentControl].q;
            return Center(
              child: fType == CurrentFilterType.equalizer
                  ? Text(
                      q.toStringAsFixed(1),
                      style: const TextStyle(color: ColorDisplay.lable, fontSize: 13),
                    )
                  : const Text(""),
            );
          }),
        ),
      ),
    ]),
  );
}
