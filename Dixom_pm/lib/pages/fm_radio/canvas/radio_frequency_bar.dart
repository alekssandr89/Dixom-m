import 'package:flutter/material.dart';
import 'package:google_fonts/google_fonts.dart';

class RadioFrequencyBar extends StatefulWidget {
  const RadioFrequencyBar({
    super.key,
    this.valueMin = 87.5,
    this.valueMax = 108,
    this.value = 105.5,
    this.indentation = 20,
    required this.onChange,
  });
  final double valueMin;
  final double valueMax;
  final double value;
  final double indentation; //смещение шкалы по края
  final ValueChanged<double> onChange;
  @override
  State<RadioFrequencyBar> createState() => _RadioFrequencyBarState();
}

class _RadioFrequencyBarState extends State<RadioFrequencyBar> {
  //возвращает текущее значение
  @override
  Widget build(BuildContext context) {
    Size canvasSize = Size.zero;

    return MouseRegion(
      cursor: SystemMouseCursors.click,
      child: GestureDetector(
        onVerticalDragUpdate: (detail) {
          double freqValue =
              (detail.localPosition.dx - widget.indentation) / (canvasSize.width - widget.indentation * 2);
          final double range = widget.valueMax - widget.valueMin;
          freqValue = freqValue * range + widget.valueMin;
          if (freqValue >= widget.valueMin && freqValue <= widget.valueMax) {
            widget.onChange(freqValue);
          }
        },
        child: CustomPaint(
          size: MediaQuery.sizeOf(context),
          painter: RadioFrequencyBarPainter(
            valueMin: widget.valueMin,
            valueMax: widget.valueMax,
            value: widget.value,
            indentation: widget.indentation,
            getCanvasSize: (Size size) {
              canvasSize = size;
            },
          ),
        ),
      ),
    );
  }
}

class RadioFrequencyBarPainter extends CustomPainter {
  final double valueMin;
  final double valueMax;
  final double value;
  final double indentation; //смещение шкалы по края
  final ValueChanged<Size> getCanvasSize;

  RadioFrequencyBarPainter({
    this.valueMin = 87.5,
    this.valueMax = 108,
    this.value = 0,
    this.indentation = 25,
    required this.getCanvasSize,
  });
  @override
  void paint(Canvas canvas, Size size) {
    //Возвращаем размер виджета для управления
    getCanvasSize(size);
    //Полоска по центру
    final vertialLine = Paint()
      ..strokeWidth = 2
      ..color = Colors.white
      ..style = PaintingStyle.stroke;

    //Полоски
    canvas.drawLine(const Offset(0, 0), Offset(size.width, 0), vertialLine);
    canvas.drawLine(Offset(0, size.height), Offset(size.width, size.height), vertialLine);

    //Шкала
    int lableEvery = 20; //Отображать лейбл каждые
    double accuracy = 0.1; //Отображать лейбл каждые

    if (size.width < 750) {
      accuracy = 0.13;
      lableEvery = 30;
    }
    if (size.width < 400) {
      accuracy = 0.25;
      lableEvery = 20;
    }
    final double range = valueMax - valueMin;
    final int lineCount = (range / accuracy).ceil();
    final double bias = (size.width - indentation * 2) / lineCount;
    double lineHigh = size.height / 3;
    int lableEveryCounter = lableEvery;
    for (double i = 0; i <= lineCount; i += 1) {
      final double width = bias * i + indentation;
      if (lableEveryCounter == lableEvery) {
        lableEveryCounter = 0;
        lineHigh = size.height / 2;
        paintText((i * accuracy + valueMin).toString(), 20, canvas, width, 40);
      } else {
        lineHigh = size.height / 3;
      }
      lableEveryCounter++;
      canvas.drawLine(Offset(width, 5), Offset(width, lineHigh), vertialLine);
    }

    //Ручка
    final handle = Paint()
      ..strokeWidth = 10
      ..color = Colors.red.withOpacity(0.7)
      ..style = PaintingStyle.stroke;

    final double valuePersent = (value - valueMin) / range;
    final double width = valuePersent * (size.width - indentation * 2) + indentation;
    canvas.drawLine(Offset(width, 5), Offset(width, size.height - 5), handle);
  }

  void paintText(String text, double fontSize, Canvas canvas, double width, double height) {
    final textSpan = TextSpan(
      text: text,
      style: GoogleFonts.abel(
        fontSize: fontSize,
        color: Colors.white70,
      ),
    );
    TextPainter painter = TextPainter(
      text: TextSpan(
        text: text,
        style: TextStyle(fontSize: fontSize),
      ),
      textDirection: TextDirection.ltr,
    );

    painter.layout();

    TextPainter textPainter =
        TextPainter(text: textSpan, textAlign: TextAlign.center, textDirection: TextDirection.ltr);
    textPainter.layout();
    textPainter.paint(canvas, Offset(width - (painter.width / 2), height));
  }

  @override
  bool shouldRepaint(CustomPainter oldDelegate) => true;
}
