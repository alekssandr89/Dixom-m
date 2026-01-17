// ignore_for_file: public_member_api_docs, sort_constructors_first
import 'package:dixom/dataBase/app_constants.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

//
enum MovingType { onlyHand, anywhere }

//Тип насечек слайдера
enum NotchMode { evenNotches, upNotches, differentNotches }

class SliderView extends StatefulWidget {
  const SliderView({
    super.key,
    required this.onChange,
    this.active = false, //активность слвйдера
    this.max = 15, //максимальное значение
    this.min = -15, //минимальное значение
    this.value = 0.0, // текущее значение
    this.handleOffset = 2, //Отступ ручки от края тела
    this.notchesNum = 9, //количество насечек
    this.accuracy = 0, //точность 0 = целое число, 1 = 0.1, 2 = 0.01
    this.handleHigh = 30, //высота ручки
    this.shineColor = ColorControls.activSlider, //свечение центра
    this.movingType = MovingType.onlyHand, //тип перемещения ручки
  });

  final bool active;
  final double accuracy;
  final double max;
  final Color shineColor;
  final double min;
  final double handleOffset;
  final int notchesNum;
  final double handleHigh;
  final double value;
  final MovingType movingType;
  final ValueChanged<double> onChange;

  @override
  State<SliderView> createState() => _SliderViewState();
}

class _SliderViewState extends State<SliderView> {
  bool permission = false;
  bool btnPresset = false;

  @override
  initState() {
    super.initState();
  }

  SystemMouseCursor cursor = SystemMouseCursors.basic;
  @override
  Widget build(BuildContext context) {
    double val = widget.value;
    return LayoutBuilder(builder: (BuildContext context, BoxConstraints constraints) {
      return MouseRegion(
        cursor: SystemMouseCursors.click,
        child: GestureDetector(
          onSecondaryTap: () {
            val = 0.0;
            widget.onChange(val);
            setState(() {});
            //  print("tap");
          },
          onDoubleTapCancel: () {
            val = 0.0;
            widget.onChange(val);
            setState(() {});
            //  print("tap");
          },
          onTap: () {
            widget.onChange(val);
          },
          onVerticalDragStart: (detail) {
            //Если пользователь нажимает в область ручки то разрешаем перемещение
            double halfHandleHigh = widget.handleHigh / 2;
            double offset = widget.handleOffset + halfHandleHigh;
            double sliderHigh = constraints.maxHeight - offset * 2;

            double y = offset + (val - widget.max) / (widget.min - widget.max) * sliderHigh;

            if (y < detail.localPosition.dy + halfHandleHigh && y > detail.localPosition.dy - halfHandleHigh) {
              permission = true;
            } else {
              permission = false;
            }
          },
          onVerticalDragUpdate: ((detail) {
            if (permission || widget.movingType == MovingType.anywhere) {
              double newValue = getValue(
                detail.localPosition.dy,
                constraints.maxHeight,
                widget.handleOffset + widget.handleHigh / 2,
              );

              if (widget.accuracy == 0) {
                newValue = newValue.round().toDouble();
                if (newValue.toInt() != val.toInt()) {
                  val = newValue;

                  widget.onChange(val);
                  setState(() {});
                }
              } else if (widget.accuracy == 1) {
                double accuracy = 10;
                if (widget.accuracy == 2) {
                  accuracy = 100;
                }
                double roundedVal = (val * accuracy).roundToDouble() / accuracy;
                double roundedNewValue = (newValue * accuracy).roundToDouble() / accuracy;

                if (roundedNewValue != roundedVal) {
                  val = newValue;
                  widget.onChange(val);
                  setState(() {});
                }
              }
            }
          }),
          child: Stack(
            children: [
              CustomPaint(
                size: Size(constraints.maxWidth, constraints.maxHeight),
                painter: SliderBody(
                  shineColor: widget.shineColor,
                  notchesNum: widget.notchesNum,
                  handleOffset: widget.handleOffset,
                  handleHigh: widget.handleHigh,
                  active: widget.active,
                ),
              ),
              CustomPaint(
                size: Size(constraints.maxWidth, constraints.maxHeight),
                painter: SliderHandle(
                  shineColor: widget.shineColor,
                  value: val,
                  btnPresset: btnPresset,
                  max: widget.max,
                  min: widget.min,
                  handleOffset: widget.handleOffset,
                  handleHigh: widget.handleHigh,
                ),
              ),
            ],
          ),
        ),
      );
    });
  }

