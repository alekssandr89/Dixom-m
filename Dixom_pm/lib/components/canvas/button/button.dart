// ignore_for_file: public_member_api_docs, sort_constructors_first

import 'dart:ui' as ui;

import 'package:flutter/material.dart';
import 'package:google_fonts/google_fonts.dart';

import 'package:dixom/dataBase/app_constants.dart';

enum TapType { tap, long }

class ClickInfo {
  final TapType type;
  final bool state;
  const ClickInfo({
    this.type = TapType.tap,
    this.state = false,
  });
}

class Button extends StatefulWidget {
  const Button({
    super.key,
    required this.onChange,
    required this.lable,
    this.lableSize = 16,
    this.buttonOnColor = ColorControls.buttonOrange,
    this.mode = BtuttonMode.standart,
    this.clickInfo = const ClickInfo(state: true, type: TapType.long),
  });

  final ValueChanged<ClickInfo> onChange;
  final String lable;
  final double lableSize;
  final Color buttonOnColor;
  final BtuttonMode mode;
  final ClickInfo clickInfo;

  @override
  State<Button> createState() => _ButtonState();
}

class _ButtonState extends State<Button> {
  bool presset = false;

  _ButtonState();

  @override
  Widget build(BuildContext context) {
    return LayoutBuilder(builder: (BuildContext context, BoxConstraints constraints) {
      final w = constraints.maxWidth;
      final h = constraints.maxHeight;

      return MouseRegion(
        cursor: SystemMouseCursors.click,
        child: GestureDetector(
          onLongPress: () {
            widget.onChange(ClickInfo(
              state: widget.clickInfo.state,
              type: TapType.long,
            ));
          },
          onTapCancel: () {
            setState(() {
              presset = false;
            });
          },
          onTapUp: (detail) {
            setState(() {
              presset = false;
              widget.onChange(ClickInfo(
                state: widget.clickInfo.state,
                type: TapType.tap,
              ));
            });
          },
          onTapDown: (detail) {
            setState(() {
              presset = true;
            });
          },
          child: CustomPaint(
            painter: ButtonPainter(
                presset: presset,
                mode: widget.mode,
                clickInfo: widget.clickInfo,
                width: w,
                height: h,
                lable: widget.lable,
                lableSize: widget.lableSize,
                buttonOnColor: widget.buttonOnColor),
          ),
        ),
      );
    });
  }
}

enum BtuttonMode {
  standart,
  shine,
}

class ButtonPainter extends CustomPainter {
  final double width;
  final double height;
  final String lable;
  final double lableSize;
  final Color buttonOnColor;
  final BtuttonMode mode;
  final ClickInfo clickInfo;
  final bool presset;

  ButtonPainter({
    this.presset = false,
    required this.width,
    required this.height,
    this.lable = "+",
    this.lableSize = 16,
    this.buttonOnColor = ColorControls.buttonOrange,
    this.mode = BtuttonMode.standart,
    required this.clickInfo,
  });
  @override
  void paint(Canvas canvas, Size size) {
    button(canvas, size);
  }

