import 'package:dixom/driver/device_rx_tx/command_msg.dart';
import 'dac_info.dart';
import 'license.dart';
import 'life.dart';
import 'usb_sound_info.dart';
import 'version.dart';

enum Information {
  deviceVersion("Device version"), // 0 Информация о версии прошики
  license("License"), // Ключь лицензии и серийный номер
  deviceLife("Device life"), // Раличные показатели работы платформы
  usbSoundVersion("Sound cadr version"), // Версии прошивки звуковой карты
  dacInfromation("Dac information"), // Информация о цап или усилителях
  newBootloaderVersion("New bootloader"); // Если есть новая прошивка для загрузкчика то она вернется сюда

  final String decoding;
  const Information(this.decoding);
}

class DeviceInformation {
  DeviceVersion version = DeviceVersion();
  DeviceLicense license = DeviceLicense();
  DeviceLife life = DeviceLife();
  DacInformation dacInformation = DacInformation();
  UsbSoundInformation usbSoundInformation = UsbSoundInformation();

  void setDefault() {
    version.setDefault();
    license.setDefault();
    life.setDefault();
    dacInformation.setDefault();
    usbSoundInformation.setDefault();
  }

  void receiver(CommandMsg msg) {
    if (msg.podParmetr == Information.deviceVersion.index) {
      version.deviceVeceiver(msg);
    } else if (msg.podParmetr == Information.license.index) {
      license.receiver(msg);
    } else if (msg.podParmetr == Information.deviceLife.index) {
      life.receiver(msg);
    } else if (msg.podParmetr == Information.usbSoundVersion.index) {
      usbSoundInformation.receiver(msg);
    } else if (msg.podParmetr == Information.dacInfromation.index) {
      dacInformation.receiver(msg);
    } else if (msg.podParmetr == Information.newBootloaderVersion.index) {
      version.bootloaderNewVersion(msg);
    }
  }
}
