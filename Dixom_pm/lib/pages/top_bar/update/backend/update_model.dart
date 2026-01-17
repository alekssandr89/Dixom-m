// ignore_for_file: public_member_api_docs, sort_constructors_first
import 'dart:convert';

import 'package:flutter/foundation.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:http/http.dart' as http;

import 'package:dixom/driver/device_rx_tx/transmiter.dart';
import 'package:dixom/main.dart';

import '../../device_info/backend/version.dart';
import 'update_const.dart';

final frimwareUpdateViewPrdr =
    StateNotifierProvider<UpdateViewNotifier, UpdateViewModel>((ref) => UpdateViewNotifier());

final frimwareInfoPrdr = FutureProvider((ref) {
  return FirmwareRepositiry().getFurmwareInfo(stableUrlDixomM);
});

class FirmwareRepositiry {
  Future<FirmwareModel> getFurmwareInfo(String url) {
    return http.get(Uri.parse(url)).then((value) => FirmwareModel.fromJson(value.body)).then((value) {
      String devVersion = appRef.read(deviceVersionPrdr.select((value) => value.verFrimware));

      UpdateStatus updateState = UpdateStatus.appNoUpdateRequired;

      if (devVersion != value.firmwareList[0].version) {
        updateState = UpdateStatus.appUpdateRequired;
      }

      appRef.read(frimwareUpdateViewPrdr.notifier).update(
            btnAction: UpdateBtnAction.download,
            status: updateState,
          );
      return value;
    });
  }
}

@immutable
class FirmwareModel {
  final String type;
  final String device;
  final String release;
  final List<FirmwareInfoModel> firmwareList;

  const FirmwareModel({required this.type, required this.device, required this.release, required this.firmwareList});

  FirmwareModel copyWith({
    String? type,
    String? device,
    String? release,
    List<FirmwareInfoModel>? firmwareList,
  }) {
    return FirmwareModel(
      type: type ?? this.type,
      device: device ?? this.device,
      release: release ?? this.release,
      firmwareList: firmwareList ?? this.firmwareList,
    );
  }

  factory FirmwareModel.fromMap(Map<String, dynamic> map) {
    return FirmwareModel(
      type: map['Type'] as String,
      device: map['Device'] as String,
      release: map['Release'] as String,
      firmwareList: (map["FrimwareList"] as List<dynamic>)
          .map((dynamic e) => FirmwareInfoModel.fromMap(e as Map<String, dynamic>))
          .toList(),
    );
  }

  factory FirmwareModel.fromJson(String source) {
    return FirmwareModel.fromMap(json.decode(source) as Map<String, dynamic>);
  }
}

@immutable
class FirmwareInfoModel {
  final String version;
  final String date;
  final String crc32;
  final String size;
  final String name;
  final String url;
  final String description;

  const FirmwareInfoModel(
      {required this.version,
      required this.date,
      required this.crc32,
      required this.size,
      required this.name,
      required this.url,
      required this.description});

  FirmwareInfoModel copyWith({
    String? version,
    String? date,
    String? crc32,
    String? size,
    String? name,
    String? url,
    String? description,
  }) {
    return FirmwareInfoModel(
      version: version ?? this.version,
      date: date ?? this.date,
      crc32: crc32 ?? this.crc32,
      size: size ?? this.size,
      name: name ?? this.name,
      url: url ?? this.url,
      description: description ?? this.description,
    );
  }

  factory FirmwareInfoModel.fromMap(Map<String, dynamic> map) {
    return FirmwareInfoModel(
      version: map['Version'] as String,
      date: map['Date'] as String,
      crc32: map['Crc32'] as String,
      size: map['Size'] as String,
      name: map['Name'] as String,
      url: map['Url'] as String,
      description: map['Description'] as String,
    );
  }

  factory FirmwareInfoModel.fromJson(String source) =>
      FirmwareInfoModel.fromMap(json.decode(source) as Map<String, dynamic>);
}

@immutable
class UpdateViewModel {
  final String version;
  final String description;
  final double progress;
  final UpdateBtnAction btnAction;
  final int updateUi;
  final UpdateStatus status;

  const UpdateViewModel({
    this.version = "",
    this.description = "",
    this.progress = 0,
    this.btnAction = UpdateBtnAction.getJson,
    this.updateUi = 0,
    this.status = UpdateStatus.appNoUpdateRequired,
  });

  UpdateViewModel copyWith({
    String? version,
    String? description,
    double? progress,
    UpdateBtnAction? btnAction,
    UpdateStatus? status,
    int? updateUi,
  }) {
    return UpdateViewModel(
      status: status ?? this.status,
      version: version ?? this.version,
      description: description ?? this.description,
      progress: progress ?? this.progress,
      btnAction: btnAction ?? this.btnAction,
      updateUi: updateUi ?? this.updateUi,
    );
  }
}

class UpdateViewNotifier extends StateNotifier<UpdateViewModel> {
  UpdateViewNotifier() : super(const UpdateViewModel());

  void update({
    String? version,
    String? description,
    double? progress,
    UpdateBtnAction? btnAction,
    UpdateStatus? status,
  }) {
    state = state.copyWith(
      version: version ?? state.version,
      description: description ?? state.description,
      progress: progress ?? state.progress,
      btnAction: btnAction ?? state.btnAction,
      status: status ?? state.status,
    );
  }

  void updateBootloader() {
    updateUi();
    transmitter("0 4 510286");
    transmitter("0 12");
    transmitter("0 4 0");
  }

  void updateUi() {
    int updateUi = state.updateUi;
    updateUi++;
    state = state.copyWith(updateUi: updateUi);
  }

  void updateBootloaderInfo(String data) {
    int updateUi = state.updateUi;
    updateUi++;
    state = state.copyWith(updateUi: updateUi);
  }
}
