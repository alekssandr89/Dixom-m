import 'package:flutter/material.dart';
import 'package:google_fonts/google_fonts.dart';

class Frame extends StatelessWidget {
  final String text;
  final int steps;
  const Frame({
    super.key,
    this.text = "",
    this.steps = 0,
  });

  @override
  Widget build(BuildContext context) {
    return LayoutBuilder(
      builder: (BuildContext context, BoxConstraints constraints) {
        return SizedBox(
          width: constraints.maxWidth,
          height: constraints.maxHeight,
          child: CustomPaint(
            painter: FramePainter(steps: steps, text: text),
          ),
        );
      },
    );
  }
}

class FramePainter extends CustomPainter {
  final String text;
  final int steps;

  FramePainter({
    required this.text,
    required this.steps,
  });

  @override
  void paint(Canvas canvas, Size size) {
    int up = 40;
    RRect line = RRect.fromRectAndRadius(
      Rect.fromLTWH(-up / 2, -up / 2, size.width + up, size.height + up),
      const Radius.circular(30),
    );

    var frameBrush = Paint()
      ..color = const Color.fromARGB(132, 122, 124, 131)
      ..style = PaintingStyle.stroke
      ..strokeWidth = 2;

    canvas.drawRRect(line, frameBrush);
    paintText(text, 15, canvas, 230, size.height - 5);
  }

  void paintText(String text, double fontSize, Canvas canvas, double width, double height,
      {Color color = const Color.fromARGB(255, 185, 193, 197)}) {
    final textSpan = TextSpan(
      text: text,
      style: GoogleFonts.alatsi(
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
  bool shouldRepaint(CustomPainter oldDelegate) => true;
}
