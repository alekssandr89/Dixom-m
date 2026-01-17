import 'dart:core';
import 'dart:math';

enum DspCrossoverType {
  highPass,
  lowPass,
}

enum DspFilterType {
  off, //GOOD
  peak, //GOOD
  lowPass, //GOOD
  highPass, //GOOD
  butterworthLowPass, //GOOD
  butterworthHighPass, //GOOD
  besselLowPass, //GOOD
  besselHighPass, //GOOD
  lowPassFirst, //GOOD
  highPassFirst, //GOOD
  lowShelf, //GOOD
  highShelf, //GOOD
  linkwitzRileyHighPass, //NO GOOD
  linkwitzRileyLowPass, //NO GOOD
  bandPass, //NO GOOD
  bandStop, //NO GOOD
}

class DspFilter {
  final int freq; //Частота фильтра
  final double q; //добротность
  final double boost; //усиление
  final DspFilterType type; //тип фильтра
  final int order; //порядок фильтра
  final bool status; //состояние фильтра

  const DspFilter(this.freq, this.q, this.boost, this.type, this.order, this.status);
}

const DspFilter hiPassDefault = DspFilter(70, 0.71, 0.0, DspFilterType.butterworthHighPass, 2, false);

const DspFilter lowPassDefault = DspFilter(5000, 0.71, 0.0, DspFilterType.butterworthLowPass, 2, false);

const List<DspFilter> eqSourceDefault = [
  DspFilter(60, 0.71, 0.0, DspFilterType.peak, 0, true),
  DspFilter(260, 0.71, 0.0, DspFilterType.peak, 0, true),
  DspFilter(800, 0.71, 0.0, DspFilterType.peak, 0, true),
  DspFilter(4000, 0.71, 0.0, DspFilterType.peak, 0, true),
  DspFilter(16000, 0.71, 0.0, DspFilterType.peak, 0, true),
];

const List<DspFilter> eqDefault = [
  DspFilter(25, 2.5, 0.0, DspFilterType.peak, 0, true),
  DspFilter(40, 2.5, 0.0, DspFilterType.peak, 0, true),
  DspFilter(60, 2.5, 0.0, DspFilterType.peak, 0, true),
  DspFilter(96, 2.5, 0.0, DspFilterType.peak, 0, true),
  DspFilter(160, 2.5, 0.0, DspFilterType.peak, 0, true),
  DspFilter(260, 2.5, 0.0, DspFilterType.peak, 0, true),
  DspFilter(425, 2.5, 0.0, DspFilterType.peak, 0, true),
  DspFilter(700, 2.5, 0.0, DspFilterType.peak, 0, true),
  DspFilter(1100, 2.5, 0.0, DspFilterType.peak, 0, true),
  DspFilter(1700, 2.5, 0.0, DspFilterType.peak, 0, true),
  DspFilter(2700, 2.5, 0.0, DspFilterType.peak, 0, true),
  DspFilter(4300, 2.5, 0.0, DspFilterType.peak, 0, true),
  DspFilter(7300, 2.5, 0.0, DspFilterType.peak, 0, true),
  DspFilter(12000, 2.5, 0.0, DspFilterType.peak, 0, true),
  DspFilter(18000, 2.5, 0.0, DspFilterType.peak, 0, true),
];

class BiQuadraticFilter {
  double a0 = 1;
  double a1 = 0;
  double a2 = 0;
  double b0 = 1;
  double b1 = 0;
  double b2 = 0;
  double q;
  double boost;
  int order;
  int frequency = 20;
  int sampleRate;
  bool state;
  DspFilterType type = DspFilterType.peak;

  BiQuadraticFilter({
    required this.frequency,
    this.q = 1.41,
    this.boost = 0,
    this.order = 2,
    this.sampleRate = 48000,
    this.state = true,
    this.type = DspFilterType.peak,
  }) {
    reconfigure();
  }

