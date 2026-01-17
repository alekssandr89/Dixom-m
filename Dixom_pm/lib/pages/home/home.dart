import 'package:dixom/components/canvas/button/button.dart';
import 'package:dixom/components/canvas/display/display.dart';
import 'package:dixom/components/canvas/knop/knop_view.dart';
import 'package:dixom/dataBase/app_constants.dart';
import 'package:dixom/main.dart';
import 'package:dixom/pages/home/home_desktop.dart';
import 'package:dixom/pages/home/home_mobile.dart';
import 'package:dixom/pages/top_bar/top_bar.dart';
import 'package:dixom/pages/home/sidebar.dart';
import 'package:dixom/components/utilities/responsive.dart';
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:shared_preferences/shared_preferences.dart';

import '../audio/backend/audio_out.dart';
import '../audio/backend/filter.dart';
import '../audio/backend/sound_enum.dart';
import '../audio/backend/sound_settings.dart';
import '../audio/backend/source.dart';

bool dbLoadState = true;

class Home extends StatelessWidget {
  const Home({super.key});

  //void initState() {}

  Future<void> initL() async {
    SharedPreferences pref = await SharedPreferences.getInstance();
    for (int i = 0; i < soundEqProvider.length; i++) {
      if (pref.getBool("$visabylityPref$i") != null) {
        bool? visabylity = pref.getBool("$visabylityPref$i")!;
        appRef.read(soundEqProvider[i].notifier).setVisabylity(visabylity);
      }
      //Загрузка сохранений из хранилища Имена каналов
      String idChannel = ChannelId.values[i].decoding;
      if (pref.getString(idChannel) != null) {
        String name = pref.getString(idChannel) as String;
        appRef.read(soundEqProvider[i].notifier).updateChannelName(name);
      }
      //Загрузка сохранений из хранилища Имена групп
      String idGroup = GroupId.values[(i ~/ 2)].decoding;
      if (pref.getString(idGroup) != null) {
        String name = pref.getString(idGroup) as String;
        appRef.read(soundEqProvider[i].notifier).updateGroupName(name);
      }
    }

    for (int i = 0; i < appRef.read(audioOutPdr).group.length; i++) {
      //Загрузка сохранений из хранилища группировки заежек
      if (pref.getInt("$channelGroupSave$i") != null) {
        int group = pref.getInt("$channelGroupSave$i") as int;
        appRef.read(audioOutPdr.notifier).loadGroup(i, group);
      }
    }
  }

