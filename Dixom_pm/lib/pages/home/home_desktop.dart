import 'dart:math';

import 'package:dixom/components/canvas/button/button.dart';
import 'package:dixom/components/canvas/frame.dart';
import 'package:dixom/components/canvas/knop/knop_view.dart';
import 'package:dixom/components/canvas/level_detector/level_detector.dart';
import 'package:dixom/dataBase/app_constants.dart';
import 'package:dixom/driver/device_rx_tx/transmiter.dart';
import 'package:dixom/main.dart';
import 'package:dixom/pages/audio/backend/audio_out.dart';
import 'package:dixom/pages/audio/backend/sound.dart';
import 'package:dixom/pages/audio/backend/sound_enum.dart';
import 'package:dixom/pages/audio/backend/sound_settings.dart';
import 'package:dixom/pages/audio/backend/source.dart';
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

import 'home_mobile.dart';

Row desktopHome() {
  return Row(
    children: [
      Expanded(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.spaceBetween,
          children: [
            desktopVolumeSub("Сабвуфер"),
            desktopVolumeTweat("Твиттер"),
            //desktopVolumeControl2(),
            //desktopVolumeControl2(),
          ],
        ),
      ),
      Expanded(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.spaceBetween,
          children: [
            desktopVolumeControl(),

            const SizedBox(),
            Consumer(builder: (context, watch, child) {
              watch.watch(levelDetectorPrdr.select((value) => value.ldUpdate));
              DspSourceModel source = watch.read(deviceSourcePrdr);
              return Column(
                children: [
                  SizedBox(
                    height: 15,
                    width: 350,
                    child: LevelDetector(
                      lable: 'L',
                      valueMax: 255,
                      value: levlDetectorData[(source.source.index * 2) + 8],
                    ),
                  ),
                  const SizedBox(height: 6),
                  SizedBox(
                    height: 15,
                    width: 350,
                    child: LevelDetector(
                      lable: 'R',
                      valueMax: 255,
                      value: levlDetectorData[(source.source.index * 2) + 9],
                    ),
                  ),
                ],
              );
            }),
            desktopLoudnessButton(),
            desktopControlButton(),
            desktopSourceButton(),
          ],
        ),
      ),
    ],
  );
}

Stack desktopVolumeControl() {
  return Stack(
    children: [
      const SizedBox(
        width: 300,
        height: 300,
        child: Frame(
          text: "Громкость",
          steps: 1,
        ),
      ),
      Consumer(
        builder: (context, watch, child) {
          int myData = watch.watch(audioOutPdr.select((value) => value.volume[Volume.master.index]!.value));
          return SizedBox(
            width: 300,
            height: 300,
            child: KnopView(
              steps: 60,
              max: 60,
              notchesLength: 1,
              value: myData.toDouble(),
              onChange: (double value) {
                appRef.read(audioOutPdr.notifier).setVolumeOnDevice(Volume.master.index, value.toInt());
              },
              onDirection: (EncoderState value) {},
            ),
          );
        },
      ),
    ],
  );
}


Stack desktopVolumeSub(String lable) {
  return Stack(
    children: [
      SizedBox(
        width: 300,
        height: 300,
        child: Frame(
          text: lable,
          steps: 1,
        ),
      ),
      Consumer(
        builder: (context, watch, child) {
          AudioOutParam myData = watch.watch(audioOutPdr.select((value) => value.volume[Volume.ch1L_2R.index]!));
          return SizedBox(
            width: 300,
            height: 300,
            /*decoration: const BoxDecoration(
              image: DecorationImage(
                image: AssetImage('assets/images/png/background.png'), // путь к изображению
                fit: BoxFit.fill, // подгонка изображения
              ),
            ),*/
            // color: ColorPanel.background,
            child: KnopView(
              steps: 60,
              max: 60,
              notchesLength: 1,
              value: myData.value.toDouble(),
              onChange: (double value) {
                appRef.read(audioOutPdr.notifier).setVolumeOnDevice(Volume.ch1L_2R.index, value.toInt());
              },
              onDirection: (EncoderState value) {},
            ),
          );
        },
      ),
    ],
  );
}

