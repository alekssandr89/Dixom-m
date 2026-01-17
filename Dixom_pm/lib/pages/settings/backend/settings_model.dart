// ignore_for_file: public_member_api_docs, sort_constructors_first
import 'package:flutter/material.dart';

@immutable
class SettingsModel {
  final int update;

  const SettingsModel({
    this.update = 0,
  });

  SettingsModel copyWith({
    int? update,
  }) {
    return SettingsModel(
      update: update ?? this.update,
    );
  }
}

class CanCarModel {
  final int manufacturer;
  final int model;
  final int generation;
  CanCarModel(this.manufacturer, this.model, this.generation);
}

@immutable
class RtcModel {
  final DateTime dataTime;
  final double temperature;

  const RtcModel({
    required this.dataTime,
    this.temperature = 25.5,
  });

  RtcModel copyWith({
    DateTime? dataTime,
    double? temperature,
  }) {
    return RtcModel(
      dataTime: dataTime ?? this.dataTime,
      temperature: temperature ?? this.temperature,
    );
  }
}
