// ignore_for_file: public_member_api_docs, sort_constructors_first

import 'package:flutter/material.dart';
import 'package:dixom/dataBase/app_constants.dart';
import 'package:google_fonts/google_fonts.dart';

class Display extends StatelessWidget {
  const Display({
    super.key,
    required this.lable,
    this.lableSize = 16,
    this.buttonOnColor = ColorControls.buttonOrange,
  });

  final String lable;
  final double lableSize;
  final Color buttonOnColor;

  @override
  Widget build(BuildContext context) {
    return LayoutBuilder(builder: (BuildContext context, BoxConstraints constraints) {
      return Stack(
        children: [
          CustomPaint(
            size: Size(constraints.maxWidth, constraints.maxHeight),
            painter: DisplayPainter(lable: lable, lableSize: lableSize, buttonOnColor: buttonOnColor),
          ),
          CustomPaint(
            size: Size(constraints.maxWidth, constraints.maxHeight),
            painter: LevelDetectorPainter(lable: lable, lableSize: lableSize, buttonOnColor: buttonOnColor),
          ),
        ],
      );
    });
  }
}

class DisplayPainter extends CustomPainter {
  final String lable;
  final double lableSize;
  final Color buttonOnColor;
  final bool state;
  final bool presset;
  final int volume;

  DisplayPainter({
    this.presset = false,
    this.lable = "+",
    this.lableSize = 16,
    this.volume = 0,
    this.buttonOnColor = ColorControls.buttonOrange,
    this.state = false,
  });
  @override
  void paint(Canvas canvas, Size size) {
    double radius = 10;

    var centerX = size.width;
    var centerY = size.height;

    RRect line = RRect.fromRectAndRadius(
      Rect.fromLTWH(0, 0, centerX, centerY),
      Radius.circular(radius),
    );

    var displayBrush = Paint()..color = ColorDisplay.backgroundOn;
    canvas.drawRRect(line, displayBrush);

    var frameBrush = Paint()
      ..color = ColorDisplay.displayFrame
      ..style = PaintingStyle.stroke
      ..strokeWidth = 3;

    canvas.drawRRect(line, frameBrush);
    paintText("Volume 60", 50, canvas, 40, 0);
    paintText("Источник: USB Звуковая карта 24/96", 18, canvas, 10, 60);
    paintText("Пресет: А «В машине»", 18, canvas, 10, 90);

    final shadowPaint = Paint()
      ..strokeWidth = 1
      ..color = ColorDisplay.lable;

    const double hUpLine = 80;
    const double hDownLine = 20;
    const double wVolLine = 50;

    var p1 = Offset(0, size.height - hUpLine);
    var p2 = Offset(size.width - wVolLine, size.height - hUpLine);
    canvas.drawLine(p1, p2, shadowPaint);

    p1 = Offset(0, size.height - hDownLine);
    p2 = Offset(size.width - wVolLine, size.height - hDownLine);
    canvas.drawLine(p1, p2, shadowPaint);

    p1 = Offset(size.width - wVolLine, 0);
    p2 = Offset(size.width - wVolLine, size.height);
    canvas.drawLine(p1, p2, shadowPaint);

    /* p1 = Offset(0, size.height - 50);
    p2 = Offset(size.width, size.height - 50);
    canvas.drawLine(p1, p2, shadowPaint);*/
  }

  void paintText(String text, double fontSize, Canvas canvas, double width, double height) {
    final textSpan = TextSpan(
      text: text,
      style: GoogleFonts.abel(
        fontSize: fontSize,
        color: ColorDisplay.lable,
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

class LevelDetectorPainter extends CustomPainter {
  final String lable;
  final double lableSize;
  final Color buttonOnColor;
  final bool state;
  final bool presset;
  final int volume;

  LevelDetectorPainter({
    this.presset = false,
    this.lable = "+",
    this.lableSize = 16,
    this.volume = 0,
    this.buttonOnColor = ColorControls.buttonOrange,
    this.state = false,
  });
  @override
  void paint(Canvas canvas, Size size) {
    Rect line = Rect.fromPoints(
      Offset(size.width - 30, 10),
      Offset(size.width - 10, size.height - 10),
    );

    var displayBrush = Paint()
      ..style = PaintingStyle.stroke
      ..strokeWidth = 1
      ..color = ColorDisplay.lable;
    canvas.drawRect(line, displayBrush);
  }

  @override
  bool shouldRepaint(CustomPainter oldDelegate) => true;
}
