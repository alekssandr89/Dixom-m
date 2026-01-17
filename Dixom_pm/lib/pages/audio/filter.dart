import 'package:dixom/dataBase/app_constants.dart';
import 'package:dixom/pages/audio/view/filter/crossover.dart';
import 'package:dixom/pages/audio/view/filter/equalizer.dart';
import 'package:dixom/pages/audio/view/filter/config.dart';
import 'package:dixom/pages/top_bar/top_bar.dart';
import 'package:dixom/pages/home/sidebar.dart';
import 'package:dixom/components/utilities/responsive.dart';
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'backend/biquadratic.dart';
import 'backend/filter.dart';
import 'backend/sound.dart';
import 'view/filter/channel_list.dart';
import 'view/filter/graph.dart';
import 'view/filter/knop.dart';

class ChannelFilter extends StatelessWidget {
  const ChannelFilter({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      //Скрывающееся боковое меню
      drawer: const Menu(),
      body: SafeArea(
        child: Container(
          color: ColorPanel.background,
          child: Row(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              //Если это десктоп то боковое меню открыто всегда
              if (Responsive.isDesktop(context)) const Expanded(child: Menu()),
              Expanded(
                flex: 4,
                child: LayoutBuilder(builder: (BuildContext context, BoxConstraints constraints) {
                  double hight = (constraints.maxHeight - 370) / 2;
                  if (hight < 160) hight = 160;
                  return Column(
                    children: [
                      const DixomAppBar(backState: false),
                      Consumer(builder: (context, watch, child) {
                        final int currentChannel = watch.watch(deviceSoundPrdr.select((value) => value.currentChannel));

                        return Column(
                          children: [
                            frequencyResponseGraph(hight, currentChannel),
                            const SizedBox(height: 16),
                            channelist(currentChannel),
                            const SizedBox(height: 5),
                            FilterControl(currentChannel, watch, hight: hight),
                            EqSlider(currentChannel, hight: hight),
                          ],
                        );
                      }),
                    ],
                  );
                }),
              ),
            ],
          ),
        ),
      ),
    );
  }
}

class FilterControl extends StatelessWidget {
  final int currentChannel;
  final WidgetRef watch;
  final double hight;
  const FilterControl(this.currentChannel, this.watch, {super.key, required this.hight});
  @override
  Widget build(BuildContext context) {
    return SingleChildScrollView(
      scrollDirection: Axis.horizontal,
      child: Consumer(builder: (context, watch, child) {
        CurrentFilterType fType = watch.watch(deviceSoundPrdr).currentFilterType;
        return Column(children: [
          const SizedBox(height: 16),
          Row(
            mainAxisAlignment: MainAxisAlignment.end,
            children: [
              freqencyControl(watch, currentChannel, fType),
              const SizedBox(width: 20),
              Column(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  Consumer(builder: (context, watch, child) {
                    DspFilter myData = watch.watch(soundEqProvider[currentChannel].select((value) => value.hiPass));
                    return crossover(DspFilterType.highPass, myData, watch, currentChannel);
                  }),
                  const SizedBox(height: 5),
                  Consumer(builder: (context, watch, child) {
                    DspFilter myData = watch.watch(soundEqProvider[currentChannel].select((value) => value.lowPass));
                    return crossover(DspFilterType.lowPass, myData, watch, currentChannel);
                  }),
                ],
              ),
              const SizedBox(width: 20),
              boostControl(watch, currentChannel, fType),
              const SizedBox(width: 20),
              qControl(watch, currentChannel, fType),
            ],
          ),
          const SizedBox(height: 15),
        ]);
      }),
    );
  }
}

class EqSlider extends StatelessWidget {
  final int currentChannel;
  final double hight;

  const EqSlider(
    this.currentChannel, {
    super.key,
    required this.hight,
  });

  @override
  Widget build(BuildContext context) {
    return SingleChildScrollView(
      scrollDirection: Axis.horizontal,
      child: Consumer(
        builder: (context, watch, child) {
          final myData = watch.watch(soundEqProvider[currentChannel]);
          return Wrap(
            alignment: WrapAlignment.end,
            spacing: 10,
            runSpacing: 10,
            children: [
              const SizedBox(width: 8),
              filterConfig(context, watch, currentChannel, hight),
              const SizedBox(width: 1),
              gain(watch, currentChannel, hight),
              const SizedBox(width: 1),
              for (int i = 0; i <= 14; i++) equalizer(myData, i, watch, currentChannel, hight),
              const SizedBox(width: 20),
            ],
          );
        },
      ),
    );
  }
}