  double getValue(double y, double maxHeight, double handleOffset) {
    final hight = maxHeight - handleOffset * 2;

    double newValue = widget.max + (y - handleOffset) / (hight - handleOffset) * (widget.min - widget.max);

    if (newValue > widget.max) {
      newValue = widget.max;
    } else if (newValue < widget.min) {
      newValue = widget.min;
    }
    return newValue;
  }

  double getPosition(double value, double maxHeight) {
    double position = widget.handleHigh +
        widget.handleOffset +
        (value - widget.max) / (widget.min - widget.max) * (maxHeight - widget.handleOffset * 2);

    return position;
  }
}

//Ручка слайдера
class SliderHandle extends CustomPainter {
  final bool btnPresset;
  final double max;
  final double min;
  final double value;
  final double handleOffset; //Отступ ползунка от края
  final double handleHigh; //Отступ ползунка от края
  final Color shineColor;

  SliderHandle({
    this.btnPresset = false,
    this.max = 10,
    this.min = -10,
    this.value = 0,
    this.handleHigh = 30,
    this.handleOffset = 2,
    required this.shineColor,
  });

  @override
  void paint(Canvas canvas, Size size) {
    const double radius = 10; //радиус скругления углов
    double halfHandleHigh = handleHigh / 2;
    double offset = handleOffset + halfHandleHigh; //Отступ ползунка от края
    double sliderHigh = size.height - offset * 2;
    double handlePosition = offset + (value - max) / (min - max) * sliderHigh;
    double centerX = size.width / 2;
    Rect rect = Rect.fromCenter(
      center: Offset(centerX, handlePosition),
      width: size.width,
      height: handleHigh,
    );
    RRect line = RRect.fromRectAndRadius(
      Rect.fromLTWH(
        handleOffset,
        handlePosition - halfHandleHigh,
        size.width - handleOffset * 2,
        handleHigh,
      ),
      const Radius.circular(radius * 0.8),
    );

    //Тень ручки
    final shadowPaint = Paint()
      ..strokeWidth = 10
      ..color = ColorControls.shadow
      ..maskFilter = const MaskFilter.blur(BlurStyle.normal, 4.0);

    var p1 = Offset(5, handlePosition + halfHandleHigh);
    var p2 = Offset(size.width - 5, handlePosition + halfHandleHigh);
    canvas.drawLine(p1, p2, shadowPaint);

    //Корпус ручки
    var fillBrush = Paint();
    fillBrush.shader = const LinearGradient(
      begin: Alignment.topCenter,
      end: Alignment.bottomCenter,
      stops: [0.1, 0.9],
      colors: [ColorControls.buttonUp, ColorControls.buttonDown],
    ).createShader(rect);

    canvas.drawRRect(line, fillBrush);

    //Блеск ручки
    var valueBrush = Paint()
      ..shader = const LinearGradient(
        begin: Alignment.topCenter,
        end: Alignment.bottomCenter,
        stops: [0.1, 0.4],
        colors: [ColorControls.buttonShineUp, Color.fromARGB(255, 54, 63, 92)],
      ).createShader(rect)
      ..style = PaintingStyle.stroke
      ..strokeWidth = 1.5;
    canvas.drawRRect(line, valueBrush);

    //Полоска по центру
    final handleCenter = Paint()
      ..strokeWidth = 3
      ..color = const Color.fromARGB(255, 253, 253, 253).withOpacity(0.8);

    p1 = Offset(handleOffset + 3, handlePosition);
    p2 = Offset(size.width - handleOffset * 3, handlePosition);
    canvas.drawLine(p1, p2, handleCenter);
  }

  @override
  bool shouldRepaint(SliderHandle old) => old.value != value;
}

//Тело слайдера
class SliderBody extends CustomPainter {
  final NotchMode notchesMode;
  final int notchesNum; //Количество насечек
  final double handleOffset; //Отступ ползунка от края
  final double handleHigh; //Отступ ползунка от края
  final bool active;
  final Color shineColor;
  const SliderBody({
    this.notchesMode = NotchMode.evenNotches,
    this.notchesNum = 10,
    this.handleOffset = 3,
    this.handleHigh = 30,
    required this.shineColor,
    this.active = false,
  });

