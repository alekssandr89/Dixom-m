// ignore_for_file: public_member_api_docs, sort_constructors_first
import 'dart:typed_data';

import 'package:flutter/material.dart';

import 'package:dixom/dataBase/device_constant.dart';

import 'can_const.dart';

@immutable
class CanModel {
  final int update;

  const CanModel({
    this.update = 0,
  });

  CanModel copyWith({
    int? update,
  }) {
    return CanModel(
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
class CanStateModel {
  final WorkState status;
  final int framesInPacked;
  final int speed;
  final bool txPermision;
  final bool autoConnect;
  final bool dataOutPermision;
  final ViewMode viewMode;
  final int manufacturer;
  final int model;
  final int generation;

  const CanStateModel({
    this.status = WorkState.disconnected,
    this.framesInPacked = defFramesInPackedIndex,
    this.speed = defCanSpeedIndex,
    this.txPermision = false,
    this.autoConnect = false,
    this.dataOutPermision = false,
    this.viewMode = ViewMode.monitor,
    this.manufacturer = 0,
    this.model = 0,
    this.generation = 0,
  });

  CanStateModel copyWith({
    WorkState? status,
    int? framesInPacked,
    int? speed,
    bool? txPermision,
    bool? autoConnect,
    bool? dataOutPermision,
    ViewMode? viewMode,
    int? manufacturer,
    int? model,
    int? generation,
  }) {
    return CanStateModel(
      speed: speed ?? this.speed,
      autoConnect: autoConnect ?? this.autoConnect,
      dataOutPermision: dataOutPermision ?? this.dataOutPermision,
      txPermision: txPermision ?? this.txPermision,
      viewMode: viewMode ?? this.viewMode,
      framesInPacked: framesInPacked ?? this.framesInPacked,
      status: status ?? this.status,
      manufacturer: manufacturer ?? this.manufacturer,
      model: model ?? this.model,
      generation: generation ?? this.generation,
    );
  }
}

///Одно сообщение
class CanFrame {
  CanIdType idType;
  int id;
  int dlc;
  Duration period;
  int framesCount;
  Uint8List buffer;
  DateTime time;
  CanFrame({
    required this.idType,
    required this.id,
    required this.dlc,
    required this.period,
    required this.framesCount,
    required this.buffer,
    required this.time,
  });
}

//Список производителей машин
class CarManufacturer {
  String name;
  List<CarModel> list;
  CarManufacturer(
    this.name,
    this.list,
  );
}

//Список марок у производителей
class CarModel {
  String name;
  List<CarGeneration> list;
  CarModel(
    this.name,
    this.list,
  );
}

//Поколение марки автомобиля у производителей
class CarGeneration {
  String generation;

  CarGeneration(
    this.generation,
  );
}
