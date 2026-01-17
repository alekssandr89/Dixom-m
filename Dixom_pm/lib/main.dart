import 'dart:async';
import 'dart:io';
import 'dart:ui';
import 'package:dixom/pages/button/external_control.dart';
import 'package:dixom/pages/my_car/my_car.dart';
import 'package:dixom/pages/settings/base_settings.dart';
import 'package:easy_localization/easy_localization.dart';
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:shared_preferences/shared_preferences.dart';
import 'package:window_manager/window_manager.dart';
import 'package:dixom/driver/android/android_service_model.dart';
import 'package:dixom/pages/top_bar/connection/backend/device_connection.dart';
import 'package:dixom/dataBase/device_constant.dart';
import 'package:dixom/driver/windows/usb_hid.dart';
import 'package:dixom/pages/top_bar/lower_menu.dart';
import 'package:dixom/pages/home/home.dart';
import 'package:dixom/pages/audio/filter.dart';
import 'package:dixom/pages/audio/view/output/audio_out.dart';
import 'package:dixom/components/utilities/app_life.dart';
import 'package:dixom/dataBase/theme.dart';
import 'dataBase/app_constants.dart';
import 'driver/android/service_channel.dart';
import 'pages/audio/loudness.dart';
import 'pages/can_bus/can_page.dart';
import 'generated/codegen_loader.g.dart';
import 'dataBase/pages.dart';
import 'pages/fm_radio/view/fm_page.dart';
import 'pages/test/test_page.dart';

 

class MyHomePage extends StatelessWidget {
  const MyHomePage({super.key});
  @override
  Widget build(BuildContext context) {
    const String beginText = "Работа демонстрационного листинга на";
    return Scaffold(
      appBar: AppBar(title: const Text('Futter Demo ВКР 2025')),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            if (Platform.isAndroid)
              const Text(textAlign: TextAlign.center, style: TextStyle(fontSize: 25), '$beginText Android'),
            if (Platform.isWindows)
              const Text(textAlign: TextAlign.center, style: TextStyle(fontSize: 25), '$beginText Windows'),
          ],
        ),
      ),
    );
  }
}

class MyCustomScrollBehavior extends MaterialScrollBehavior {
  // Переопределить методы поведения, такие как buildOverscrollIndicator и buildScrollbar
  // Для работы горизонтальной прокрутки мышкой
  @override
  Set<PointerDeviceKind> get dragDevices => {
        PointerDeviceKind.touch,
        PointerDeviceKind.mouse,
        // etc.
      };
}

bool initializationOnFirstRun = false;

void appInitialization(WidgetRef ref, BuildContext context) async {
  if (initializationOnFirstRun == false) {
    initializationOnFirstRun = true;
    appRef = ref;

    if (Platform.isAndroid) {
      //Запуск слушателя сообщений из сервиса андройд
      AndroidService.start();

      ///Запускаем таймер и проеряем достпность таймера каждые 50мс
      Timer.periodic(const Duration(milliseconds: 50), (timer) async {
        //Если сервис android отвечает, значит получаем инфомацию о подключении
        if (WorkState.available == await seviceAvilableCheck()) {
          //Говорим сервису что приложение запущено
          await AndroidService.exchange("app_state resumed");
          //Получаем информацию о подключениях
          await AndroidServiceConnectionRepository().getSettingsData(ExchangeInterface.usbHid);
          await AndroidServiceConnectionRepository().getSettingsData(ExchangeInterface.usbCdc);
          await AndroidServiceConnectionRepository().getSettingsData(ExchangeInterface.bluetoothSpp);
          timer.cancel();
        }
      });
    }
    if (Platform.isWindows) {
      // Obtain shared preferences.
      final SharedPreferences prefs = await SharedPreferences.getInstance();

      final bool? repeat = prefs.getBool('windowsAutoConnectUsbHid');

      if (repeat == null) {
        await prefs.setBool('windowsAutoConnectUsbHid', true);
      }

      final bool? repeat2 = prefs.getBool('windowsAutoConnectUsbHid');

      if (repeat2 != null) {
        if (repeat2 == true) {
          usbHid.startAutoConnect();
          appRef.read(connectionState.notifier).copyWith(autoConnectionHid: true);
        } else {
          appRef.read(connectionState.notifier).copyWith(autoConnectionHid: false);
        }
      }
    }
  }
}

late final WidgetRef appRef;

final platformNotification = StateNotifierProvider.autoDispose<NotificationMessage, String>((ref) {
  return NotificationMessage();
});

class NotificationMessage extends StateNotifier<String> {
  NotificationMessage() : super("");

  void showNotification(String message) {
    state = message;
  }
}

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  WidgetsBinding.instance.addObserver(AppLifecycleObserver());
  await EasyLocalization.ensureInitialized();

  if (Platform.isWindows) {
    await windowManager.ensureInitialized();
    WindowManager.instance.setSize(const Size(1300, 800));
    WindowManager.instance.setMinimumSize(const Size(400, 600));
    WindowManager.instance.setMaximumSize(const Size(1300, 1000));
  }

  runApp(
    ProviderScope(
      child: EasyLocalization(
        supportedLocales: const [Locale('en'), Locale('ru')],
        path: 'assets/translations',
        saveLocale: true,
        fallbackLocale: const Locale('ru'),
        assetLoader: const CodegenLoader(),
        child: const DixomApp(),
      ),
    ),
  );
}

class DixomApp extends ConsumerWidget {
  const DixomApp({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    appInitialization(ref, context);

    return MaterialApp(
      scrollBehavior: MyCustomScrollBehavior(),
      debugShowCheckedModeBanner: false,
      localizationsDelegates: context.localizationDelegates,
      supportedLocales: context.supportedLocales,
      locale: context.locale,
      theme: darkTheme().copyWith(
        scaffoldBackgroundColor: AppColor.background,
        canvasColor: AppColor.secondary,
      ),
      initialRoute: AppPage.home,
      routes: {
        AppPage.home: (context) => const Home(),
        AppPage.connection: (context) => const MainConnectionPage(),
        AppPage.channelFilter: (context) => const ChannelFilter(),
        AppPage.volumeAndDelay: (context) => const VolumeAndDelay(),
        AppPage.externalControl: (context) => const ExternalControlPage(),
        AppPage.canBus: (context) => const CanBusPage(),
        AppPage.baseSettings: (context) => const GeneralSettings(),
        AppPage.fmRadio: (context) => const FmRadioPage(),
        AppPage.test: (context) => const TestPage(),
        AppPage.loudness: (context) => const Loudness(),
        AppPage.myCar: (context) => const MyCarPage(),
      },
    );
  }
}

// TODO  список задач

// FIXME исправь меня