  @override
  void paint(Canvas canvas, Size size) {
    const double radius = 10; //радиус скругления углов
    double offset = handleOffset + handleHigh / 2;
    const double slitWidth = 3.5; //Толщина щели
    const double slitEdgeWidth = 5; //Толщина границы щели

    const int notchSpacing = 6; //Отступ от края щели до насечек

    var centerX = size.width / 2;
    var centerY = size.height / 2;
    var center = Offset(size.width / 2, centerY);
    Rect rect = Rect.fromCenter(
      center: center,
      width: size.width,
      height: size.height,
    );

    paintBorder(canvas, rect, radius, size);

    double sliderHigh = size.height - offset * 2;
    double sliderEnd = sliderHigh + offset;

    //Вырез щели
    final slit = Paint()
      ..strokeWidth = slitWidth * 2
      ..color = active == false ? ColorControls.unActivSlider : shineColor;

    canvas.drawLine(Offset(centerX, offset), Offset(centerX, sliderEnd), slit);

    //Граница щели
    RRect line = RRect.fromRectAndRadius(
      Rect.fromLTWH(centerX - slitWidth, offset - slitEdgeWidth / 2, slitWidth * 2, sliderHigh + slitEdgeWidth),
      const Radius.circular(10),
    );
    var slitBoundary = Paint()
      ..color = const Color.fromARGB(255, 34, 40, 54)
      ..style = PaintingStyle.stroke
      ..strokeWidth = slitEdgeWidth;
    canvas.drawRRect(line, slitBoundary);

    // насечки
    var notch = Paint()
      ..color = ColorControls.notchNoActive
      ..style = PaintingStyle.stroke
      ..strokeWidth = 1;

    double notchLen = size.width / 4.3;
    if (size.width < 40) {
      notchLen = size.width / 20.3;
    }
    double notchHight = (sliderHigh) / (notchesNum - 1);
    double leftX1 = centerX - notchSpacing - slitWidth - notchLen;
    double leftX2 = centerX - notchSpacing - slitWidth;
    double rightX1 = centerX + notchSpacing + slitWidth + notchLen;
    double rightX2 = centerX + notchSpacing + slitWidth;

    for (int i = 0; i < notchesNum; i++) {
      if (notchesMode == NotchMode.evenNotches) {
        var y1 = offset + notchHight * i;
        var y2 = offset + notchHight * i;
        canvas.drawLine(Offset(leftX1, y1), Offset(leftX2, y2), notch);
        canvas.drawLine(Offset(rightX1, y1), Offset(rightX2, y2), notch);
      } else if (notchesMode == NotchMode.upNotches) {}
    }
  }

  void paintBorder(Canvas canvas, Rect rect, double radius, Size size) {
    //Впадина
    var fillBrush = Paint()..color = const Color.fromARGB(255, 52, 62, 83);
    RRect line = RRect.fromRectAndRadius(
      Rect.fromLTWH(0, 0, size.width, size.height),
      Radius.circular(radius),
    );
    canvas.drawRRect(line, fillBrush);

    //Рамка
    var frameBrush = Paint()
      ..style = PaintingStyle.stroke
      ..color = ColorControls.buttonDown;
    canvas.drawRRect(line, frameBrush);

    //свечение впадины сверху снизу
    var valueBrush = Paint()
      ..shader = LinearGradient(
        begin: Alignment.topCenter,
        end: Alignment.bottomCenter,
        stops: const [0, 0.05],
        colors: [const Color.fromARGB(255, 26, 25, 25).withOpacity(0.4), ColorControls.buttonDown.withOpacity(0.1)],
      ).createShader(rect)
      ..style = PaintingStyle.stroke
      ..strokeWidth = 1;
    canvas.drawRRect(line, valueBrush);

    line = RRect.fromRectAndRadius(
      Rect.fromLTWH(0, 1, size.width, size.height),
      Radius.circular(radius),
    );
    canvas.drawRRect(line, fillBrush);

    //свечение впадины снизу вверх
    var shineDownBrush = Paint()
      ..shader = LinearGradient(
        begin: Alignment.topCenter,
        end: Alignment.bottomCenter,
        stops: const [0.95, 1],
        colors: [
          ColorControls.buttonDown.withOpacity(0.1),
          ColorControls.buttonShineUp,
        ],
      ).createShader(rect)
      ..style = PaintingStyle.stroke
      ..strokeWidth = 1;
    canvas.drawRRect(line, shineDownBrush);
  }

  @override
  bool shouldRepaint(SliderBody old) => false;
}
