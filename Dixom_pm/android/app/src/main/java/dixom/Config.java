package dixom;

import static dixom.Const.DEF_BLUETOOTH_DEVICE;

import android.content.Context;
import android.content.SharedPreferences;
import android.util.Base64;

/**
 * Config — централизованный статический класс конфигурации сервиса Dixom.
 *
 * <p>
 * Класс отвечает за:
 * <ul>
 *     <li>хранение пользовательских и системных настроек сервиса</li>
 *     <li>инициализацию настроек при запуске</li>
 *     <li>проверку версии структуры настроек</li>
 *     <li>сброс к значениям по умолчанию при несовместимости</li>
 *     <li>хранение имени Bluetooth-устройства</li>
 * </ul>
 * </p>
 *
 * <p>
 * Все настройки сервиса хранятся в {@link SharedPreferences}
 * в виде массива байтов {@link #serviceSettings},
 * закодированного в Base64.
 * Индекс каждого параметра соответствует {@link SettingKey#ordinal()}.
 * </p>
 *
 * <p>
 * Класс является полностью статическим и используется
 * всеми сервисными компонентами (соединения, уведомления, HUD, Flutter bridge).
 * </p>
 */
public class Config {

    /**
     * Ключи настроек сервиса.
     *
     * <p>
     * Порядок элементов критичен:
     * {@link #ordinal()} используется как индекс
     * в массиве {@link #serviceSettings}.
     * </p>
     */
    public enum SettingKey {

        /** Версия структуры настроек */
        SETTINGS_VERSION,

        /** Разрешение подключения по USB HID */
        CONNECTION_USB_HID,

        /** Разрешение подключения по USB CDC */
        CONNECTION_USB_CDC,

        /** Разрешение подключения по Bluetooth SPP */
        CONNECTION_BLUETOOTH_SPP,

        /** Разрешение отображения HUD-оверлея */
        HUD_ENABLED,

        /** Разрешение foreground-уведомлений */
        NOTIFICATIONS_ENABLED,

        /** Используемая Bluetooth-технология */
        BT_TECHNOLOGY
    }

    /* =========================================================
     * Константы SharedPreferences
     * ========================================================= */

    /** Имя SharedPreferences для настроек сервиса */
    public static final String PREF_NAME_SERVICE_SETTINGS = "dixomServiceSettings";

    /** Имя SharedPreferences для Bluetooth-устройства */
    public static final String PREF_NAME_BLUETOOTH_NAME = "dixomBluetoothName";

    /** Текущая версия структуры настроек */
    private static final int SETTINGS_VERSION = 4;

    /* =========================================================
     * Хранилище настроек
     * ========================================================= */

    /**
     * Массив байтов с настройками сервиса.
     *
     * <p>
     * Каждый элемент соответствует {@link SettingKey#ordinal()}.
     * Значения хранятся как byte (0–255).
     * </p>
     */
    public static byte[] serviceSettings = new byte[10];

    /** SharedPreferences для настроек сервиса */
    private static SharedPreferences PrefServiceSettings;


    /* =========================================================
     * Bluetooth device name
     * ========================================================= */

    /** Текущее имя Bluetooth-устройства */
    private static String currentBluetoothDeviceName;

    /** SharedPreferences для имени Bluetooth-устройства */
    private static SharedPreferences PrefBluetoothName;

    /**
     * Сохраняет имя Bluetooth-устройства.
     *
     * @param param имя устройства
     */
    public static void setBluetoothName(String param) {
        currentBluetoothDeviceName = param;
        SharedPreferences.Editor editor = PrefBluetoothName.edit();
        editor.putString(PREF_NAME_BLUETOOTH_NAME, param);
        editor.apply();
    }

    /**
     * Возвращает текущее имя Bluetooth-устройства.
     *
     * @return имя устройства
     */
    public static String GetBluetoothName() {
        return currentBluetoothDeviceName;
    }

    /* =========================================================
     * Инициализация
     * ========================================================= */

