// ignore_for_file: public_member_api_docs, sort_constructors_first

import 'package:flutter/material.dart';
import 'package:google_fonts/google_fonts.dart';

import 'package:dixom/dataBase/app_constants.dart';

class LevelDetector extends StatelessWidget {
  const LevelDetector({
    super.key,
    required this.lable,
    this.color = ColorControls.buttonOrange,
    required this.valueMax,
    required this.value,
  });

  final String lable;
  final Color color;
  final int valueMax;
  final int value;

  @override
  Widget build(BuildContext context) {
    return LayoutBuilder(builder: (BuildContext context, BoxConstraints constraints) {
      return Stack(
        children: [
          CustomPaint(
            size: Size(constraints.maxWidth, constraints.maxHeight),
            painter: LevelDetectorPainter(lable: lable, color: color, valueMax: valueMax, value: value),
          ),
        ],
      );
    });
  }
}

class LevelDetectorPainter extends CustomPainter {
  final String lable;
  final Color color;
  final int valueMax;
  final int value;

  LevelDetectorPainter({
    required this.lable,
    required this.color,
    required this.valueMax,
    required this.value,
  });
  @override
  void paint(Canvas canvas, Size size) {
    paintText(lable, 18, canvas, 0, -5);

    double max = size.width - 2;
    double min = 25;

    final rectBorder = Rect.fromLTRB(min - 2, 0, size.width, size.height);
    final border = Paint()
      ..color = const Color.fromARGB(197, 0, 225, 255) // Цвет рамки
      ..style = PaintingStyle.stroke // Установка стиля рамки
      ..strokeWidth = 1.0; // Толщина рамки

    canvas.drawRect(rectBorder, border);

    double diferent = max - min;
    double inputPersent = value / valueMax;
    double levelValue = inputPersent * diferent + min;

    final rectLvel = Rect.fromLTRB(min, 2, levelValue, size.height - 2);
    final level = Paint()..color = const Color.fromARGB(197, 0, 225, 255);

    canvas.drawRect(rectLvel, level);
  }

  void paintText(String text, double fontSize, Canvas canvas, double width, double height) {
    final textSpan = TextSpan(
      text: text,
      style: GoogleFonts.abel(
        fontSize: fontSize,
        color: Colors.white70,
      ),
    );
    TextPainter textPainter =
        TextPainter(text: textSpan, textAlign: TextAlign.center, textDirection: TextDirection.ltr);
    textPainter.layout();
    textPainter.paint(canvas, Offset(width, height));
  }

  @override
  bool shouldRepaint(CustomPainter oldDelegate) => true;
}
