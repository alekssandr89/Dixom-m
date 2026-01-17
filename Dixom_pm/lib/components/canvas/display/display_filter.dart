
import 'dart:math';
import 'dart:ui' as ui;
import 'package:flutter/material.dart';
import 'package:dixom/dataBase/app_constants.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:dixom/pages/audio/backend/biquadratic.dart';
import 'package:dixom/pages/audio/backend/filter.dart';

enum DrawingType { frame, graph, frameAndGraph }

class DisplayFilter extends StatelessWidget {
  const DisplayFilter({
    super.key,
    required this.lable,
    this.lableSize = 16,
    this.showCrossover = false,
    this.strokeWidth = 1.5, //Толщина линии графика
    required this.filter,
    this.color = ColorControls.buttonOrange,
    this.drawingType = DrawingType.frameAndGraph,
  });

  final String lable;
  final double lableSize;
  final Color color;
  final DspFilterModel filter;
  final DrawingType drawingType;
  final bool showCrossover;
  final double strokeWidth;

  @override
  Widget build(BuildContext context) {
    return LayoutBuilder(builder: (BuildContext context, BoxConstraints constraints) {
      return Stack(
        children: [
          CustomPaint(
            size: Size(constraints.maxWidth, constraints.maxHeight),
            painter: DisplayFilterPainter(
              lable: lable,
              lableSize: lableSize,
              color: color,
              filter: filter,
              drawingType: drawingType,
              showCrossover: showCrossover,
              strokeWidth: strokeWidth,
            ),
          ),
        ],
      );
    });
  }
}

class DisplayFilterPainter extends CustomPainter {
  final String lable;
  final double lableSize;
  final Color color;
  final DspFilterModel filter;
  final bool state;
  final bool presset;
  final bool showCrossover;
  final DrawingType drawingType;
  final double strokeWidth; //Толщина линии графика