Stack desktopVolumeTweat(String lable) {
  return Stack(
    children: [
      SizedBox(
        width: 300,
        height: 300,
        child: Frame(
          text: lable,
          steps: 1,
        ),
      ),
      Consumer(
        builder: (context, watch, child) {
          AudioOutParam myData = watch.watch(audioOutPdr.select((value) => value.volume[Volume.ch3L_4R.index]!));
          return SizedBox(
            width: 300,
            height: 300,
            /*decoration: const BoxDecoration(
              image: DecorationImage(
                image: AssetImage('assets/images/png/background.png'), // путь к изображению
                fit: BoxFit.fill, // подгонка изображения
              ),
            ),*/
            //  color: ColorPanel.background,
            child: KnopView(
              steps: 60,
              max: 60,
              notchesLength: 1,
              value: myData.value.toDouble(),
              onChange: (double value) {
                appRef.read(audioOutPdr.notifier).setVolumeOnDevice(Volume.ch3L_4R.index, value.toInt());
              },
              onDirection: (EncoderState value) {},
            ),
          );
        },
      ),
    ],
  );
}

Row desktopLoudnessButton() {
  return Row(
    mainAxisAlignment: MainAxisAlignment.center,
    children: [
      Consumer(builder: (context, watch, child) {
        int source = watch.watch(soundSettingsPrdr.select((value) => value.settings[SoundCfg.volumeControlMode.index]));
        return SizedBox(
          width: 240,
          height: 50,
          child: Button(
              onChange: (ClickInfo value) {
                appRef.read(soundSettingsPrdr.notifier).setLoudness();
              },
              lable: 'LOUDNESS',
              lableSize: 13,
              clickInfo: ClickInfo(state: (source == 2) ? true : false),
              mode: BtuttonMode.shine,
              buttonOnColor: ColorControls.buttonBlue),
        );
      }),
      const SizedBox(
        width: 10,
      ),
      Consumer(builder: (context, watch, child) {
        int source = watch.watch(soundSettingsPrdr.select((value) => value.settings[SoundCfg.levelDetector.index]));
        return SizedBox(
          width: 100,
          height: 50,
          child: Button(
              onChange: (ClickInfo value) {
                appRef.read(soundSettingsPrdr.notifier).setLevelDetector();
              },
              lable: 'LEVEL',
              lableSize: 13,
              clickInfo: ClickInfo(state: (source == 1) ? true : false),
              mode: BtuttonMode.shine,
              buttonOnColor: ColorControls.buttonBlue),
        );
      }),
    ],
  );
}

Row desktopControlButton() {
  return Row(
    mainAxisAlignment: MainAxisAlignment.center,
    children: [
      SizedBox(
        width: 85,
        height: 50,
        child: Button(
            onChange: (ClickInfo value) {
              transmitter("1 16=20");
            },
            lable: 'Назад',
            mode: BtuttonMode.standart,
            buttonOnColor: ColorControls.buttonBlue),
      ),
      const SizedBox(width: 30),
      SizedBox(
        width: 120,
        height: 50,
        child: Button(
            onChange: (ClickInfo value) {
              transmitter("1 16=18");
            },
            lable: 'Play/Pause',
            mode: BtuttonMode.standart,
            buttonOnColor: ColorControls.buttonBlue),
      ),
      const SizedBox(width: 30),
      SizedBox(
        width: 85,
        height: 50,
        child: Button(
            onChange: (ClickInfo value) {
              transmitter("1 16=19");
            },
            lable: 'Вперед',
            mode: BtuttonMode.standart,
            buttonOnColor: ColorControls.buttonBlue),
      ),
    ],
  );
}

Consumer desktopSourceButton() {
  return Consumer(builder: (context, watch, child) {
    DspSourceModel source = watch.watch(deviceSourcePrdr);

    source.source == DspSource.usb;
    return Column(
      children: [
        Row(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            sourceBtn(DspSource.usb, source),
            const SizedBox(width: 30),
            sourceBtn(DspSource.bluetooth, source),
            const SizedBox(width: 30),
            sourceBtn(DspSource.aux, source),
          ],
        ),
        const SizedBox(height: 16),
        Row(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            sourceBtn(DspSource.fmRadio, source),
            const SizedBox(width: 30),
            sourceBtn(DspSource.spdif, source),
            const SizedBox(width: 30),
            sourceBtn(DspSource.microphone, source),
          ],
        ),
      ],
    );
  });
}
