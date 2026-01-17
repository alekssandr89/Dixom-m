import 'package:dixom/components/canvas/button/button.dart';
import 'package:dixom/pages/fm_radio/backend/radio_provider.dart';
import 'package:dixom/pages/fm_radio/canvas/radio_frequency_bar.dart';
import 'package:flutter/material.dart';
import 'package:dixom/dataBase/app_constants.dart';
import 'package:dixom/pages/top_bar/top_bar.dart';
import 'package:dixom/pages/home/sidebar.dart';
import 'package:dixom/components/utilities/responsive.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

class FmRadioPage extends StatelessWidget {
  const FmRadioPage({super.key});

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
                    Column(
                      children: [
                        Container(
                          padding: const EdgeInsets.symmetric(horizontal: 15, vertical: 20),
                          child: Consumer(builder: (context, ref, child) {
                            double freq = ref.watch(fmRadioPrdr.select((value) => value.freqency));
                            return Column(
                              mainAxisAlignment: MainAxisAlignment.center,
                              children: [
                                SizedBox(
                                  height: 70,
                                  child: RadioFrequencyBar(
                                    value: freq,
                                    onChange: (double value) {
                                      ref.read(fmRadioPrdr.notifier).setFreqency(value);
                                    },
                                  ),
                                ),
                                const SizedBox(height: 15),
                                Row(
                                  mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                                  children: [
                                    SizedBox(
                                      width: 90,
                                      height: 90,
                                      child: Button(
                                        onChange: (ClickInfo value) {
                                          ref.read(fmRadioPrdr.notifier).radioAutoSearchDown();
                                        },
                                        lable: '<<',
                                        lableSize: 60,
                                        clickInfo: const ClickInfo(state: false),
                                        mode: BtuttonMode.standart,
                                      ),
                                    ),
                                    SizedBox(
                                      width: 90,
                                      height: 90,
                                      child: Button(
                                        onChange: (ClickInfo value) {
                                          ref.read(fmRadioPrdr.notifier).radioManualSearchDown();
                                        },
                                        lable: '<',
                                        lableSize: 60,
                                        clickInfo: const ClickInfo(state: false),
                                        mode: BtuttonMode.standart,
                                      ),
                                    ),
                                    Text(
                                      freq.toStringAsFixed(2),
                                      style: const TextStyle(fontSize: 120),
                                    ),
                                    SizedBox(
                                      width: 90,
                                      height: 90,
                                      child: Button(
                                        onChange: (ClickInfo value) {
                                          ref.read(fmRadioPrdr.notifier).radioManualSearchUp();
                                        },
                                        lable: '>',
                                        lableSize: 60,
                                        clickInfo: const ClickInfo(state: false),
                                        mode: BtuttonMode.standart,
                                      ),
                                    ),
                                    SizedBox(
                                      width: 90,
                                      height: 90,
                                      child: Button(
                                        onChange: (ClickInfo value) {
                                          ref.read(fmRadioPrdr.notifier).radioAutoSearchUp();
                                        },
                                        lable: '>>',
                                        lableSize: 60,
                                        clickInfo: const ClickInfo(state: false),
                                        mode: BtuttonMode.standart,
                                      ),
                                    ),
                                  ],
                                ),
                                const SizedBox(height: 20),
                                Row(
                                  mainAxisAlignment: MainAxisAlignment.center,
                                  children: [
                                    SizedBox(
                                      width: 120,
                                      height: 40,
                                      child: Button(
                                        onChange: (ClickInfo value) {},
                                        lable: '105.1',
                                        lableSize: 20,
                                        clickInfo: const ClickInfo(state: true),
                                        mode: BtuttonMode.shine,
                                      ),
                                    ),
                                    const SizedBox(width: 20),
                                    SizedBox(
                                      width: 120,
                                      height: 40,
                                      child: Button(
                                        onChange: (ClickInfo value) {},
                                        lable: '107.0',
                                        lableSize: 20,
                                        clickInfo: const ClickInfo(state: false),
                                        mode: BtuttonMode.shine,
                                      ),
                                    ),
                                    const SizedBox(width: 20),
                                    SizedBox(
                                      width: 120,
                                      height: 40,
                                      child: Button(
                                        onChange: (ClickInfo value) {},
                                        lable: '80.1',
                                        lableSize: 20,
                                        clickInfo: const ClickInfo(state: false),
                                        mode: BtuttonMode.shine,
                                      ),
                                    ),
                                  ],
                                ),
                                const SizedBox(height: 20),
                                Row(
                                  mainAxisAlignment: MainAxisAlignment.center,
                                  children: [
                                    SizedBox(
                                      width: 120,
                                      height: 40,
                                      child: Button(
                                        onChange: (ClickInfo value) {},
                                        lable: '78.5',
                                        lableSize: 20,
                                        clickInfo: const ClickInfo(state: false),
                                        mode: BtuttonMode.shine,
                                      ),
                                    ),
                                    const SizedBox(width: 20),
                                    SizedBox(
                                      width: 120,
                                      height: 40,
                                      child: Button(
                                        onChange: (ClickInfo value) {},
                                        lable: '100.0',
                                        lableSize: 20,
                                        clickInfo: const ClickInfo(state: false),
                                        mode: BtuttonMode.shine,
                                      ),
                                    ),
                                    const SizedBox(width: 20),
                                    SizedBox(
                                      width: 120,
                                      height: 40,
                                      child: Button(
                                        onChange: (ClickInfo value) {},
                                        lable: '108.0',
                                        lableSize: 20,
                                        clickInfo: const ClickInfo(state: false),
                                        mode: BtuttonMode.shine,
                                      ),
                                    ),
                                  ],
                                )
                              ],
                            );
                          }),
                        ),
                        const SizedBox(height: 50),
                        Consumer(builder: (context, ref, child) {
                          int stereo = ref.watch(fmRadioPrdr.select((value) => value.stereo));
                          int rssi = ref.watch(fmRadioPrdr.select((value) => value.rssi));
                          int snr = ref.watch(fmRadioPrdr.select((value) => value.snr));
                          return Row(
                            mainAxisAlignment: MainAxisAlignment.center,
                            children: [
                              if (stereo == 1) const Text("Стерео"),
                              if (stereo == 0) const Text("Моно"),
                              const SizedBox(width: 30),
                              Text("Мощность RSSI - $rssi"),
                              const SizedBox(width: 30),
                              Text("Сигнал шум SNR - $snr"),
                            ],
                          );
                        }),
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
