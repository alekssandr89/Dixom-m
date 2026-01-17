/*
                pi 3.14
                  180°
               *      *
            *            *
          *                *
    pi/2  90°               270° 3*pi/2
          *                *
            *            *
               *      *
                0° 360° 6.28
                 2*pi
*/
import 'dart:ui' as ui;
import 'dart:math';
import 'package:dixom/dataBase/app_constants.dart';
import 'package:flutter/material.dart';

enum EncoderState {
  left,
  right,
  empty,
}

enum ControlMode {
  ordinary,
  encoder,
  analogWithStop,
  analogWithoutStop,
  test,
}

class KnopView extends StatefulWidget {
  final double value; //Текущий шаг
  final double minAngle; //начальный гол
  final double maxAngle; //конечный угол
  final double min; //минимальное значение
  final double max; //максимальное значение
  final int steps; //количество шагов (насечек)
  final int encoderSteps; //количество точек на энкодере
  final int notchesLength; //длина насечек
  final double pading; //отступ от ручки до насечек
  final ControlMode controlMode; //режим управления
  final ValueChanged<double> onChange; //возвращает текущее значение
  final ValueChanged<EncoderState> onDirection; //Возвращяет направление
  const KnopView({
    super.key,
    this.value = 0,
    required this.onChange,
    required this.onDirection,
    this.minAngle = (45 * pi) / 180,
    this.maxAngle = (315 * pi) / 180,
    this.notchesLength = 2,
    this.pading = 10,
    this.encoderSteps = 14,
    this.min = 0,
    this.max = 60,
    this.steps = 60,
    this.controlMode = ControlMode.analogWithStop,
  });
  @override
  State<KnopView> createState() => _KnopViewState();
}

//Если ткнули не вкуруг то возвращает false
bool getIncludedOrNot(double margin, double xWidth, double yHeight, double xIn, double yIn) {
  bool inscribed = false;
  // Рассчитываем радиус окружности с учётом отступа от края
  double radius = (min(xWidth, yHeight) / 2.0) - margin;
  // Рассчитываем координаты центра окружности
  Offset center = Offset(xWidth / 2.0, yHeight / 2.0);
  // Рассчитываем расстояние от входных координат до центра окружности
  double distance = sqrt(pow(xIn - center.dx, 2) + pow(yIn - center.dy, 2));
  // Проверяем, находятся ли входные координаты внутри окружности
  if (distance <= radius) {
    inscribed = true;
  }
  return inscribed;
}

//На основании предыдущего угла и нового возвращает направление вращения
EncoderState getDirrection(double pastAngle, double newAngle) {
  if (newAngle > pastAngle) {
    if ((newAngle - pastAngle).abs() > pi) {
      return EncoderState.left;
    } else {
      return EncoderState.right;
    }
  } else {
    if ((pastAngle - newAngle).abs() > pi) {
      return EncoderState.right;
    } else {
      return EncoderState.left;
    }
  }
}

class _KnopViewState extends State<KnopView> {
  double distance = 0; //смещение ручки от нажатия пользователя
  double pastAngle = 0; //Прошлое значение угла
  bool rotate = false;
  double encValue = 0;

