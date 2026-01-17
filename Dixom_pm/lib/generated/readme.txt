Официальное описание здесь
https://pub.dev/packages/easy_localization

Перегенерация кода для добавления слов в языковые пакеты
flutter pub run easy_localization:generate -S "assets/translations" 

Генерация ключей для доступа к словам
flutter pub run easy_localization:generate -f keys -o locale_keys.g.dart -S "assets/translations" 

Выставление языка: После выбора языка, не нужно сохранять. Библиотека делает всё автоматический.
Английский
context.setLocale(Locale('en'));
Русский
context.setLocale(Locale('ru'));


cd C:\Users\alekk\AppData\Local\Android\Sdk\platform-tools
adb connect 192.168.0.5:45554

 

