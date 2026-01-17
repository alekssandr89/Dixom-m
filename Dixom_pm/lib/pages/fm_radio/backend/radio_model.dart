import 'package:flutter/material.dart';

class FmPreset {
  double freqency;
  String name;
  FmPreset({
    required this.freqency,
    required this.name,
  });
}

@immutable
class FmRadioModel {
  final int rssi;
  final int snr;
  final int mult;
  final int stereo;
  final int stereoBlend;
  final int freqOff;
  final int preset;
  final double freqency;
  final List<FmPreset> presetList;

  const FmRadioModel({
    this.rssi = 0,
    this.snr = 0,
    this.mult = 0,
    this.stereo = 0,
    this.stereoBlend = 0,
    this.freqOff = 0,
    this.preset = 0,
    this.freqency = 87.5,
    required this.presetList,
  });

  FmRadioModel copyWith({
    int? rssi,
    int? snr,
    int? mult,
    int? stereo,
    int? stereoBlend,
    int? freqOff,
    int? preset,
    double? freqency,
    List<FmPreset>? presetList,
  }) {
    return FmRadioModel(
      rssi: rssi ?? this.rssi,
      snr: snr ?? this.snr,
      mult: mult ?? this.mult,
      stereo: stereo ?? this.stereo,
      stereoBlend: stereoBlend ?? this.stereoBlend,
      freqOff: freqOff ?? this.freqOff,
      preset: preset ?? this.preset,
      freqency: freqency ?? this.freqency,
      presetList: presetList ?? this.presetList,
    );
  }
}
