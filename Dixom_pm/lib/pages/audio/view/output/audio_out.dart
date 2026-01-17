import 'dart:math';

import 'package:dixom/components/canvas/button/button.dart';
import 'package:dixom/main.dart';
import 'package:flutter/material.dart';

import 'package:dixom/components/canvas/knop/knop_view.dart';
import 'package:dixom/dataBase/app_constants.dart';
import 'package:dixom/pages/top_bar/top_bar.dart';
import 'package:dixom/pages/home/sidebar.dart';
import 'package:dixom/components/utilities/responsive.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

import '../../backend/audio_out.dart';
import '../../backend/filter.dart';
import '../../backend/sound_enum.dart';

double testVal = 0;
double testVal2 = 12;
double value = 0.0;
double sensitivity = 0.1; // Чувствительность вращения

int encoderResolution = 100;
double encoderValue = 0;

class VolumeAndDelay extends StatelessWidget {
  const VolumeAndDelay({super.key});

  Container paramer(String text) {
    return Container(
      width: 80,
      height: 25,
      margin: const EdgeInsets.symmetric(horizontal: 3, vertical: 2),
      padding: const EdgeInsets.symmetric(horizontal: 3, vertical: 0),
      decoration: BoxDecoration(borderRadius: BorderRadius.circular(3), color: ColorDisplay.backgroundOn),
      child: Center(
        child: Text(
          text,
          style: const TextStyle(color: ColorDisplay.lable, fontSize: 16),
        ),
      ),
    );
  }

  Consumer speaker(Volume ch) {
    return Consumer(builder: (context, ref, child) {
      AudioOutParam? myData = ref.watch(audioOutPdr.select((value) => value.volume[ch.index]));

      int volume = 0;
      int oldVolume = 0;
      int index = 0;
      bool isMute = false;

      if (myData is AudioOutParam) {
        volume = myData.value;
        oldVolume = myData.oldValue;
        index = myData.index;
        isMute = myData.isMute;
      }

      int group = ref.watch(audioOutPdr).group[index];

      bool phase = ref.watch(audioOutPdr.select((value) => value.phase[index]));
      double delay = ref.watch(audioOutPdr.select((value) => value.delay[index]));
      Volume currentChannel = ref.watch(audioOutControlPdr).currentControl;

      String channelName = ref.read(soundEqProvider[index].select((value) => value.channelName));

      return Column(
        children: [
          InkWell(
            onTap: () {
              if (isMute) {
                ref.read(audioOutPdr.notifier).setVolumeOnDevice(ch.index, oldVolume);
              } else {                
                ref.read(audioOutPdr.notifier).setVolumeOnDevice(ch.index, 0, isMute: true);
              }
            },
            child: Container(
              margin: const EdgeInsets.symmetric(horizontal: 3, vertical: 2),
              padding: const EdgeInsets.symmetric(horizontal: 3, vertical: 0),
              child: isMute
                  ? Image.asset(
                      'assets/images/png/speakerMute.png',
                      height: 70,
                    )
                  : Image.asset(
                      'assets/images/png/speaker.png',
                      height: 70,
                    ),
            ),
          ),
          const SizedBox(height: 20),
          InkWell(
            onTap: () {
              ref.read(audioOutControlPdr.notifier).setCurrentChannel(ch);
            },
            child: Container(
              height: 142,
              decoration: BoxDecoration(
                border: Border.all(
                  color: ch == currentChannel ? ColorControls.buttonBlue : ColorControls.buttonShineUp,
                  width: 2.0,
                ),
              ),
              child: Column(
                children: [
                  Text(channelName),
                  paramer("Группа $group"),
                  paramer(volume.toString()),
                  paramer("${delay.toStringAsFixed(2)} ms"),
                  paramer(phase ? "Обратная" : "Прямая"),
                ],
              ),
            ),
          ),
        ],
      );
    });
  }

  Container bitContainer(int maxBit) {
    return Container(
      padding: const EdgeInsets.all(2.0),
      height: 95,
      width: 230,
      child: Column(
        children: [
          const Text("Группировка каналов"),
          Consumer(builder: (context, ref, child) {
            Volume currentChannel = ref.watch(audioOutControlPdr).currentControl;
            AudioOutParam? myData = appRef.read(audioOutPdr.select((value) => value.volume[currentChannel.index]));
            int index = myData!.index;

            int group = ref.watch(audioOutPdr.select((value) => value.group[index]));
            return Center(
              child: Wrap(
                direction: Axis.horizontal,
                spacing: -4.0,
                children: [
                  for (int i = 0; i < maxBit; i++) bitWiget(ref, i, group == i ? true : false),
                ],
              ),
            );
          }),
        ],
      ),
    );
  }

