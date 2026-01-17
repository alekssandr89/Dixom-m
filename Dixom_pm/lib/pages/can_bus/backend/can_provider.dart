import 'package:dixom/dataBase/device_constant.dart';
import 'package:dixom/driver/device_rx_tx/transmiter.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

import 'can_const.dart';
import 'can_model.dart';

final canFramePrdr = StateNotifierProvider<CanModelNotifier, CanModel>((ref) => CanModelNotifier());

class CanModelNotifier extends StateNotifier<CanModel> {
  CanModelNotifier() : super(const CanModel());

  void frameUpdate() {
    int btUpdate = state.update;
    btUpdate++;
    state = state.copyWith(
      update: btUpdate,
    );
  }
}

final canStatePrdr = StateNotifierProvider<CanStateModelNotifier, CanStateModel>((ref) => CanStateModelNotifier());

class CanStateModelNotifier extends StateNotifier<CanStateModel> {
  CanStateModelNotifier() : super(const CanStateModel());

  void copyWith({
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
    state = state.copyWith(
      speed: speed ?? state.speed,
      autoConnect: autoConnect ?? state.autoConnect,
      dataOutPermision: dataOutPermision ?? state.dataOutPermision,
      txPermision: txPermision ?? state.txPermision,
      viewMode: viewMode ?? state.viewMode,
      framesInPacked: framesInPacked ?? state.framesInPacked,
      status: status ?? state.status,
      manufacturer: manufacturer ?? state.manufacturer,
      model: model ?? state.model,
      generation: generation ?? state.generation,
    );
  }

  void setCarManufacturer(int? manufacturer) {
    state = state.copyWith(manufacturer: manufacturer, model: 0, generation: 0);
    transmitter("3 6=${state.manufacturer} ${state.model} ${state.generation} 0 0 0 0");
  }

  void setCarModel(int? model) {
    state = state.copyWith(model: model, generation: 0);
    transmitter("3 6=${state.manufacturer} ${state.model} ${state.generation} 0 0 0 0");
  }

  void setCarGeneration(int? generation) {
    state = state.copyWith(generation: generation);
    transmitter("3 6=${state.manufacturer} ${state.model} ${state.generation} 0 0 0 0");
  }

  void setTxPermision() {
    if (state.txPermision) {
      state = state.copyWith(txPermision: false);
    } else {
      state = state.copyWith(txPermision: true);
    }
    transmitter("3 2=2 ${state.txPermision ? "1" : "0"}");
  }

  //Разрешение на получение данных из кан шины в интефейс
  void setDataOutPermision() {
    if (state.dataOutPermision) {
      state = state.copyWith(dataOutPermision: false);
    } else {
      state = state.copyWith(dataOutPermision: true);
    }
    transmitter("3 2=4 ${state.dataOutPermision ? "1" : "0"}");
  }

  //Настройка автоподключения при включении платформы
  void setAutoConnect() {
    if (state.autoConnect) {
      state = state.copyWith(autoConnect: false);
    } else {
      state = state.copyWith(autoConnect: true);
    }
    transmitter("3 2=3 ${state.autoConnect ? "1" : "0"}");
  }

  //Выставление количества кадров в одном пакете
  void setFramesInPacked(int? value) {
    state = state.copyWith(framesInPacked: value);
    transmitter("3 2=0 $value");
  }

  //Выставление скорости кан шины
  void setSpeed(int? value) {
    state = state.copyWith(speed: value);
    transmitter("3 4=$value");
  }

  //Подключение к кан шине
  void connecting() {
    if (state.status != WorkState.connected) {
      transmitter("3 3=2");
    } else {
      transmitter("3 3=4");
    }
  }
}