  DisplayFilterPainter({
    required this.filter,
    this.presset = false,
    this.showCrossover = false,
    this.lable = "+",
    this.lableSize = 16,
    this.color = ColorControls.buttonOrange,
    this.state = false,
    required this.strokeWidth,
    required this.drawingType,
  });
  @override
  void paint(Canvas canvas, Size size) {
    const int minFreq = 20; // Минимальная частота
    const int maxFreq = 22000; //максимальная частота
    final double logMinFreq = log(minFreq) / ln10; //мин частота в лог
    final double logMaxFreq = log(maxFreq) / ln10; //Макс частота в лог
    const int margin = 20; //отступ от края графика
    final double chartWidth = size.width - margin; // ширина графика
    const double accuracy = 0.01; // точность графика
    const double radius = 10; // адиус скругления графика
    final double nullDbLine = size.height / 2 - 10; //нулевая линия
    const double gainScaling = 3; // коэфициент маштабирования усиления

    double freqToCoordinate(int freq) {
      // Преобразование входного значения в логарифмическую шкалу
      double logScaledValue = (log(freq) / ln10 - logMinFreq) / (logMaxFreq - logMinFreq);
      // Преобразование логарифмически масштабированного значения в координаты на графике
      return chartWidth * logScaledValue + margin / 2;
    }

    //Рисуем вертикальные линии
    void drawVerticalLine(
      int freq,
      double height, {
      bool text = false,
      Color color = ColorDisplay.lable,
      double strokeWidth = 0.3,
    }) {
      if (freq < minFreq) {
        return;
      }
      if (freq > maxFreq) {
        return;
      }
      double x = freqToCoordinate(freq);
      if (text) {
        paintText(freq.toString(), 15, canvas, x + 2, height - 20);
      }
      canvas.drawLine(
        Offset(x, 0),
        Offset(x, size.height),
        Paint()
          ..strokeWidth = strokeWidth
          ..color = color,
      );
    }

    //Рисуем горизонтальная линия
    void drawHorizontalLine(
      double gain, {
      bool text = false,
      double strokeWidth = 0.3,
    }) {
      gain = nullDbLine + gain * gainScaling;

      canvas.drawLine(
        Offset(0, gain),
        Offset(size.width, gain),
        Paint()
          ..strokeWidth = strokeWidth
          ..color = ColorDisplay.lable,
      );
    }

    if (drawingType == DrawingType.frame || drawingType == DrawingType.frameAndGraph) {
      RRect line = RRect.fromRectAndRadius(
        Rect.fromLTWH(0, 0, size.width, size.height),
        const Radius.circular(radius),
      );
      var displayBrush = Paint()..color = ColorDisplay.backgroundOn;
      canvas.drawRRect(line, displayBrush);
      var frameBrush = Paint()
        ..color = ColorDisplay.displayFrame
        ..style = PaintingStyle.stroke
        ..strokeWidth = 2;
      canvas.drawRRect(line, frameBrush);
      drawHorizontalLine(20, strokeWidth: .6);
      drawHorizontalLine(15);
      drawHorizontalLine(10, strokeWidth: .6);
      drawHorizontalLine(5);
      drawHorizontalLine(0, strokeWidth: 1);
      drawHorizontalLine(-5);
      drawHorizontalLine(-10, strokeWidth: .6);
      drawHorizontalLine(-15);
      drawHorizontalLine(-20, strokeWidth: .6);
      drawHorizontalLine(-25);
      void drawVerticalLines() {
        for (int i = 10; i <= maxFreq; i *= 10) {
          if (i < minFreq) {
            drawVerticalLine(minFreq, size.height, text: true, strokeWidth: 1);
          }
          drawVerticalLine(i, size.height, text: true);
          for (int j = i; j <= i * 10; j += (i / 2).ceil()) {
            drawVerticalLine(j, size.height, text: false);
          }
        }
      }

      drawVerticalLines();
    }

    if (drawingType == DrawingType.graph || drawingType == DrawingType.frameAndGraph) {
      const pointMode = ui.PointMode.points;

      //Добавление точек полос эквалайзера
      if (showCrossover) {
        List<Offset> bandPoints = [];
        for (int i = 0; i < filter.eq.length; i++) {
          double freq = freqToCoordinate(filter.eq[i].freq);
          double boost = nullDbLine - filter.eq[i].boost * gainScaling - filter.gain * gainScaling;
          double xScalling = 4;
          if (i > 8) xScalling = 8;
          paintText((i + 1).toString(), 15, canvas, freq - xScalling, boost - 23);
          bandPoints.add(Offset(freq, boost));
        }
        final paintBand = Paint()
          ..color = color
          ..strokeWidth = 9
          ..strokeCap = StrokeCap.round;
        canvas.drawPoints(pointMode, bandPoints, paintBand);

        if (filter.hiPass.status) {
          paintText("Hi-Pass", 15, canvas, freqToCoordinate(filter.hiPass.freq) + 5, 5, color: color);
          drawVerticalLine(filter.hiPass.freq, size.height, color: color, strokeWidth: 1);
        }
        if (filter.lowPass.status) {
          paintText("Low-Pass", 15, canvas, freqToCoordinate(filter.lowPass.freq) + 5, 5, color: color);
          drawVerticalLine(filter.lowPass.freq, size.height, color: color, strokeWidth: 1);
        }
      }

      List<Offset> points = [];
      for (double frequencyPoint = log(minFreq) / ln10; frequencyPoint < logMaxFreq;) {
        /*Расчёт координаты графика для точки X*/
        num pointX = pow(10, frequencyPoint);
        /*Расчёт координаты графика для точки Y*/
        double pointY = 0;
        /*Эквалайзер */
        if (filter.bypass) {
          for (int band = 0; band < filter.eq.length; band++) {
            BiQuadraticFilter biquad = BiQuadraticFilter(
              frequency: filter.eq[band].freq,
              q: filter.eq[band].q,
              boost: filter.eq[band].boost,
              state: filter.eq[band].status,
            );
            pointY += biquad.logResult(pointX.ceil());
          }
        }
        /*Кроссвоер hi pass*/
        if (filter.hiPass.status) {
          BiQuadraticFilter biquad = BiQuadraticFilter(
              frequency: filter.hiPass.freq,
              q: filter.hiPass.q,
              boost: filter.hiPass.boost,
              state: filter.hiPass.status,
              type: filter.hiPass.type);
          for (int order = 0; order < filter.hiPass.order; order++) {
            pointY += biquad.logResult(pointX.ceil());
          }
        }
        /*Кроссвоер low pass*/
        if (filter.lowPass.status) {
          BiQuadraticFilter biquad = BiQuadraticFilter(
              frequency: filter.lowPass.freq,
              q: filter.lowPass.q,
              boost: filter.lowPass.boost,
              state: filter.lowPass.status,
              type: filter.lowPass.type);
          for (int order = 0; order < filter.lowPass.order; order++) {
            pointY += biquad.logResult(pointX.ceil());
          }
        }
        /*Добавление полученных координат на график*/
        pointX = freqToCoordinate(pointX.ceil());

        pointY = nullDbLine - pointY * gainScaling - filter.gain * gainScaling;

        if (pointY < size.height && pointY > 0) {
          points.add(Offset(pointX as double, pointY));
        }
        frequencyPoint += accuracy;
      }
      final paint = Paint()
        ..color = color
        ..strokeWidth = strokeWidth
        ..strokeCap = StrokeCap.round;

      for (int i = 0; i < points.length - 1; i++) {
        canvas.drawLine(points[i], points[i + 1], paint);
      }
    }
  }

  void paintText(String text, double fontSize, Canvas canvas, double width, double height,
      {Color color = ColorDisplay.lable}) {
    final textSpan = TextSpan(
      text: text,
      style: GoogleFonts.abel(
        fontSize: fontSize,
        color: color,
      ),
    );
    TextPainter textPainter =
        TextPainter(text: textSpan, textAlign: TextAlign.center, textDirection: TextDirection.ltr);
    textPainter.layout();
    textPainter.paint(canvas, Offset(width, height));
  }

  @override
  bool shouldRepaint(DisplayFilterPainter old) => old.state != state;

  /*   final String lable;
  final double lableSize;
  final Color color;
  final DspFilterModel filter;
  final bool state;
  final bool presset;
  final int volume;
  final bool showCrossover;
  final DrawingType drawingType;
  final double strokeWidth; //Толщина линии графика*/
}
