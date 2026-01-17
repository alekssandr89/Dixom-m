import 'package:dixom/components/canvas/button/button.dart';
import 'package:dixom/dataBase/app_constants.dart';
import 'package:dixom/pages/audio/backend/filter.dart';
import 'package:dixom/pages/audio/backend/sound.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import '../../backend/biquadratic.dart';

Container crossover(DspFilterType fType, DspFilter myData, WidgetRef watch, int currentChannel) {
  CurrentFilterType type = watch.watch(deviceSoundPrdr).currentFilterType;

  Color border = ColorControls.buttonShineUp;
  if (fType == DspFilterType.lowPass) {
    if (type == CurrentFilterType.lowPass) {
      border = ColorDisplay.graphChannelLine[currentChannel];
    }
  } else {
    if (type == CurrentFilterType.hiPass) {
      border = ColorDisplay.graphChannelLine[currentChannel];
    }
  }
  return Container(
    width: 430,
    height: 78,
    //margin: const EdgeInsets.all(20.0),
    padding: const EdgeInsets.all(2.0),
    decoration: BoxDecoration(
      borderRadius: const BorderRadius.all(Radius.circular(10)),
      border: Border.all(
        color: border,
      ),
    ), //             <--- BoxDecoration here
    child: Row(
      mainAxisAlignment: MainAxisAlignment.spaceAround,
      children: [
        crossoverFreqencyEditing(fType, myData.freq, watch, currentChannel),
        crossoverOrderEditing(fType, myData.order, watch, currentChannel),
        SizedBox(
          width: 80,
          height: 60,
          child: Button(
              onChange: (ClickInfo info) {
                watch
                    .watch(soundEqProvider[currentChannel].notifier)
                    .updateUiCrossover(fType, status: myData.status == true ? false : true);
              },
              lable: fType == DspFilterType.lowPass ? "Low-Pass" : "Hi-Pass",
              lableSize: 13,
              clickInfo: ClickInfo(state: myData.status == true ? true : false),
              mode: BtuttonMode.shine,
              buttonOnColor: ColorDisplay.graphChannelLine[currentChannel]),
        ),
        Column(
          mainAxisAlignment: MainAxisAlignment.spaceAround,
          children: [
            SizedBox(
              width: 100,
              height: 25,
              child: Button(
                  onChange: (ClickInfo info) {
                    if (fType == DspFilterType.lowPass) {
                      watch
                          .read(soundEqProvider[currentChannel].notifier)
                          .updateUiCrossover(fType, type: DspFilterType.besselLowPass);
                    } else {
                      watch
                          .read(soundEqProvider[currentChannel].notifier)
                          .updateUiCrossover(fType, type: DspFilterType.besselHighPass);
                    }
                  },
                  lable: "Bessel",
                  lableSize: 13,
                  clickInfo: ClickInfo(
                      state: myData.type == DspFilterType.besselLowPass || myData.type == DspFilterType.besselHighPass
                          ? true
                          : false),
                  mode: BtuttonMode.shine,
                  buttonOnColor: ColorDisplay.graphChannelLine[currentChannel]),
            ),
            SizedBox(
              width: 100,
              height: 25,
              child: Button(
                  onChange: (ClickInfo info) {
                    if (fType == DspFilterType.lowPass) {
                      watch.read(deviceSoundPrdr.notifier).setCrssover(CurrentFilterType.lowPass);
                      watch
                          .read(soundEqProvider[currentChannel].notifier)
                          .updateUiCrossover(fType, type: DspFilterType.butterworthLowPass);
                    } else {
                      watch.read(deviceSoundPrdr.notifier).setCrssover(CurrentFilterType.hiPass);
                      watch
                          .read(soundEqProvider[currentChannel].notifier)
                          .updateUiCrossover(fType, type: DspFilterType.butterworthHighPass);
                    }
                  },
                  lable: "Butterworth",
                  lableSize: 13,
                  clickInfo: ClickInfo(
                      state: myData.type == DspFilterType.butterworthLowPass ||
                              myData.type == DspFilterType.butterworthHighPass
                          ? true
                          : false),
                  mode: BtuttonMode.shine,
                  buttonOnColor: ColorDisplay.graphChannelLine[currentChannel]),
            ),
          ],
        ),
      ],
    ),
  );
}