  // allow parameter change while running
  void reconfigure() {
    num gainAbs = pow(10, boost / 40);
    double omega = 2 * pi * frequency / sampleRate;
    double sn = sin(omega);
    double cs = cos(omega);
    double alpha;

    switch (type) {
      case DspFilterType.off:
        break;
      case DspFilterType.peak:
        alpha = sn / (2 * ((q == 0) ? 1e-9 : q));

        b0 = (1 + (alpha * gainAbs));
        b1 = -2 * cs;
        b2 = (1 - (alpha * gainAbs));
        a0 = 1 + (alpha / gainAbs);
        a1 = -2 * cs;
        a2 = 1 - (alpha / gainAbs);
        break;
      /* case DspFilterType.lowPass:
        alpha = sn / (2 * ((q == 0) ? 1e-9 : q));
        b0 = (1 - cs) / 2;
        b1 = 1 - cs;
        b2 = (1 - cs) / 2;
        a0 = 1 + alpha;
        a1 = -2 * cs;
        a2 = 1 - alpha;
        break;
      case DspFilterType.highPass:
        alpha = sn / (2 * ((q == 0) ? 1e-9 : q));
        a0 = 1 + alpha;
        a1 = -2 * cs;
        a2 = 1 - alpha;
        b0 = (1 + cs) / 2;
        b1 = -(1 + cs);
        b2 = (1 + cs) / 2;
        break;*/
      case DspFilterType.butterworthLowPass:
        alpha = sn / (2.0 * 1 / sqrt(2));
        a0 = 1 + alpha;
        a1 = -2 * cs;
        a2 = 1 - alpha;
        b0 = (1 - cs) / 2;
        b1 = 1 - cs;
        b2 = (1 - cs) / 2;
        break;
      case DspFilterType.butterworthHighPass:
        alpha = sn / (2.0 * 1 / sqrt(2));
        a0 = 1 + alpha;
        a1 = -2 * cs;
        a2 = 1 - alpha;
        b0 = (1 + cs) / 2;
        b1 = -(1 + cs);
        b2 = (1 + cs) / 2;
        break;
      case DspFilterType.besselLowPass:
        alpha = sn / (2.0 * 1 / sqrt(3));
        a0 = 1 + alpha;
        a1 = -2 * cs;
        a2 = 1 - alpha;
        b0 = (1 - cs) / 2;
        b1 = 1 - cs;
        b2 = (1 - cs) / 2;
        break;
      case DspFilterType.besselHighPass:
        alpha = sn / (2.0 * 1 / sqrt(3));
        a0 = 1 + alpha;
        a1 = -2 * cs;
        a2 = 1 - alpha;
        b0 = (1 + cs) / 2;
        b1 = -(1 + cs);
        b2 = (1 + cs) / 2;
        break;
      case DspFilterType.lowPassFirst:
        a0 = 1;
        a1 = pow(2.718, (-2 * pi * frequency / sampleRate)).toDouble();
        a2 = 0;
        b0 = 1.0 - a1;
        b1 = 0;
        b2 = 0;
        break;
      case DspFilterType.highPassFirst:
        a0 = 1;
        a1 = pow(2.718, (-2 * pi * frequency / sampleRate)).toDouble();
        a2 = 0;
        b0 = a1;
        b1 = -a1;
        b2 = 0;
        /* a0 = 1;
        a1 = Math.Pow(2.7, -omega);
        a2 = 0;
        b0 = a1;
        b1 = -a1;
        b2 = 0;*/
        break;
      /*case DspFilterType.lowShelf:
        double beta = 0;
        alpha = sn / (2 * Q);
        b0 = gainAbs * ((gainAbs + 1) - (gainAbs - 1) * cs + beta * sn);
        b1 = 2 * gainAbs * ((gainAbs - 1) - (gainAbs + 1) * cs);
        b2 = gainAbs * ((gainAbs + 1) - (gainAbs - 1) * cs - beta * sn);
        a0 = (gainAbs + 1) + (gainAbs - 1) * cs + beta * sn;
        a1 = -2 * ((gainAbs - 1) + (gainAbs + 1) * cs);
        a2 = (gainAbs + 1) + (gainAbs - 1) * cs - beta * sn;
        break;
      case DspFilterType.highShelf:
        double beta = 0;
        alpha = sn / (2 * Q);
        b0 = gainAbs * ((gainAbs + 1) + (gainAbs - 1) * cs + beta * sn);
        b1 = -2 * gainAbs * ((gainAbs - 1) + (gainAbs + 1) * cs);
        b2 = gainAbs * ((gainAbs + 1) + (gainAbs - 1) * cs - beta * sn);
        a0 = (gainAbs + 1) - (gainAbs - 1) * cs + beta * sn;
        a1 = 2 * ((gainAbs - 1) - (gainAbs + 1) * cs);
        a2 = (gainAbs + 1) - (gainAbs - 1) * cs - beta * sn;
        break;
      case DspFilterType.linkwitzRileyHighPass:
        alpha = sn / (2 * Q);
        a0 = 1;
        a1 = 0;
        a2 = 0;
        b0 = 1;
        b1 = 0;
        b2 = 0;
        break;
      case DspFilterType.linkwitzRileyLowPass:
        alpha = sn / (2 * Q);
        a0 = 1;
        a1 = 0;
        a2 = 0;
        b0 = 1;
        b1 = 0;
        b2 = 0;
        break;
      case DspFilterType.bandPass:
        alpha = sn / (2 * Q);
        b0 = alpha;
        b1 = 0;
        b2 = -alpha;
        a0 = 1 + alpha;
        a1 = -2 * cs;
        a2 = 1 - alpha;
        break;
      case DspFilterType.bandStop:
        alpha = sn / (2 * Q);
        b0 = 1;
        b1 = -2 * cs;
        b2 = 1;
        a0 = 1 + alpha;
        a1 = -2 * cs;
        a2 = 1 - alpha;
        break;*/
      default:
        break;
    }
    // prescale flter constants
    b0 /= a0;
    b1 /= a0;
    b2 /= a0;
    a1 /= a0;
    a2 /= a0;
  }

  // provide a static amplitude result for testing
  double result(int feq) {
    double phi = pow((sin(2.0 * pi * feq / (2.0 * sampleRate))), 2.0).toDouble();
    double r = (pow(b0 + b1 + b2, 2.0) - 4.0 * (b0 * b1 + 4.0 * b0 * b2 + b1 * b2) * phi + 16.0 * b0 * b2 * phi * phi) /
        (pow(1.0 + a1 + a2, 2.0) - 4.0 * (a1 + 4.0 * a2 + a1 * a2) * phi + 16.0 * a2 * phi * phi);
    if (r < 0) {
      r = 0;
    }
    return sqrt(r);
  }

  // provide a static decibel result for testing
  double logResult(int feq) {
    double r;
    if (!state) {
      r = 0;
    } else {
      try {
        r = 20 * (log(result(feq)) / ln10);
      } catch (e) {
        r = -100;
      }
      if (r.isInfinite || r.isNaN) {
        r = -100;
      }
    }
    return r;
  }
}
