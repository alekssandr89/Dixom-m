import 'package:dixom/dataBase/app_constants.dart';
import 'package:dixom/driver/android/android_service_model.dart';
import 'package:dixom/driver/android/service_channel.dart';
import 'package:dixom/pages/top_bar/connection/backend/device_connection.dart';
import 'package:dixom/dataBase/device_constant.dart';
import 'package:dixom/components/widgets/connection.dart';
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:permission_handler/permission_handler.dart';

class ConnectionSettingsAndroid extends ConsumerWidget {
  const ConnectionSettingsAndroid({
    super.key,
  });

  void btnClick() {}

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final response = ref.watch(serviceConfiguration);

    return response.when(
      data: (data) {
        return Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text(
              "Настройка сервиса Dixom",
              style: TextStyle(fontSize: 20, color: Colors.white70),
            ),
            const Divider(),
            SettingsChecked(
              parrametr: "Авто-подключение по интерфейсу Usb-Hid",
              description:
                  "При обнаружении аудиоплафтормы, программа будет автоматический подключаться по интерфейсу Usb-Hid",
              state: data.autoConUsbHid,
              onTap: () {
                if (ref.read(connectionState.select((value) => value.autoConnectionHid))) {
                  data.copyWith(autoConUsbHid: false);
                  AndroidService.exchange(
                      "$methodSetSettings ${ServiceSettings.connectionUsbHid.index} ${WorkState.disable.index}");
                  ref.read(connectionState.notifier).copyWith(autoConnectionHid: false);
                } else {
                  data.copyWith(autoConUsbHid: true);
                  AndroidService.exchange(
                      "$methodSetSettings ${ServiceSettings.connectionUsbHid.index} ${WorkState.enable.index}");
                  ref.read(connectionState.notifier).copyWith(autoConnectionHid: true);
                }
              },
            ),
            const Divider(),
            SettingsChecked(
              parrametr: "Авто-подключение по интерфейсу Usb-Cdc",
              description:
                  "При обнаружении аудиоплафтормы, программа будет автоматический подключаться по интерфейсу Usb-Cdc",
              state: data.autoConUsbCdc,
              onTap: () {
                if (ref.read(connectionState.select((value) => value.autoConnectionCdc))) {
                  data.copyWith(autoConUsbCdc: false);
                  AndroidService.exchange(
                      "$methodSetSettings ${ServiceSettings.connectionUsbCdc.index} ${WorkState.disable.index}");
                  ref.read(connectionState.notifier).copyWith(autoConnectionCdc: false);
                } else {
                  data.copyWith(autoConUsbCdc: true);
                  AndroidService.exchange(
                      "$methodSetSettings ${ServiceSettings.connectionUsbCdc.index} ${WorkState.enable.index}");
                  ref.read(connectionState.notifier).copyWith(autoConnectionCdc: true);
                }
              },
            ),
            const Divider(),
            SettingsChecked(
              parrametr: "Авто-подключение по интерфейсу Bluetooth SPP",
              description:
                  "При обнаружении аудиоплафтормы, программа будет автоматический подключаться по интерфейсу Bluetooth SPP",
              state: data.autoConBluetooth,
              onTap: () {
                if (ref.read(connectionState.select((value) => value.autoConnectionBluetooth))) {
                  data.copyWith(autoConBluetooth: false);
                  AndroidService.exchange(
                      "$methodSetSettings ${ServiceSettings.connectionBluetoothSpp.index} ${WorkState.disable.index}");
                  ref.read(connectionState.notifier).copyWith(autoConnectionBluetooth: false);
                } else {
                  data.copyWith(autoConBluetooth: true);
                  AndroidService.exchange(
                      "$methodSetSettings ${ServiceSettings.connectionBluetoothSpp.index} ${WorkState.enable.index}");
                  ref.read(connectionState.notifier).copyWith(autoConnectionBluetooth: true);
                }
              },
            ),
            const Divider(),
            SettingsChecked(
              parrametr: "Включить Head-Up Display",
              description:
                  "Включить отображение HUD информации. Т.е. при изменении уровня громкости, источников звука и .т.п. будет всплывать диалог с различной информацией поверх всех приложений. Внимание для работы этой функции необходимо разрешение Поверх других приложений.",
              state: data.hud,
              onTap: () {
                if (ref.read(androidServiceSettings.select((value) => value.hud))) {
                  AndroidService.exchange("$methodSetSettings ${ServiceSettings.hud.index} ${WorkState.disable.index}");
                  ref.read(androidServiceSettings.notifier).copyWith(hud: false);
                } else {
                  AndroidService.exchange("$methodSetSettings ${ServiceSettings.hud.index} ${WorkState.enable.index}");
                  ref.read(androidServiceSettings.notifier).copyWith(hud: true);
                }
              },
            ),
            const Divider(),
            SettingsChecked(
              parrametr: "Состояние сервиса в шторке уведомлений",
              description:
                  "Сервис Dixom работает в фоновом режиме для обслуживания подключения к аудиоплатформе и обмена данными. Не отключайте уведомление — иначе связь будет не стабильной. ВНИМАНИЕ!!! Для работы Функция требует разрешение 'Уведомления'.",
              state: data.notification,
              onTap: () {
                if (ref.read(androidServiceSettings.select((value) => value.notification))) {
                  AndroidService.exchange(
                      "$methodSetSettings ${ServiceSettings.notification.index} ${WorkState.disable.index}");
                  ref.read(androidServiceSettings.notifier).copyWith(notification: false);
                } else {
                  data.copyWith(notification: true);
                  AndroidService.exchange(
                      "$methodSetSettings ${ServiceSettings.notification.index} ${WorkState.enable.index}");
                  ref.read(androidServiceSettings.notifier).copyWith(notification: true);
                }
              },
            ),
            const SizedBox(height: 26),
          ],
        );
      },
      error: (error, stackTrace) {
        return Text(error.toString());
      },
      loading: () {
        return const CircularProgressIndicator();
      },
    );
  }
}