Container crossoverFreqencyEditing(DspFilterType fType, int data, WidgetRef watch, int currentChannel) {
  return Container(
    padding: const EdgeInsets.all(3),
    decoration: BoxDecoration(
      borderRadius: BorderRadius.circular(5),
    ),
    child: Column(
      mainAxisAlignment: MainAxisAlignment.spaceBetween,
      children: [
        Container(
          width: 90,
          margin: const EdgeInsets.symmetric(horizontal: 3),
          padding: const EdgeInsets.symmetric(horizontal: 3, vertical: 2),
          decoration: BoxDecoration(
            borderRadius: BorderRadius.circular(5),
            color: ColorDisplay.backgroundOn, // Assuming ColorDisplay.backgroundOn is defined
            border: Border.all(color: ColorDisplay.displayFrame, width: 1), // Add white border
          ),
          child: FreqencyEdit(
            suffix: " Гц",
            text: data.toString(),
            onChangedCallback: (String val) {
              if (val.isEmpty) val = "20";
              data = int.parse(val);
              watch.read(soundEqProvider[currentChannel].notifier).updateUiCrossover(fType, freq: data);
              print(fType);
            },
          ),
        ),
        Row(
          mainAxisAlignment: MainAxisAlignment.spaceAround,
          children: [
            SizedBox(
              width: 35,
              height: 25,
              child: Button(
                onChange: (ClickInfo info) {
                  data--;
                  watch.read(soundEqProvider[currentChannel].notifier).updateUiCrossover(fType, freq: data);
                },
                lable: "-",
                lableSize: 13,
              ),
            ),
            const SizedBox(width: 10),
            SizedBox(
              width: 35,
              height: 25,
              child: Button(
                onChange: (ClickInfo info) {
                  data++;
                  watch.read(soundEqProvider[currentChannel].notifier).updateUiCrossover(fType, freq: data);
                },
                lable: "+",
                lableSize: 13,
              ),
            ),
          ],
        ),
      ],
    ),
  );
}

Container crossoverOrderEditing(DspFilterType fType, int data, WidgetRef watch, int currentChannel) {
  return Container(
    padding: const EdgeInsets.all(3),
    decoration: BoxDecoration(
      borderRadius: BorderRadius.circular(5),
    ),
    child: Column(
      mainAxisAlignment: MainAxisAlignment.spaceBetween,
      children: [
        Container(
          width: 90,
          margin: const EdgeInsets.symmetric(horizontal: 3),
          padding: const EdgeInsets.symmetric(horizontal: 3, vertical: 2),
          decoration: BoxDecoration(
            borderRadius: BorderRadius.circular(5),
            color: ColorDisplay.backgroundOn, // Assuming ColorDisplay.backgroundOn is defined
            border: Border.all(color: ColorDisplay.displayFrame, width: 1), // Add white border
          ),
          child: Text(
              textAlign: TextAlign.center,
              style: const TextStyle(color: ColorDisplay.lable, fontSize: 16),
              "${(data * 6).toString()} дБ/окт"),
        ),
        Row(
          mainAxisAlignment: MainAxisAlignment.spaceAround,
          children: [
            SizedBox(
              width: 35,
              height: 25,
              child: Button(
                onChange: (ClickInfo info) {
                  data--;
                  watch.read(soundEqProvider[currentChannel].notifier).updateUiCrossover(fType, order: data);
                },
                lable: "-",
                lableSize: 13,
              ),
            ),
            const SizedBox(width: 10),
            SizedBox(
              width: 35,
              height: 25,
              child: Button(
                onChange: (ClickInfo info) {
                  data++;
                  watch.read(soundEqProvider[currentChannel].notifier).updateUiCrossover(fType, order: data);
                },
                lable: "+",
                lableSize: 13,
              ),
            ),
          ],
        ),
      ],
    ),
  );
}

class FreqencyEdit extends StatefulWidget {
  final String suffix;
  final String text;
  final Function(String) onChangedCallback;
  const FreqencyEdit({
    super.key,
    required this.suffix,
    required this.text,
    required this.onChangedCallback,
  });

  @override
  FreqencyEditState createState() => FreqencyEditState();
}

class FreqencyEditState extends State<FreqencyEdit> {
  FreqencyEditState();
  final TextEditingController _controller = TextEditingController();
  final FocusNode _focusNode = FocusNode();
  String currentText = "";

  @override
  void initState() {
    super.initState();
    _controller.addListener(_updateDisplayText);
    _focusNode.addListener(_updateData);
  }

  @override
  void dispose() {
    _controller.removeListener(_updateDisplayText);
    _controller.dispose();
    _focusNode.dispose();
    super.dispose();
  }

  void _updateDisplayText() {
    currentText = _controller.text;

    // Удаляем суффикс, если он уже есть
    if (currentText.endsWith(widget.suffix)) {
      currentText = currentText.substring(0, currentText.length - widget.suffix.length);
    }

    // Обновляем текст с добавлением суффикса
    _controller.value = TextEditingValue(
      text: currentText + widget.suffix,
      selection: TextSelection.fromPosition(TextPosition(offset: currentText.length)),
    );
  }

  void _updateData() {
    widget.onChangedCallback(currentText);
  }

  @override
  Widget build(BuildContext context) {
    _controller.text = widget.text;
    return TextField(
      textAlign: TextAlign.center,
      keyboardType: TextInputType.number,
      controller: _controller,
      focusNode: _focusNode,
      inputFormatters: [
        FilteringTextInputFormatter.allow(RegExp(r'^\d*\.?\d{0,2}')),
      ],
      decoration: null,
    );
  }
}
