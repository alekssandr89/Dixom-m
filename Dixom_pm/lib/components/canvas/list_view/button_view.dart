// ignore_for_file: public_member_api_docs, sort_constructors_first

import 'package:flutter/material.dart';
import 'package:google_fonts/google_fonts.dart';

import 'package:dixom/dataBase/app_constants.dart';

import '../../../pages/button/backend/button_model.dart';

enum ClickType { delete, setup }

class ButtonView extends StatelessWidget {
  const ButtonView({
    super.key,
    required this.onChange,
    required this.button,
    required this.w,
    required this.h,
  });

  final ValueChanged<ClickType> onChange;
  final OneButton button;
  final double w;
  final double h;

  @override
  Widget build(BuildContext context) {
    return MouseRegion(
      cursor: SystemMouseCursors.click,
      child: GestureDetector(
        onLongPress: () {},
        onTapCancel: () {},
        onTapUp: (detail) {},
        onTapDown: (detail) {},
        child: CustomPaint(
          painter: ButtonPainter(
            width: w,
            height: h,
          ),
        ),
      ),
    );
  }
}

enum BtuttonMode {
  standart,
  shine,
}

class ButtonPainter extends CustomPainter {
  final double width;
  final double height;

  ButtonPainter({
    required this.width,
    required this.height,
  });
  @override
  void paint(Canvas canvas, Size size) {
    double radius = 10;
    var center = Offset(width / 2, height / 2);
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

    //Текст

    final textSpan = TextSpan(
      text: "пример",
      style: GoogleFonts.kanit(
          letterSpacing: 1,
          fontSize: 15, // Размер шрифта
          //fontWeight: FontWeight.bold,
          color: ColorControls.lable),
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
  bool shouldRepaint(CustomPainter oldDelegate) => true;
}