    /**
     * Инициализирует настройки сервиса.
     *
     * <p>
     * Выполняет:
     * <ul>
     *     <li>чтение настроек из SharedPreferences</li>
     *     <li>декодирование Base64</li>
     *     <li>проверку версии структуры</li>
     *     <li>сброс к значениям по умолчанию при ошибках</li>
     *     <li>загрузку имени Bluetooth-устройства</li>
     * </ul>
     * </p>
     *
     * @param context контекст приложения или сервиса
     */
    public static void initializeServiceSettings(Context context) {

        /* ---------- Service settings ---------- */

        PrefServiceSettings =
                context.getSharedPreferences(
                        PREF_NAME_SERVICE_SETTINGS,
                        Context.MODE_PRIVATE
                );

        String encodedSettings =
                PrefServiceSettings.getString(
                        PREF_NAME_SERVICE_SETTINGS,
                        null
                );

        boolean resetToDefaults = false;

        if (encodedSettings == null) {
            resetToDefaults = true;
        } else {
            try {
                serviceSettings =
                        Base64.decode(encodedSettings, Base64.DEFAULT);

                if (serviceSettings.length == 0 ||
                        serviceSettings[SettingKey.SETTINGS_VERSION.ordinal()]
                                != SETTINGS_VERSION) {
                    resetToDefaults = true;
                }

            } catch (IllegalArgumentException e) {
                resetToDefaults = true;
            }
        }

        if (resetToDefaults) {
            setDefaultServiceSettings();
        }

        /* ---------- Bluetooth name ---------- */

        PrefBluetoothName =
                context.getSharedPreferences(
                        PREF_NAME_BLUETOOTH_NAME,
                        Context.MODE_PRIVATE
                );

        currentBluetoothDeviceName =
                PrefBluetoothName.getString(
                        PREF_NAME_BLUETOOTH_NAME,
                        DEF_BLUETOOTH_DEVICE
                );

        if (DEF_BLUETOOTH_DEVICE.equals(currentBluetoothDeviceName)) {
            setBluetoothName(DEF_BLUETOOTH_DEVICE);
        }
    }

    /* =========================================================
     * Работа с настройками
     * ========================================================= */

    /**
     * Устанавливает значение настройки сервиса.
     *
     * <p>
     * Значение сохраняется в массив {@link #serviceSettings}
     * и сразу записывается в SharedPreferences.
     * </p>
     *
     * @param key   ключ настройки
     * @param value значение (0–255)
     */
    public static void setServiceSetting(SettingKey key, int value) {

        if (serviceSettings == null || PrefServiceSettings == null) {
            return;
        }

        if (value < 0 || value > 255) {
            return;
        }

        int index = key.ordinal();

        if (index >= serviceSettings.length) {
            return;
        }

        serviceSettings[index] = (byte) value;

        String encoded =
                Base64.encodeToString(serviceSettings, Base64.DEFAULT);

        PrefServiceSettings.edit()
                .putString(PREF_NAME_SERVICE_SETTINGS, encoded)
                .apply();
    }

    /**
     * Сбрасывает настройки сервиса к значениям по умолчанию
     * и сохраняет их в SharedPreferences.
     */
    public static void setDefaultServiceSettings() {

        serviceSettings[SettingKey.SETTINGS_VERSION.ordinal()] =
                (byte) SETTINGS_VERSION;

        serviceSettings[SettingKey.CONNECTION_USB_HID.ordinal()] =
                (byte) Const.WorkState.ENABLE.ordinal();

        serviceSettings[SettingKey.CONNECTION_USB_CDC.ordinal()] =
                (byte) Const.WorkState.ENABLE.ordinal();

        serviceSettings[SettingKey.CONNECTION_BLUETOOTH_SPP.ordinal()] =
                (byte) Const.WorkState.DISABLE.ordinal();

        serviceSettings[SettingKey.HUD_ENABLED.ordinal()] =
                (byte) Const.WorkState.ENABLE.ordinal();

        serviceSettings[SettingKey.NOTIFICATIONS_ENABLED.ordinal()] =
                (byte) Const.WorkState.ENABLE.ordinal();

        serviceSettings[SettingKey.BT_TECHNOLOGY.ordinal()] =
                (byte) Const.BtTechnology.SPP.ordinal();

        String prefParamServiceSettings =
                Base64.encodeToString(serviceSettings, Base64.DEFAULT);

        PrefServiceSettings.edit()
                .putString(PREF_NAME_SERVICE_SETTINGS, prefParamServiceSettings)
                .apply();
    }
}