  @override
  Widget build(BuildContext context) {
    if (dbLoadState) {
      initL();
      dbLoadState = false;
    }
    return Scaffold(
      drawer: const Menu(),
      body: SafeArea(
        child: Container(
          /*decoration: const BoxDecoration(
            image: DecorationImage(
              image: AssetImage('assets/images/png/background.png'), // путь к изображению
              fit: BoxFit.fill, // подгонка изображения
            ),
          ),*/
          color: ColorPanel.background,
          child: Row(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              if (Responsive.isDesktop(context)) const Expanded(child: Menu()),
              Expanded(
                flex: 4,
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.spaceBetween,
                  children: [
                    const DixomAppBar(backState: false),
                    Expanded(
                      child: Container(
                        padding: const EdgeInsets.symmetric(horizontal: 20, vertical: 20),
                        child: !Responsive.isMobile(context) ? desktopHome() : mobileHome(),
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
}

class AppDashboard extends StatefulWidget {
  const AppDashboard({
    super.key,
  });

  @override
  State<AppDashboard> createState() => _AppDashboardState();
}

class _AppDashboardState extends State<AppDashboard> {
  bool switchValue = false;

  bool btn4 = false;
  double slider1 = 10;

  @override
  Widget build(BuildContext context) {
    return LayoutBuilder(
      builder: (BuildContext context, BoxConstraints constraints) {
        return Column(
          mainAxisAlignment: MainAxisAlignment.spaceBetween,
          children: [
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceAround,
              children: [
                Consumer(
                  builder: (context, watch, child) {
                    AudioOutParam myData =
                        watch.watch(audioOutPdr.select((value) => value.volume[Volume.ch1L_2R.index]!));
                    return Container(
                      width: 250,
                      height: 250,
                      color: ColorPanel.background,
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
                Column(
                  mainAxisAlignment: MainAxisAlignment.start,
                  children: [
                    const SizedBox(height: 20),
                    SizedBox(
                      height: 200,
                      width: constraints.maxWidth / 2.5,
                      child: const Display(
                        lable: "",
                      ),
                    ),
                    const SizedBox(height: 20),
                    SizedBox(
                      width: constraints.maxWidth / 2.5,
                      child: Row(
                        mainAxisAlignment: MainAxisAlignment.spaceBetween,
                        children: [
                          SizedBox(
                            width: 90,
                            height: 40,
                            child: Button(
                                onChange: (ClickInfo value) {},
                                lable: 'A',
                                lableSize: 15,
                                clickInfo: const ClickInfo(state: false),
                                mode: BtuttonMode.standart,
                                buttonOnColor: ColorControls.buttonBlue),
                          ),
                          SizedBox(
                            width: 90,
                            height: 40,
                            child: Button(
                                onChange: (ClickInfo value) {},
                                lable: 'B',
                                lableSize: 15,
                                clickInfo: const ClickInfo(state: false),
                                mode: BtuttonMode.standart,
                                buttonOnColor: ColorControls.buttonBlue),
                          ),
                          SizedBox(
                            width: 90,
                            height: 40,
                            child: Button(
                                onChange: (ClickInfo value) {},
                                lable: 'C',
                                lableSize: 15,
                                clickInfo: const ClickInfo(state: false),
                                mode: BtuttonMode.standart,
                                buttonOnColor: ColorControls.buttonBlue),
                          ),
                          Consumer(builder: (context, watch, child) {
                            int source = watch.watch(
                                soundSettingsPrdr.select((value) => value.settings[SoundCfg.volumeControlMode.index]));
                            return SizedBox(
                              width: 90,
                              height: 40,
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
                        ],
                      ),
                    ),
                  ],
                ),
                Consumer(
                  builder: (context, watch, child) {
                    int myData = watch.watch(audioOutPdr.select((value) => value.volume[Volume.master.index]!.value));
                    return Container(
                      width: 250,
                      height: 250,
                      color: ColorPanel.background,
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
            ),
            SizedBox(
              width: constraints.maxWidth / 2,
              child: Row(
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                children: [
                  SizedBox(
                    width: 80,
                    height: 80,
                    child: Button(
                        onChange: (ClickInfo value) {
                          setState(() {
                            btn4 = value.state;
                          });
                        },
                        lable: 'Play',
                        clickInfo: ClickInfo(state: btn4),
                        mode: BtuttonMode.standart,
                        buttonOnColor: ColorControls.buttonBlue),
                  ),
                  SizedBox(
                    width: 80,
                    height: 80,
                    child: Button(
                        onChange: (ClickInfo value) {
                          setState(() {
                            btn4 = value.state;
                          });
                        },
                        lable: 'Pause',
                        clickInfo: ClickInfo(state: btn4),
                        mode: BtuttonMode.standart,
                        buttonOnColor: ColorControls.buttonBlue),
                  ),
                  SizedBox(
                    width: 80,
                    height: 80,
                    child: Button(
                        onChange: (ClickInfo value) {
                          setState(() {
                            btn4 = value.state;
                          });
                        },
                        lable: 'Next',
                        clickInfo: ClickInfo(state: btn4),
                        mode: BtuttonMode.standart,
                        buttonOnColor: ColorControls.buttonBlue),
                  ),
                  SizedBox(
                    width: 80,
                    height: 80,
                    child: Button(
                        onChange: (ClickInfo value) {
                          setState(() {
                            btn4 = value.state;
                          });
                        },
                        lable: 'Previus',
                        clickInfo: ClickInfo(state: btn4),
                        mode: BtuttonMode.standart,
                        buttonOnColor: ColorControls.buttonBlue),
                  ),
                ],
              ),
            ),
            Consumer(builder: (context, watch, child) {
              final source = watch.watch(deviceSourcePrdr);
              source.source == DspSource.usb;
              return Row(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  sourceBtn(DspSource.usb, source),
                  const SizedBox(width: 30),
                  sourceBtn(DspSource.bluetooth, source),
                  const SizedBox(width: 30),
                  sourceBtn(DspSource.aux, source),
                  const SizedBox(width: 30),
                  sourceBtn(DspSource.fmRadio, source),
                  const SizedBox(width: 30),
                  sourceBtn(DspSource.spdif, source),
                  const SizedBox(width: 30),
                  sourceBtn(DspSource.microphone, source),
                ],
              );
            }),
          ],
        );
      },
    );
  }
}
