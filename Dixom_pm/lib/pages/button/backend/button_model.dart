import 'package:flutter/material.dart';

class OneButton {
  int channel;
  int id;
  int interval;
  int action;
  int doublAction;
  int delayAction;
  int delayTiming;
  int dobleTiming;
  int action2;
  int doubleAction2;
  int delayAtion2;
  int param1;
  int param2;
  int param3;
  int param4;
  String name;
  OneButton({
    required this.channel,
    required this.id,
    required this.interval,
    required this.action,
    required this.delayAction,
    required this.doublAction,
    required this.delayTiming,
    required this.dobleTiming,
    required this.action2,
    required this.delayAtion2,
    required this.doubleAction2,
    required this.param1,
    required this.param2,
    required this.param3,
    required this.param4,
    this.name = "",
  });
}

@immutable
class ExternalControlModel {
  final int btUpdate; //При изменении будет обновлен интерфейс

  const ExternalControlModel({
    this.btUpdate = 0,
  });

  ExternalControlModel copyWith({
    int? btUpdate,
  }) {
    return ExternalControlModel(
      btUpdate: btUpdate ?? this.btUpdate,
    );
  }
}