  @override
  Widget build(BuildContext context) {
    return Transform.rotate(
      angle: pi * 3 / 2,
      child: LayoutBuilder(
        builder: (BuildContext context, BoxConstraints constraints) {
          double range = widget.maxAngle - widget.minAngle;

          return GestureDetector(
            onPanStart: (d) {
              rotate = getIncludedOrNot(
                widget.pading + widget.notchesLength,
                constraints.maxWidth,
                constraints.maxHeight,
                d.localPosition.dx,
                d.localPosition.dy,
              );
              final double x = d.localPosition.dx - constraints.maxWidth / 2;
              final double y = d.localPosition.dy - constraints.maxHeight / 2;
              distance = ((widget.value / widget.max) * range + widget.minAngle) - (atan2(y, x) + pi);
            },
            onPanUpdate: (details) {
              if (rotate) {
                // Получаем радиану положения пальца на координатной сетке в моменты перемещения
                final double newAngle = atan2(
                  details.localPosition.dy - constraints.maxHeight / 2,
                  details.localPosition.dx - constraints.maxWidth / 2,
                );

                double oneStep;
                double valueNew;

                switch (widget.controlMode) {
                  case ControlMode.ordinary:
                    // Обычный режим вращения
                    // Угол одного шага
                    oneStep = widget.max / range;
                    // Получение значения из угла
                    valueNew = oneStep * (newAngle + pi - widget.minAngle);
                    valueNew = valueNew.clamp(widget.min, widget.max);
                    widget.onChange(valueNew);
                    break;

                  case ControlMode.analogWithStop:
                    // На подобие аналогового энкодера
                    // Угол одного шага
                    oneStep = widget.max / range;

                    // Определяем момент перехода через 0
                    const double pi2 = 2 * pi;
                    if ((newAngle - pastAngle).abs() > pi2 - 0.5) {
                      if (newAngle < pastAngle) {
                        // Вращение в правую сторону
                        distance += pi2;
                      } else {
                        // Вращение в левую сторону
                        distance -= pi2;
                      }
                    }
                    pastAngle = newAngle;

                    valueNew = oneStep * (newAngle + pi - widget.minAngle + distance);
                    valueNew = valueNew.clamp(widget.min, widget.max);

                    // Вызываем onChange только если значение реально изменилось
                    if (valueNew.toInt() != widget.value.toInt()) {
                      widget.onChange(valueNew);
                    }
                    break;

                  case ControlMode.encoder:
                    // Режим энкодера
                    // Угол одного шага
                    oneStep = widget.steps / range;
                    valueNew = oneStep * (newAngle + pi - widget.minAngle);

                    if (valueNew.toInt() != encValue.toInt()) {
                      widget.onDirection(getDirrection(pastAngle, newAngle));
                    }

                    setState(() {
                      encValue = valueNew;
                    });
                    pastAngle = newAngle;
                    break;
                  default:
                }
              }
            },
            child: SizedBox(
              width: constraints.maxWidth,
              height: constraints.maxHeight,
              child: CustomPaint(
                painter: KnopPainter(
                  encoderSteps: widget.encoderSteps,
                  pading: widget.pading,
                  encValue: encValue,
                  value: widget.value,
                  minAngle: widget.minAngle,
                  maxAngle: widget.maxAngle,
                  notchesLenght: widget.notchesLength,
                  steps: widget.steps,
                  controlMode: widget.controlMode,
                ),
              ),
            ),
          );
        },
      ),
    );
  }
}

class KnopPainter extends CustomPainter {
  final double value; //Текущий шаг
  final double encValue;
  final double minAngle; //начальный гол
  final double maxAngle; //конечный угол
  final int steps; //количество насечек
  final int notchesLenght; //длина насечек
  final double pading; //граница от края до крутилки
  final ControlMode controlMode; //Тип кроллера
  final int encoderSteps;

  KnopPainter(
      {required this.value,
      required this.encValue,
      required this.minAngle,
      required this.maxAngle,
      required this.notchesLenght,
      required this.steps,
      required this.pading,
      required this.controlMode,
      required this.encoderSteps});