  void button(Canvas canvas, Size size) {
    double radius = 10;

    var centerX = width / 2;
    var centerY = height / 2;
    var center = Offset(centerX, centerY);
    Rect rect = Rect.fromCenter(center: center, width: size.width, height: size.height);

    const double cutout = 4;
    const double cutoutOfset = cutout / 2;
    double buttonX = width - cutout;
    double buttonY = height - cutout;

    //Вырез
    final Paint cutoutBrush = Paint()
      ..color = ColorControls.cutout
      ..strokeWidth = cutout
      ..style = PaintingStyle.stroke
      ..strokeCap = StrokeCap.round;
    RRect line = RRect.fromRectAndRadius(
      Rect.fromLTWH(0, 0, width, height),
      Radius.circular(radius * 1.25),
    );
    canvas.drawRRect(line, cutoutBrush);

    //блеск выреза
    line = RRect.fromRectAndRadius(
      Rect.fromLTWH(-cutoutOfset, -cutoutOfset, width + cutout, height + cutout),
      Radius.circular(radius * 1.25),
    );
    var depressionBrush = Paint()
      ..shader = const LinearGradient(
        begin: Alignment.topCenter,
        end: Alignment.bottomCenter,
        stops: [0.80, 1],
        colors: [
          ColorPanel.background,
          ColorControls.depressionDown,
        ],
      ).createShader(rect)
      ..style = PaintingStyle.stroke
      ..strokeWidth = 1;
    canvas.drawRRect(line, depressionBrush);

    // Рисуем линию для создания эфекта тени

    final shadowPaint = Paint()
      ..strokeWidth = height / (presset ? 3 : 2)
      ..color = ColorControls.shadow.withOpacity(0.9)
      ..maskFilter = const MaskFilter.blur(BlurStyle.normal, 12.0);

    var p1 = Offset(10, height - 8);
    var p2 = Offset(width - 10, height - 8);
    canvas.drawLine(p1, p2, shadowPaint);

    //кнопка
    var fillBrush = Paint();
    fillBrush.shader = const LinearGradient(
      begin: Alignment.topCenter,
      end: Alignment.bottomCenter,
      stops: [0.1, 0.9],
      colors: [ColorControls.buttonUp, ColorControls.buttonDown],
    ).createShader(rect);
    line = RRect.fromRectAndRadius(
      Rect.fromLTWH(cutoutOfset, cutoutOfset, buttonX, buttonY),
      Radius.circular(radius),
    );
    canvas.drawRRect(line, fillBrush);

    //блеск или свечение кнопки
    if (mode == BtuttonMode.standart) {
      line = RRect.fromRectAndRadius(
        Rect.fromLTWH(cutoutOfset, cutoutOfset, buttonX, buttonY),
        Radius.circular(radius),
      );
      var valueBrush = Paint()
        ..shader = const LinearGradient(
          begin: Alignment.topCenter,
          end: Alignment.bottomCenter,
          stops: [0.1, 0.3],
          colors: [ColorControls.buttonShineUp, ui.Color.fromARGB(255, 64, 74, 104)],
        ).createShader(rect)
        ..style = PaintingStyle.stroke
        ..strokeWidth = 1;
      if (presset == false) canvas.drawRRect(line, valueBrush);
    } else if (mode == BtuttonMode.shine) {
      line = RRect.fromRectAndRadius(
        Rect.fromLTWH(cutoutOfset, cutoutOfset, buttonX, buttonY),
        Radius.circular(radius),
      );
      final Paint valueBrush = Paint()
        ..color = clickInfo.state == true ? buttonOnColor : ColorControls.notchNoActive
        ..strokeWidth = cutout
        ..style = PaintingStyle.stroke
        ..strokeWidth = presset ? 1.5 : 2
        ..strokeCap = StrokeCap.round;

      canvas.drawRRect(line, valueBrush);

      if (clickInfo.state == true) {
        final Paint valueShadowBrush = Paint()
          ..color = buttonOnColor
          ..strokeWidth = cutout
          ..style = PaintingStyle.stroke
          ..strokeWidth = presset ? 1.5 : 2
          ..maskFilter = const ui.MaskFilter.blur(BlurStyle.normal, 10)
          ..strokeCap = StrokeCap.round;
        canvas.drawRRect(line, valueShadowBrush);
      }
    }

    //Текст

    final textSpan = TextSpan(
      text: lable,
      style: GoogleFonts.kanit(
          letterSpacing: presset ? 1.4 : 1,
          fontSize: presset ? lableSize - 1 : lableSize, // Размер шрифта
          //fontWeight: FontWeight.bold,
          color: clickInfo.state == true ? buttonOnColor : ColorControls.lable),
    );

    final textPainter = TextPainter(
      text: textSpan,
      textAlign: TextAlign.center,
      textDirection: TextDirection.ltr,
    );

    textPainter.layout(); // Вычисляем размеры текста

    final textOffset = Offset(
      center.dx - textPainter.width / 2,
      center.dy - textPainter.height / 2,
    );

    textPainter.paint(canvas, textOffset); // Рисуем текст на холсте
  }

  @override   
  bool shouldRepaint(covariant ButtonPainter old) => old.presset != presset;
}