  Column bitWiget(WidgetRef ref, int num, bool state) {
    return Column(
      children: [
        Checkbox(
          value: state,
          onChanged: (bool? newState) {
            Volume currentChannel = appRef.read(audioOutControlPdr).currentControl;
            AudioOutParam? myData = appRef.read(audioOutPdr.select((value) => value.volume[currentChannel.index]));
            int index = myData!.index;
            ref.read(audioOutPdr.notifier).setGroup(index, num);
          },
        ),
        num == 0 ? const Text("Нет") : Text(num.toString()),
      ],
    );
  }

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
              if (Responsive.isDesktop(context)) const Expanded(child: Menu()),
              Expanded(
                flex: 4,
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.start,
                  children: [
                    const DixomAppBar(backState: false),
                    Expanded(
                      child: Container(
                        padding: const EdgeInsets.symmetric(horizontal: 20, vertical: 20),
                        child: Responsive.isDesktop(context) || Responsive.isTablet(context)
                            ? Row(
                                children: [
                                  //channelListWithCar(),
                                  channelList(),
                                  desktopVolControl(),
                                ],
                              )
                            : Column(
                                children: [
                                  channelList(),
                                  mobileVolControl(),
                                ],
                              ),
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

  Expanded desktopVolControl() {
    return Expanded(
      flex: 5,
      child: Center(
        child: Container(
          color: ColorPanel.background,
          child: Column(
            mainAxisAlignment: MainAxisAlignment.spaceEvenly,
            children: [
              Row(
                mainAxisAlignment: MainAxisAlignment.spaceAround,
                children: [
                  bitContainer(5),
                  Consumer(builder: (context, ref, child) {
                    Volume currentChannel = ref.watch(audioOutControlPdr).currentControl;
                    AudioOutParam? myData = ref.read(audioOutPdr.select((value) => value.volume[currentChannel.index]));
                    int index = myData!.index;

                    bool phaseState = ref.watch(audioOutPdr.select((value) => value.phase[index]));
                    return SizedBox(
                      width: 60,
                      height: 40,
                      child: Button(
                          onChange: (ClickInfo info) {
                            ref.watch(audioOutPdr.notifier).setPhase(index);
                          },
                          lable: "Фаза",
                          lableSize: 13,
                          clickInfo: ClickInfo(state: phaseState),
                          mode: BtuttonMode.shine,
                          buttonOnColor: ColorControls.buttonBlue),
                    );
                  }),
                ],
              ),
              Row(
                mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                children: [
                  SizedBox(
                    width: 40,
                    height: 40,
                    child: Button(
                        onChange: (ClickInfo info) {
                          Volume currentChannel = appRef.read(audioOutControlPdr).currentControl;
                          AudioOutParam? myData =
                              appRef.read(audioOutPdr.select((value) => value.volume[currentChannel.index]));
                          int index = myData!.index;
                          double delay = appRef.read(audioOutPdr.select((value) => value.delay[index]));
                          delay = delay - 0.01;
                          appRef.read(audioOutPdr.notifier).setDelayOnDevice(index, delay);
                        },
                        lable: "-",
                        lableSize: 13,
                        mode: BtuttonMode.standart,
                        buttonOnColor: ColorControls.buttonBlue),
                  ),
                  const Text("Задержка"),
                  SizedBox(
                    width: 40,
                    height: 40,
                    child: Button(
                        onChange: (ClickInfo info) {
                          Volume currentChannel = appRef.read(audioOutControlPdr).currentControl;
                          AudioOutParam? myData =
                              appRef.read(audioOutPdr.select((value) => value.volume[currentChannel.index]));
                          int index = myData!.index;
                          double delay = appRef.read(audioOutPdr.select((value) => value.delay[index]));
                          delay = delay + 0.01;
                          appRef.read(audioOutPdr.notifier).setDelayOnDevice(index, delay);
                        },
                        lable: "+",
                        lableSize: 13,
                        mode: BtuttonMode.standart,
                        buttonOnColor: ColorControls.buttonBlue),
                  ),
                ],
              ),
              SizedBox(
                width: 250,
                height: 250,
                child: Stack(children: [
                  KnopView(
                    steps: 20,
                    controlMode: ControlMode.encoder,
                    minAngle: (0 * pi) / 180,
                    maxAngle: (360 * pi) / 180,
                    onChange: (double value) {},
                    onDirection: (EncoderState value) {
                      Volume currentChannel = appRef.read(audioOutControlPdr).currentControl;
                      AudioOutParam? myData =
                          appRef.read(audioOutPdr.select((value) => value.volume[currentChannel.index]));
                      int index = myData!.index;

                      double delay = appRef.read(audioOutPdr.select((value) => value.delay[index]));
                      if (value == EncoderState.left) {
                        delay -= 0.1;
                      } else if (value == EncoderState.right) {
                        delay += 0.1;
                      }
                      appRef.read(audioOutPdr.notifier).setDelayOnDevice(index, delay);
                    },
                  ),
                ]),
              ),
              const Text("Громкость"),
              Consumer(builder: (context, ref, child) {
                Volume currentChannel = ref.watch(audioOutControlPdr).currentControl;

                AudioOutParam? myData = ref.watch(audioOutPdr.select((value) => value.volume[currentChannel.index]));

                int volume = myData!.value;
                return SizedBox(
                  width: 250,
                  height: 250,
                  child: KnopView(
                    steps: 60,
                    max: 60,
                    notchesLength: 6,
                    value: volume.toDouble(),
                    controlMode: ControlMode.analogWithStop,
                    onChange: (double value) {
                      ref.read(audioOutPdr.notifier).setVolumeOnDevice(currentChannel.index, value.toInt());
                    },
                    onDirection: (EncoderState value) {},
                  ),
                );
              }),
            ],
          ),
        ),
      ),
    );
  }

  Expanded mobileVolControl() {
    return Expanded(
      flex: 4,
      child: Center(
        child: Container(
          color: ColorPanel.background,
          child: Column(
            mainAxisAlignment: MainAxisAlignment.spaceEvenly,
            children: [
              Row(
                mainAxisAlignment: MainAxisAlignment.spaceAround,
                children: [
                  SizedBox(
                    width: 40,
                    height: 40,
                    child: Button(
                        onChange: (ClickInfo info) {
                          Volume currentChannel = appRef.read(audioOutControlPdr).currentControl;
                          AudioOutParam? myData =
                              appRef.read(audioOutPdr.select((value) => value.volume[currentChannel.index]));
                          int index = myData!.index;
                          double delay = appRef.read(audioOutPdr.select((value) => value.delay[index]));
                          delay = delay - 0.01;
                          appRef.read(audioOutPdr.notifier).setDelayOnDevice(index, delay);
                        },
                        lable: "-",
                        lableSize: 13,
                        mode: BtuttonMode.standart,
                        buttonOnColor: ColorControls.buttonBlue),
                  ),
                  const SizedBox(width: 10),
                  SizedBox(
                    width: 40,
                    height: 40,
                    child: Button(
                        onChange: (ClickInfo info) {
                          Volume currentChannel = appRef.read(audioOutControlPdr).currentControl;
                          AudioOutParam? myData =
                              appRef.read(audioOutPdr.select((value) => value.volume[currentChannel.index]));
                          int index = myData!.index;
                          double delay = appRef.read(audioOutPdr.select((value) => value.delay[index]));
                          delay = delay + 0.01;
                          appRef.read(audioOutPdr.notifier).setDelayOnDevice(index, delay);
                        },
                        lable: "+",
                        lableSize: 13,
                        mode: BtuttonMode.standart,
                        buttonOnColor: ColorControls.buttonBlue),
                  ),
                  bitContainer(5),
                  Consumer(builder: (context, ref, child) {
                    Volume currentChannel = ref.watch(audioOutControlPdr).currentControl;
                    AudioOutParam? myData = ref.read(audioOutPdr.select((value) => value.volume[currentChannel.index]));
                    int index = myData!.index;

                    bool phaseState = ref.watch(audioOutPdr.select((value) => value.phase[index]));
                    return SizedBox(
                      width: 60,
                      height: 40,
                      child: Button(
                          onChange: (ClickInfo info) {
                            ref.watch(audioOutPdr.notifier).setPhase(index);
                          },
                          lable: "Фаза",
                          lableSize: 13,
                          clickInfo: ClickInfo(state: phaseState),
                          mode: BtuttonMode.shine,
                          buttonOnColor: ColorControls.buttonBlue),
                    );
                  }),
                ],
              ),
              Row(
                mainAxisAlignment: MainAxisAlignment.spaceAround,
                children: [
                  SizedBox(
                    width: 150,
                    height: 150,
                    child: Stack(children: [
                      KnopView(
                        steps: 20,
                        controlMode: ControlMode.encoder,
                        minAngle: (0 * pi) / 180,
                        maxAngle: (360 * pi) / 180,
                        onChange: (double value) {},
                        onDirection: (EncoderState value) {
                          Volume currentChannel = appRef.read(audioOutControlPdr).currentControl;
                          AudioOutParam? myData =
                              appRef.read(audioOutPdr.select((value) => value.volume[currentChannel.index]));
                          int index = myData!.index;

                          double delay = appRef.read(audioOutPdr.select((value) => value.delay[index]));
                          if (value == EncoderState.left) {
                            delay -= 0.1;
                          } else if (value == EncoderState.right) {
                            delay += 0.1;
                          }
                          appRef.read(audioOutPdr.notifier).setDelayOnDevice(index, delay);
                        },
                      ),
                    ]),
                  ),
                  Consumer(builder: (context, ref, child) {
                    Volume currentChannel = ref.watch(audioOutControlPdr).currentControl;

                    AudioOutParam? myData =
                        ref.watch(audioOutPdr.select((value) => value.volume[currentChannel.index]));

                    int volume = myData!.value;
                    return SizedBox(
                      width: 150,
                      height: 150,
                      child: KnopView(
                        steps: 60,
                        max: 60,
                        notchesLength: 6,
                        value: volume.toDouble(),
                        controlMode: ControlMode.analogWithStop,
                        onChange: (double value) {
                          ref.read(audioOutPdr.notifier).setVolumeOnDevice(currentChannel.index, value.toInt());
                        },
                        onDirection: (EncoderState value) {},
                      ),
                    );
                  }),
                ],
              ),
            ],
          ),
        ),
      ),
    );
  }

  Expanded channelListWithCar() {
    return Expanded(
      flex: 7,
      child: Stack(
        children: [
          Row(
            mainAxisAlignment: MainAxisAlignment.spaceBetween,
            children: [
              Column(
                mainAxisAlignment: MainAxisAlignment.spaceAround,
                children: [
                  speaker(Volume.ch3L),
                  speaker(Volume.ch5L),
                ],
              ),
              Column(
                mainAxisAlignment: MainAxisAlignment.start,
                children: [
                  speaker(Volume.ch1L),
                ],
              ),
              Column(
                mainAxisAlignment: MainAxisAlignment.end,
                children: [
                  speaker(Volume.ch7L),
                ],
              ),
              Column(
                mainAxisAlignment: MainAxisAlignment.end,
                children: [
                  speaker(Volume.ch8R),
                ],
              ),
              Column(
                mainAxisAlignment: MainAxisAlignment.start,
                children: [
                  speaker(Volume.ch2R),
                ],
              ),
              Column(
                mainAxisAlignment: MainAxisAlignment.spaceAround,
                children: [
                  speaker(Volume.ch4R),
                  speaker(Volume.ch6R),
                ],
              ),
            ],
          ),
          Center(
            child: Transform.translate(
              offset: const Offset(0, -50),
              child: Transform.rotate(
                angle: -0.5 * 3.14159,
                child: Image.asset(
                  'assets/images/png/car/white.png',
                  height: 150,
                ),
              ),
            ),
          ),
        ],
      ),
    );
  }

  Expanded channelList() {
    return Expanded(
      flex: 7,
      child: Column(
        mainAxisAlignment: MainAxisAlignment.spaceAround,
        children: [
          Row(
            mainAxisAlignment: MainAxisAlignment.spaceAround,
            children: [
              speaker(Volume.ch1L),
              speaker(Volume.ch2R),
              speaker(Volume.ch3L),
              speaker(Volume.ch4R),
            ],
          ),
          Row(
            mainAxisAlignment: MainAxisAlignment.spaceAround,
            children: [
              speaker(Volume.ch5L),
              speaker(Volume.ch6R),
              speaker(Volume.ch7L),
              speaker(Volume.ch8R),
            ],
          ),
        ],
      ),
    );
  }
}
