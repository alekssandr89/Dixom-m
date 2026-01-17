import 'package:dixom/components/canvas/slider/slider_view.dart';
import 'package:dixom/components/utilities/responsive.dart';
import 'package:dixom/dataBase/app_constants.dart';
import 'package:dixom/main.dart';
import 'package:dixom/pages/audio/backend/sound_effect.dart';
import 'package:dixom/pages/audio/backend/source.dart';
import 'package:dixom/pages/home/sidebar.dart';
import 'package:dixom/pages/top_bar/top_bar.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

class Loudness extends StatelessWidget {
  const Loudness({super.key});

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
                child: Column(
                  children: [
                    const DixomAppBar(backState: false),
                    const SizedBox(height: 20),
                    Padding(
                      padding: const EdgeInsets.all(16.0),
                      child: SizedBox(
                        height: MediaQuery.of(context).size.height / 5, // Пятая экрана
                        child: const SingleChildScrollView(
                          child: Text(
                            "   Чувствительность человеческого слуха различна к звукам разной частоты, которые имеют одно и то же звуковое давление. Иными словами, звуки одинакового звукового давления, но разной частоты, субъективно воспринимаются человеком как звуки различной громкости. Наибольшая чувствительность слуха проявляется при частоте звука около 3 кГц. Падение чувствительности слуха при частотах менее и более 3 кГц тем больше, чем меньше звуковое давление. Запись музыкальных звуковых сигналов обычно осуществляется при уровне звукового давления 90‒92 дБ, при котором выставляется необходимый тональный баланс. В дальнейшем, при прослушивании в других условиях данного звукового сигнала с меньшим звуковым давлением, из-за изменения чувствительности слуха человека субъективно будет ощущаться недостаток высоких и низких частот. Для компенсации данного эффекта и применяется тонкомпенсация путём изменения частотных характеристик звука. Тонкомпенсация осуществляется, как правило, в соответствии с кривыми равной громкости Флетчера-Менсона.", // Пример длинного текста
                            style: TextStyle(fontSize: 16),
                          ),
                        ),
                      ),
                    ),
                    const SizedBox(height: 20),
                    const SingleChildScrollView(
                      scrollDirection: Axis.horizontal,
                      child: Row(
                        children: [
                          SizedBox(width: 20),
                          LoudnessBox(DspSource.usb),
                          SizedBox(width: 20),
                          LoudnessBox(DspSource.bluetooth),
                          SizedBox(width: 20),
                          LoudnessBox(DspSource.aux),
                          SizedBox(width: 20),
                          LoudnessBox(DspSource.fmRadio),
                          SizedBox(width: 20),
                          LoudnessBox(DspSource.spdif),
                          SizedBox(width: 20),
                        ],
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

  Consumer loudnessBox(DspSource interface) {
    return Consumer(builder: (context, ref, child) {
      Loundess loudness = ref.watch(audioEffectsPdr).loudness[interface.index];
      TextEditingController controllerLow = TextEditingController(text: loudness.freqL.toString());
      TextEditingController controllerHigh = TextEditingController(text: loudness.freqH.toString());
      FocusNode focusNodeLow = FocusNode();

      void enterText(String text) {
        //print(text);
      }

      // Listener для фокуса
      focusNodeLow.addListener(() {
        enterText(controllerLow.text); // Вызов функции при потеря фокуса
      });

      return Container(
        width: 240,
        height: 370,
        decoration: BoxDecoration(
          borderRadius: const BorderRadius.all(Radius.circular(10)),
          border: Border.all(
            color: const Color.fromARGB(255, 6, 243, 211),
          ),
        ),
        child: Column(
          children: [
            const SizedBox(height: 10),
            Text(interface.decoding),
            const SizedBox(height: 10),
            Row(
              children: [
                const SizedBox(width: 20),
                Expanded(
                  child: Column(
                    children: [
                      TextField(
                        decoration: const InputDecoration(
                          //  helperText: "Low",
                          suffix: Text("Гц"),
                          border: OutlineInputBorder(),
                        ),
                        controller: controllerLow,
                        focusNode: focusNodeLow,
                        textAlign: TextAlign.center,
                        keyboardType: TextInputType.number,
                        inputFormatters: [
                          FilteringTextInputFormatter.allow(RegExp(r'^\d*\.?\d{0,2}')),
                        ],
                      ),
                      const SizedBox(height: 20),
                      SizedBox(
                        width: 50,
                        height: 200,
                        child: SliderView(
                          active: true,
                          max: 60,
                          min: 0,
                          accuracy: 0,
                          notchesNum: 10,
                          shineColor: ColorDisplay.backgroundOn,
                          value: loudness.lvlL.toDouble(),
                          onChange: (double value) {
                            loudness.lvlL = value.toInt();
                            ref.read(audioEffectsPdr.notifier).loudness(interface, loudness, LoudnessParram.lvlLP);
                          },
                        ),
                      ),
                      const SizedBox(height: 10),
                      Text("${loudness.lvlL}"),
                      const SizedBox(height: 20),
                    ],
                  ),
                ),
                const SizedBox(width: 20),
                Expanded(
                  child: Column(
                    children: [
                      TextField(
                        decoration: const InputDecoration(
                          //  helperText: "High",
                          border: OutlineInputBorder(),
                          suffix: Text("Гц"),
                        ),
                        controller: controllerHigh,
                        textAlign: TextAlign.center,
                        keyboardType: TextInputType.number,
                        inputFormatters: [
                          FilteringTextInputFormatter.allow(RegExp(r'^\d*\.?\d{0,2}')),
                        ],
                      ),
                      const SizedBox(height: 20),
                      SizedBox(
                        width: 50,
                        height: 200,
                        child: SliderView(
                          active: true,
                          max: 60,
                          min: 0,
                          accuracy: 0,
                          notchesNum: 10,
                          shineColor: const Color.fromARGB(255, 56, 241, 0),
                          value: loudness.lvlH.toDouble(),
                          onChange: (double value) {
                            loudness.lvlH = value.toInt();
                            ref.read(audioEffectsPdr.notifier).loudness(interface, loudness, LoudnessParram.lvlHP);
                          },
                        ),
                      ),
                      const SizedBox(height: 10),
                      Text("${loudness.lvlH}"),
                      const SizedBox(height: 20),
                    ],
                  ),
                ),
                const SizedBox(width: 20),
              ],
            ),
          ],
        ),
      );
    });
  }
}

class LoudnessBox extends StatefulWidget {
  final DspSource interface;

  const LoudnessBox(this.interface, {super.key});

  @override
  LoudnessBoxState createState() => LoudnessBoxState();
}

class LoudnessBoxState extends State<LoudnessBox> {
  late TextEditingController controllerLow;
  late TextEditingController controllerHigh;

  late FocusNode focusNodeLow;
  late FocusNode focusNodeHigh;

  @override
  void initState() {
    super.initState();
    focusNodeLow = FocusNode();
    focusNodeHigh = FocusNode();
    // Инициализируем контроллер с текстом
    controllerLow = TextEditingController(
      text: appRef.read(audioEffectsPdr).loudness[widget.interface.index].freqL.toString(),
    );
    controllerHigh = TextEditingController(
      text: appRef.read(audioEffectsPdr).loudness[widget.interface.index].freqH.toString(),
    );

    // Добавляем слушатель для FocusNode
    focusNodeLow.addListener(_lowOnFocusChange);
    focusNodeHigh.addListener(_highOnFocusChange);
  }

  void _lowOnFocusChange() {
    enterText(true, controllerLow.text);
  }

  void _highOnFocusChange() {
    enterText(false, controllerHigh.text);
  }

  void enterText(bool isLow, String text) {
    Loundess loudness = appRef.watch(audioEffectsPdr).loudness[widget.interface.index];
    if (isLow) {
      loudness.freqL = int.parse(text);
      appRef.read(audioEffectsPdr.notifier).loudness(widget.interface, loudness, LoudnessParram.freqLP);
    } else {
      loudness.freqH = int.parse(text);
      appRef.read(audioEffectsPdr.notifier).loudness(widget.interface, loudness, LoudnessParram.freqHP);
    }
  }

  @override
  void dispose() {
    // Удаляем слушатель перед уничтожением виджета
    focusNodeLow.removeListener(_lowOnFocusChange);
    focusNodeLow.dispose();
    focusNodeHigh.removeListener(_highOnFocusChange);
    focusNodeHigh.dispose();
    controllerLow.dispose();
    controllerHigh.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Container(
      width: 240,
      height: 370,
      decoration: BoxDecoration(
        borderRadius: const BorderRadius.all(Radius.circular(10)),
        border: Border.all(
          color: const Color.fromARGB(255, 6, 243, 211),
        ),
      ),
      child: Column(
        children: [
          const SizedBox(height: 10),
          Text(widget.interface.decoding),
          const SizedBox(height: 10),
          Consumer(builder: (context, ref, child) {
            Loundess loudness = ref.watch(audioEffectsPdr).loudness[widget.interface.index];
            return Row(
              children: [
                const SizedBox(width: 20),
                Expanded(
                  child: Column(
                    children: [
                      TextField(
                        decoration: const InputDecoration(
                          //  helperText: "Low",
                          suffix: Text("Гц"),
                          border: OutlineInputBorder(),
                        ),
                        controller: controllerLow,
                        focusNode: focusNodeLow,
                        textAlign: TextAlign.center,
                        keyboardType: TextInputType.number,
                        inputFormatters: [
                          FilteringTextInputFormatter.allow(RegExp(r'^\d*\.?\d{0,2}')),
                        ],
                      ),
                      const SizedBox(height: 20),
                      SizedBox(
                        width: 50,
                        height: 200,
                        child: SliderView(
                          active: true,
                          max: 60,
                          min: 0,
                          accuracy: 0,
                          notchesNum: 10,
                          shineColor: ColorDisplay.backgroundOn,
                          value: loudness.lvlL.toDouble(),
                          onChange: (double value) {
                            loudness.lvlL = value.toInt();
                            ref
                                .read(audioEffectsPdr.notifier)
                                .loudness(widget.interface, loudness, LoudnessParram.lvlLP);
                          },
                        ),
                      ),
                      const SizedBox(height: 10),
                      Text("${loudness.lvlL}"),
                      const SizedBox(height: 20),
                    ],
                  ),
                ),
                const SizedBox(width: 20),
                Expanded(
                  child: Column(
                    children: [
                      TextField(
                        decoration: const InputDecoration(
                          //  helperText: "High",
                          border: OutlineInputBorder(),
                          suffix: Text("Гц"),
                        ),
                        controller: controllerHigh,
                        focusNode: focusNodeHigh,
                        textAlign: TextAlign.center,
                        keyboardType: TextInputType.number,
                        inputFormatters: [
                          FilteringTextInputFormatter.allow(RegExp(r'^\d*\.?\d{0,2}')),
                        ],
                      ),
                      const SizedBox(height: 20),
                      SizedBox(
                        width: 50,
                        height: 200,
                        child: SliderView(
                          active: true,
                          max: 60,
                          min: 0,
                          accuracy: 0,
                          notchesNum: 10,
                          shineColor: const Color.fromARGB(255, 56, 241, 0),
                          value: loudness.lvlH.toDouble(),
                          onChange: (double value) {
                            loudness.lvlH = value.toInt();
                            ref
                                .read(audioEffectsPdr.notifier)
                                .loudness(widget.interface, loudness, LoudnessParram.lvlHP);
                          },
                        ),
                      ),
                      const SizedBox(height: 10),
                      Text("${loudness.lvlH}"),
                      const SizedBox(height: 20),
                    ],
                  ),
                ),
                const SizedBox(width: 20),
              ],
            );
          }),
        ],
      ),
    );
  }
}
