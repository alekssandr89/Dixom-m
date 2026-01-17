import 'dart:async';
import 'package:dixom/pages/settings/backend/settings_reciever.dart';
import 'package:dixom/pages/top_bar/device_info/backend/reciever.dart';
import 'package:dixom/pages/top_bar/log/backend/device_log.dart';
import 'package:dixom/pages/top_bar/register/backend/register.dart';
import 'package:dixom/pages/top_bar/update/backend/update_isolate.dart';
import '../../pages/button/backend/button_reciever.dart';
import '../../pages/can_bus/backend/can_reciever.dart';
import '../../pages/audio/backend/sound.dart';
import '../../pages/fm_radio/backend/radio_rciever.dart';
import 'package:dixom/pages/top_bar/console/console.dart';
import '../../pages/top_bar/connection/backend/device_connection.dart';
import 'command_msg.dart';
import 'exchange_param.dart';

final Distributor distributor = Distributor();

class Distributor {
  void initialization() {}

  System system = System();
  Sound sound = Sound();
  RxGeneral general = RxGeneral();
  ExternalControl externalControl = ExternalControl();
  CanBus canBus = CanBus();
  FmRadio fmRadio = FmRadio();
  void setDefault() {
    system.setDefault();
    sound.setDefault();
    externalControl.setDefault();
    canBus.setDefault();
    fmRadio.setDefault();
    general.setDefault();
  }

  RxController rxController = RxController();
  void receiver(CommandMsg msg) {
    rxController.addMessage(msg);
    if (msg.parametr != 0) {
      dataConsole.addMessage(msg);
    }
    if (msg.packed == RootPacked.system.index) {
      system.receiver(msg);
    } else if (msg.packed == RootPacked.general.index) {
      general.receiver(msg);
    } else if (msg.packed == RootPacked.gateway.index) {
    } else if (msg.packed == RootPacked.canBus.index) {
      canBus.receiver(msg);
    } else if (msg.packed == RootPacked.sound.index) {
      sound.receiver(msg);
    } else if (msg.packed == RootPacked.bluetooth.index) {
    } else if (msg.packed == RootPacked.extControl.index) {
      externalControl.receiver(msg);
    } else if (msg.packed == RootPacked.radio.index) {
      fmRadio.receiver(msg);
    }
  }
}

class System {
  DeviceInformation information = DeviceInformation();

  void setDefault() {
    information.setDefault();
  }

  void receiver(CommandMsg msg) {
    if (msg.parametr == SystemPacked.notification.index) {
      updateNotification(msg);
    } else if (msg.parametr == SystemPacked.information.index) {
      information.receiver(msg);
    } else if (msg.parametr == SystemPacked.accesControl.index) {
    } else if (msg.parametr == SystemPacked.txControl.index) {
      setConnectionState(msg);
    }
  }
}

class RxGeneral {
  RxEchangeLog log = RxEchangeLog();
  RxRegister register = RxRegister();
  GeneralSettings settings = GeneralSettings();
  Rtc rtc = Rtc();

  void setDefault() {
    settings.setDefault();
    rtc.setDefault();
  }

  void receiver(CommandMsg msg) {
    if (msg.parametr == General.notification.index) {
    } else if (msg.parametr == General.message.index) {
    } else if (msg.parametr == General.settings.index) {
      settings.receiver(msg);
    } else if (msg.parametr == General.log.index) {
      log.receiver(msg);
    } else if (msg.parametr == General.register.index) {
      register.receiver(msg);
    } else if (msg.parametr == General.dataSave.index) {
    } else if (msg.parametr == General.dataTime.index) {
      rtc.receiver(msg);
    } else if (msg.parametr == General.action.index) {
    } else if (msg.parametr == General.spiStatus.index) {
    } else if (msg.parametr == General.usbPwrControl.index) {
    } else if (msg.parametr == General.dataTime.index) {}
  }
}

class RxController {
  StreamController<CommandMsg> msg = StreamController<CommandMsg>.broadcast(sync: false);
  Stream<CommandMsg> get stream => msg.stream;

  void addMessage(CommandMsg data) {
    msg.sink.add(data);
  }

  void dispose() {
    msg.close();
  }
}