  @override
  void paint(Canvas canvas, Size size) {
    
    final Offset center = Offset(size.width / 2, size.height / 2);

    //Радиус ручки
    double handleRadius = min(center.dx, center.dy);
    //Если это не энкодер, то добавляем отступ для насечек
    if (ControlMode.encoder != controlMode) {
      handleRadius = handleRadius - notchesLenght - pading;
    }
    final double borderWidth = handleRadius / 10;

    //Вырез
    final Paint cutoutBrush = Paint()
      ..color = ColorControls.cutout
      ..strokeWidth = borderWidth;
    canvas.drawCircle(center, handleRadius + (handleRadius / 20) - borderWidth, cutoutBrush);

    final Rect rect = Rect.fromCenter(
      center: center,
      width: size.width,
      height: size.height,
    );

    //Свечение выреза снизу
    final Paint valueBrush = Paint()
      ..shader = const LinearGradient(
        begin: Alignment.bottomRight,
        end: Alignment.bottomLeft,
        stops: [0.6, 0.9],
        colors: [ColorControls.buttonShineDown, ColorControls.buttonShineUp],
      ).createShader(rect)
      ..style = PaintingStyle.stroke
      ..strokeWidth = handleRadius < 100 ? 1 : 2;
    canvas.drawCircle(center, handleRadius + (handleRadius / 20) - borderWidth, valueBrush);

    //Тень
    final Paint shadowPaint = Paint()
      ..color = ColorControls.shadow.withValues(alpha: 0.9)
      ..maskFilter = const MaskFilter.blur(BlurStyle.normal, 20.0);
    canvas.drawCircle(center.translate(-center.dx / 5, 0), handleRadius - borderWidth, shadowPaint);

    //Градиент падения света на ручку
    final Paint fillBrush = Paint();
    fillBrush.shader = const LinearGradient(
      begin: Alignment.bottomRight,
      end: Alignment.bottomLeft,
      colors: [
        ColorControls.buttonUp,
        ColorControls.buttonDown,
      ],
    ).createShader(rect);
    canvas.drawCircle(center, handleRadius - borderWidth, fillBrush);

    //Свечение ручки сверху
    final Paint depressionBrush = Paint()
      ..shader = const LinearGradient(
        begin: Alignment.bottomRight,
        end: Alignment.bottomLeft,
        colors: [
          ColorControls.depressionDown,
          ColorControls.depressionUp,
        ],
      ).createShader(rect)
      ..style = PaintingStyle.stroke
      ..strokeWidth = handleRadius < 100 ? 1 : 2;
    canvas.drawCircle(center, handleRadius - borderWidth, depressionBrush);

    /*
                pi 3.14
                  180°
               *      *
            *            *
          *                *
    pi/2  90°               270° 3*pi/2
          *                *
            *            *
               *      *
                0° 360° 6.28
                 2*pi
     */

    //РИСУЕМ РУЧКУ
    //Получаем диапазон
    final double range = maxAngle - minAngle;
    //Радиус ручки
    final double handle = (handleRadius - borderWidth - handleRadius / 8);
    // Определяем текущий режим
    final bool isEncoderMode = controlMode == ControlMode.encoder;
    // Вычисляем угл (радиан) от текущего значения
    double valueRadian = ((isEncoderMode ? encValue : value) / steps) * range + minAngle;

    final Paint handleFillBrush = Paint()..color = ColorControls.notchActive;
    //Свечение насечек
    final Paint handleShine = Paint()
      ..color = ColorControls.notchActive
      ..maskFilter = const ui.MaskFilter.blur(BlurStyle.normal, 15);

    if (!isEncoderMode) {
      final double x = center.dx + handle * cos(valueRadian - pi);
      final double y = center.dy + handle * sin(valueRadian - pi);
      final double radius = handleRadius / 30;
      canvas.drawCircle(Offset(x, y), radius, handleFillBrush);
      canvas.drawCircle(Offset(x, y), radius, handleShine);
    } else {
      for (double i = minAngle; i <= maxAngle; i += pi / (encoderSteps / 2)) {
        final double point = valueRadian - pi + i;
        final double x = center.dx + handle * cos(point);
        final double y = center.dy + handle * sin(point);
        canvas.drawCircle(Offset(x, y), handleRadius / 60, handleFillBrush);
      }
    }

    if (!isEncoderMode) {
      // Рисуем насечки
      // Угол одного шага
      final double step = range / steps;

      // Радиус насечки внутренний
      final double innerRadius = handleRadius + pading;
      // Радиус насечки наружный
      final double outerRadius = innerRadius + notchesLenght;

      // Флаг для толщины линий в зависимости от размера ручки
      final bool small = handleRadius < 100;

      // Неактивная насечка
      final Paint notchInactive = Paint()
        ..color = ColorControls.notchNoActive
        ..style = PaintingStyle.stroke
        ..strokeCap = StrokeCap.round
        ..strokeWidth = small ? 1 : 2;

      // Активная насечка
      final Paint notchActive = Paint()
        ..color = ColorControls.notchActive
        ..style = PaintingStyle.stroke
        ..strokeCap = StrokeCap.round
        ..strokeWidth = small ? 2 : 4;

      // Свечение активной насечки
      final Paint notchGlow = Paint()
        ..color = ColorControls.notchActive
        ..style = PaintingStyle.stroke
        ..strokeCap = StrokeCap.round
        ..maskFilter = const ui.MaskFilter.blur(BlurStyle.normal, 7)
        ..strokeWidth = small ? 2 : 4;

      // Цикл по углам от minAngle до maxAngle с шагом step
      for (double angle = minAngle; angle <= maxAngle; angle += step) {
        final double a = angle - pi;
        final double cosA = cos(a);
        final double sinA = sin(a);

        // Координаты внешней точки насечки
        final Offset p1 = Offset(
          center.dx + outerRadius * cosA,
          center.dy + outerRadius * sinA,
        );

        // Координаты внутренней точки насечки
        final Offset p2 = Offset(
          center.dx + innerRadius * cosA,
          center.dy + innerRadius * sinA,
        );

        // Определяем активная ли насечка
        final bool isActive = angle <= valueRadian;

        // Рисуем основную линию
        canvas.drawLine(p1, p2, isActive ? notchActive : notchInactive);

        // Добавляем свечение для активной
        if (isActive) {
          canvas.drawLine(p1, p2, notchGlow);
        }
      }
    }
  }

  @override
  bool shouldRepaint(covariant KnopPainter old) => old.value != value;
}
