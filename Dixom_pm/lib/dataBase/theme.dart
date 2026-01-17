import 'package:dixom/dataBase/app_constants.dart';
import 'package:flutter/material.dart';

const largeTeztSize = 26.0;
const mediumTextSize = 20;
const bodyTextSize = 16;

Color kPrimaryColor = const Color(0xFF28292E);
Color kSecondaryColor = const Color(0xFFEF9A9A);
Color kBackgroundColor = const Color(0xFF28292E);
Color kAccentColor = Colors.red;

ButtonStyle segmentedTheme() {
  return ButtonStyle(
    backgroundColor: WidgetStateProperty.resolveWith<Color?>(
      (Set<WidgetState> states) {
        if (states.contains(WidgetState.pressed)) {
          return Colors.white24; // Цвет при нажатии
        } else if (states.contains(WidgetState.disabled)) {
          return AppColor.background; // Цвет при нажатии
        } else if (states.contains(WidgetState.selected)) {
          return Colors.white; // Цвет при нажатии
        } else if (states.contains(WidgetState.hovered)) {
          return Colors.white30; // Цвет при нажатии
        } else {
          return AppColor.background; // Цвет по умолчанию
        }
      },
    ),
    shape: WidgetStateProperty.all<RoundedRectangleBorder>(
      RoundedRectangleBorder(
        borderRadius: BorderRadius.circular(8.0),
      ),
    ),
  );
}

ButtonStyle buttonTheme() {
  return ButtonStyle(
    backgroundColor: WidgetStateProperty.resolveWith<Color?>(
      (Set<WidgetState> states) {
        if (states.contains(WidgetState.pressed)) {
          return Colors.white70; // Цвет при нажатии
        } else if (states.contains(WidgetState.disabled)) {
          return Colors.white30; // Цвет при отключении
        } else if (states.contains(WidgetState.selected)) {
          return Colors.white; // Цвет при выборе
        } else if (states.contains(WidgetState.hovered)) {
          return Colors.white; // Цвет при наведении
        } else {
          return Colors.white; // Цвет по умолчанию
        }
      },
    ),
    foregroundColor: WidgetStateProperty.resolveWith<Color?>(
      (Set<WidgetState> states) {
        if (states.contains(WidgetState.pressed)) {
          return Colors.black; // Цвет при нажатии
        } else if (states.contains(WidgetState.disabled)) {
          return AppColor.background; // Цвет при отключении
        } else if (states.contains(WidgetState.selected)) {
          return Colors.white; // Цвет при выборе
        } else {
          return Colors.black; // Цвет по умолчанию
        }
      },
    ),
    shape: WidgetStateProperty.all<RoundedRectangleBorder>(
      RoundedRectangleBorder(
        borderRadius: BorderRadius.circular(6.0),
      ),
    ),
  );
}

ThemeData darkTheme() => ThemeData(
      brightness: Brightness.dark,
      primaryColor: kPrimaryColor,
      textTheme: const TextTheme(),
      iconTheme: const IconThemeData(color: Color.fromARGB(255, 94, 91, 90), size: 25.0),
      elevatedButtonTheme: ElevatedButtonThemeData(style: buttonTheme()),
      segmentedButtonTheme: SegmentedButtonThemeData(style: segmentedTheme()),
      bottomNavigationBarTheme: const BottomNavigationBarThemeData(
          showUnselectedLabels: true, unselectedItemColor: Color.fromARGB(255, 110, 110, 110)),
      switchTheme: SwitchThemeData(
        thumbColor: WidgetStateProperty.resolveWith<Color>(
          (Set<WidgetState> states) {
            if (states.contains(WidgetState.selected)) {
              return Colors.black12; // Цвет при включенном состоянии
            }
            return Colors.grey; // Цвет при отключенном состоянии
          },
        ),
        trackColor: WidgetStateProperty.resolveWith<Color>(
          (Set<WidgetState> states) {
            if (states.contains(WidgetState.selected)) {
              return Colors.white; // Цвет дорожки при включенном состоянии
            }
            return AppColor.background; // Цвет дорожки при отключенном состоянии
          },
        ),
      ),
    );

ThemeData lightTheme() => ThemeData(
      brightness: Brightness.light,
      primaryColor: kPrimaryColor,
      textTheme: const TextTheme(),
      iconTheme: const IconThemeData(color: Colors.red, size: 25.0),
    );
