import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import '../../../../dataBase/app_constants.dart';
import '../../../../components/widgets/decorated_container.dart';
import '../backend/license.dart';
import '../backend/life.dart';
import '../backend/usb_sound_info.dart';
import '../backend/version.dart';

class InformationView extends ConsumerWidget {
  const InformationView({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final version = ref.watch(deviceVersionPrdr);
    final license = ref.watch(deviceLicensePrdr);
    final defLife = ref.watch(deviceLifePrdr);
    final usbCard = ref.watch(usbSoundInfoPrdr);

    return SingleChildScrollView(
      child: Column(
        children: [
          DecoratedContainer(
            top: 0,
            left: AppIndents.pading,
            right: AppIndents.pading,
            bottom: AppIndents.pading,
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                const Text(
                  "Dixom PM v2.0.6",
                  style: TextStyle(fontSize: 16, color: Colors.white70),
                ),
                const Text(
                  "Информация о аудиоплатформе",
                  style: TextStyle(fontSize: 18, color: Colors.white70),
                ),
                SizedBox(
                  width: double.infinity,
                  child: DataTable(
                    horizontalMargin: 0,
                    columns: const [DataColumn(label: Text("")), DataColumn(label: Text(""))],
                    rows: [
                      addDataRow("Серийный номер", license.serialNumber),
                      addDataRow("Ключь активации", license.licenseKey),
                      addDataRow("Ревизия печатной платы", version.revPCB),
                      addDataRow("Тип устройства", version.type),
                      addDataRow("Модель", version.model),
                      addDataRow("Название прошивки", version.firmwareName),
                      addDataRow("Версия прошивки", version.verFrimware),
                      addDataRow("Версия загрузчика", version.verBootloader),
                      addDataRow("Записей в FLASH", defLife.numFlashWrite),
                      addDataRow("Записей в EEPROM", defLife.numEepromWrite),
                      addDataRow("Usb карта", usbCard.verFrimware),
                      addDataRow("Загрузчик usb карты", usbCard.verBootloader),
                      addDataRow("Записей в EEPROM", defLife.numEepromWrite),
                      addDataRow("Наработка платформы (сек)", defLife.workingHours),
                    ],
                  ),
                )
              ],
            ),
          ),
        ],
      ),
    );
  }

  DataRow addDataRow(String parameter, String value) {
    return DataRow(
      cells: [
        DataCell(Padding(
            padding: const EdgeInsets.symmetric(horizontal: AppIndents.pading),
            child: Text(parameter, style: const TextStyle(fontSize: 14, color: Colors.white70)))),
        DataCell(Text(value, style: const TextStyle(fontSize: 14, color: Colors.white70)))
      ],
    );
  }
}
