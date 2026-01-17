import 'dart:math';

import 'package:dixom/components/canvas/button/button.dart';
import 'package:dixom/components/canvas/knop/knop_view.dart';
import 'package:dixom/components/canvas/slider/slider_view.dart';
import 'package:flutter/material.dart';
import 'package:dixom/dataBase/app_constants.dart';
import 'package:dixom/pages/home/sidebar.dart';
import 'package:dixom/components/utilities/responsive.dart';


class TestPage extends StatelessWidget {
  const TestPage({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      drawer: const Menu(),
      body: SafeArea(
        child: Container(
          color: ColorPanel.background,
          child: Row(
            crossAxisAlignment: CrossAxisAlignment.center,
            children: [
              if (Responsive.isDesktop(context)) const Expanded(child: Menu()),
              Expanded(
                flex: 4,
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    //const DixomAppBar(backState: false),
                    /* Expanded(
                      child: Container(
                        padding: const EdgeInsets.symmetric(horizontal: 20, vertical: 20),
                        child: const SqlMain(),
                      ),
                    ),*/
                    SizedBox(
                      width: 50,
                      height: 250,
                      child: SliderView(
                        active: true,
                        max: 15,
                        min: -20,
                        accuracy: 0,
                        shineColor: ColorDisplay.graphChannelLine[0],
                        value: 0,
                        onChange: (double value) {},
                      ),
                    ),
                    const SizedBox(width: 70),
                    Container(
                      width: 250,
                      height: 250,
                      color: ColorPanel.background,
                      child: KnopView(
                        minAngle: (0 * pi) / 180,
                        maxAngle: (360 * pi) / 180,
                        notchesLength: 2,
                        controlMode: ControlMode.encoder,
                        value: 100,
                        onChange: (double value) {},
                        onDirection: (EncoderState value) {},
                      ),
                    ),
                    const SizedBox(width: 70),
                    Container(
                      width: 250,
                      height: 250,
                      color: ColorPanel.background,
                      child: KnopView(
                        steps: 60,
                        max: 60,
                        notchesLength: 1,
                        value: 20,
                        onChange: (double value) {},
                        onDirection: (EncoderState value) {},
                      ),
                    ),
                    const SizedBox(width: 60),
                    Column(
                      // crossAxisAlignment: CrossAxisAlignment.center,
                      mainAxisAlignment: MainAxisAlignment.center,
                      children: [
                        SizedBox(
                          width: 90,
                          height: 40,
                          child: Button(
                              onChange: (ClickInfo value) {
                                // appRef.read(soundSettingsPrdr.notifier).setLoudness();
                              },
                              lable: 'LOUDNESS',
                              lableSize: 13,
                              clickInfo: const ClickInfo(state: true),
                              mode: BtuttonMode.standart,
                              buttonOnColor: ColorControls.buttonBlue),
                        ),
                        const SizedBox(height: 20),
                        SizedBox(
                          width: 90,
                          height: 40,
                          child: Button(
                              onChange: (ClickInfo value) {
                                // appRef.read(soundSettingsPrdr.notifier).setLoudness();
                              },
                              lable: 'LOUDNESS',
                              lableSize: 13,
                              clickInfo: const ClickInfo(state: false),
                              mode: BtuttonMode.shine,
                              buttonOnColor: ColorControls.buttonBlue),
                        ),
                        const SizedBox(height: 20),
                        SizedBox(
                          width: 90,
                          height: 40,
                          child: Button(
                              onChange: (ClickInfo value) {
                                // appRef.read(soundSettingsPrdr.notifier).setLoudness();
                              },
                              lable: 'LOUDNESS',
                              lableSize: 13,
                              clickInfo: const ClickInfo(state: true),
                              mode: BtuttonMode.shine,
                              buttonOnColor: ColorControls.buttonBlue),
                        ),
                      ],
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