class GetAndroidPermissions extends StatefulWidget {
  const GetAndroidPermissions({
    super.key,
  });

  @override
  State<GetAndroidPermissions> createState() => _GetAndroidPermissionsState();
}

class _GetAndroidPermissionsState extends State<GetAndroidPermissions> {
  @override
  Widget build(BuildContext context) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        const Text(
          "Разрешения системы Android",
          style: TextStyle(fontSize: 20, color: Colors.white70),
        ),
        const Divider(),
        const GettingPermision(
          description:
              "Разрешите отображать уведомления поверх всех приложений.\n 1) Нажмите Разрешить\n 2) Найдите в списке приложений приложение Dixom \n 3) Включите переклчатель Показывать поверх других приложений \n 4) Вернитесь в приложение",
          parrametr: "Поверх дргуих приложений",
          descriptionOk: "Показывать уведомления поверх всех приложений",
          state: true,
          permission: Permission.systemAlertWindow,
        ),
        const SizedBox(height: 10),
        ButtoDescription(
          description:
              "Нажмите на кнопку проверить, если ваше устрйоство поддерживает вывод всплывающих сообщений то будет отображено, привет я HUD дисплей. Если не поддерживает то прост ничего не произойдёт.",
          parrametr: "Проверка Head-Up Display",
          onTap: () {
            sendStringToAndroid(methodHudTestMsg, param: "Привет, я HUD дисплей");
          },
          btnCaption: 'Проверить',
        ),
        const Divider(),
        const GettingPermision(
          description: "Разрешите определения местоположения для возможности сканирования BLE",
          parrametr: "Определение мостоположения",
          descriptionOk: "Разрешено определения местоположения для подключения по BLE",
          state: true,
          permission: Permission.location,
        ),
        const Divider(),
        const GettingPermision(
          description:
              "Необходимо для работы приложения в фоновом режиме и отображения состояния подключения к аудиоплатформе.",
          parrametr: "Уведомления",
          descriptionOk:
              "Работа приложения в фоновом режиме разрешена. Состояние подключения отображается в строке уведомлений. Не рекомендуется блокировать уведомление: при его блокировке возможно нестабильное функционирование подключения к платформе.",
          state: true,
          permission: Permission.notification,
        ),
        const Divider(),
        const GettingPermision(
          description: "Разрешите сканировать Bluetooth устройтсва по близости, технология BLE",
          parrametr: "Скарирование Bluetooth LE",
          descriptionOk: "Разрешено сканировать Bluetooth устройтсва по близости",
          state: true,
          permission: Permission.bluetoothScan,
        ),
        const Divider(),
        const GettingPermision(
          description: "Разрешите подключаться к Bluetooth устрйоствам",
          parrametr: "Подключение к bluetooth устройтсва",
          descriptionOk: "Разрешено подключаться к Bluetooth устрйоствам",
          state: true,
          permission: Permission.bluetoothConnect,
        ),
        const Divider(),
        const GettingPermision(
          description:
              "Для полноценной работы приложения необходимо разрешение для чтения и записи в память вашего устройства.",
          parrametr: "Доступ к памяти",
          descriptionOk: "Приложению Dixom разрешено, читать и писть в память вашего устройства.",
          state: true,
          permission: Permission.manageExternalStorage,
        ),
        const Divider(),
        ButtoDescription(
          description:
              "Если не получается включить необходимые разрешения через приложение, включите их в ручную, также рекомендуется убрать ограничения с контроля активности.",
          parrametr: "Настройки приложения Dixom",
          onTap: () {
            openAppSettings();
          },
          btnCaption: 'Открыть',
        ),
        const SizedBox(height: 26),
      ],
    );
  }
}
