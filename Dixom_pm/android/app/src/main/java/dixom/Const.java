package dixom;

/**
 * Const — центральный класс констант проекта Dixom.
 *
 * <p>
 * Содержит:
 * <ul>
 *     <li>строковые константы для Broadcast Intent и Flutter MethodChannel</li>
 *     <li>идентификаторы USB-устройств</li>
 *     <li>настройки foreground-уведомлений</li>
 *     <li>типы сообщений протокола обмена</li>
 *     <li>enum-состояния приложения, соединений и интерфейсов</li>
 * </ul>
 * </p>
 *
 * <p>
 * Класс является {@code final} и не предназначен для наследования.
 * Все элементы — {@code static}, используются во всём сервисе,
 * включая Android Service, ConnectionManager и Flutter bridge.
 * </p>
 */
public final class Const {

    /* =========================================================
     * Broadcast actions / Intent actions
     * ========================================================= */

    /** Разрешение на доступ к USB HID */
    public static final String ACTION_USB_PERMISSION =
            "com.google.android.HID.action.USB_PERMISSION";

    /** Получена команда (строка) */
    public static final String ACTION_COMMAND_RECEIVED =
            "ru.dixom.service.command_received";

    /** Получена команда (байты) */
    public static final String ACTION_COMMAND_RECEIVED_BYTES =
            "ru.dixom.service.command_received_bytes";

    /** Ключ extra-параметра в broadcast */
    public static final String BROADCAST_EXTRA_KEY = "key";

    /** Данные команды в broadcast */
    public static final String BROADCAST_EXTRA_DATA = "data";

    /** Запуск Flutter Engine */
    public static final String ACTION_START_FLUTTER_ENGINE =
            "ru.dixom.service.flutter_engine_start";

    /** Отправка строковой команды в сервис */
    public static final String ACTION_COMMAND_SEND =
            "ru.dixom.service.command_send";

    /** Отправка байтовой команды в сервис */
    public static final String ACTION_COMMAND_SEND_BYTES =
            "ru.dixom.service.command_send_bytes";

    /** Проверка доступности STM */
    public static final String ACTION_COMMAND_STM_CHECK =
            "ru.dixom.service.stm_check";

    /** Состояние Flutter-приложения */
    public static final String ACTION_FLUTTER_APP_STATE =
            "ru.dixom.service.flutter_app_state";

    /** Получить тип интерфейса */
    public static final String ACTION_COMMAND_GET_TYPE =
            "ru.dixom.service.command_type_get";

    /** Установить тип интерфейса */
    public static final String ACTION_COMMAND_SET_TYPE =
            "ru.dixom.service.command_type_set";

    /** Команда из Flutter → Service */
    public static final String ACTION_COMMAND_SEND_TO_SERVICE =
            "ru.dixom.service.command_send_to_service";

    /** Команда из Service → Interface */
    public static final String ACTION_COMMAND_SEND_TO_INTERFACE =
            "ru.dixom.service.command_send_to_interface";

    /* =========================================================
     * Flutter MethodChannel
     * ========================================================= */

    /** Канал Android → Dart */
    public static final String EXCHANGE_CHANNEL_FROM_ANDROID_TO_DART =
            "com.dixom.exchange.android.dart";

    /** Канал Dart → Android */
    public static final String EXCHANGE_CHANNEL_FROM_DART_TO_ANDROID =
            "com.dixom.exchange.dart.android";

    /** ID Flutter Engine */
    public static final String FLUTTER_ENGINE_ID =
            "dixom_engine_id";

    /** Ключ состояния приложения */
    public static final String NAME_APP_STATE = "state";

    /* =========================================================
     * Bluetooth
     * ========================================================= */

    /** Основное имя Bluetooth-устройства */
    public static final String DEF_BLUETOOTH_DEVICE =
            "Dixom HiRes Audio";

    /** Альтернативное имя Bluetooth-устройства */
    public static final String DEF_BLUETOOTH_DEVICE_2 =
            "Dixom Hi-Res Audio";

    /* =========================================================
     * Flutter methods / arguments
     * ========================================================= */

    public static final String MAIN_METHOD_SEND_STRING = "str";
    public static final String MAIN_METHOD_SEND_BYTE = "byte";
    public static final String MAIN_ARGUMENT = "data";
    public static final String MAIN_METHOD_SERVICE = "service";
    public static final String MAIN_METHOD_CHECK = "check";

    public static final String METHOD_APP_STATE = "app_state";
    public static final String METHOD_GET_SETTINGS = "get_settings";
    public static final String METHOD_GET_CONNECTION = "get_connection";
    public static final String METHOD_SET_CONNECTION = "set_connection";
    public static final String METHOD_GET_CONNECTION_STATUS = "get_conn_status";
    public static final String METHOD_SET_SETTINGS = "set_settings";
    public static final String METHOD_SET_INTERFACE = "set_interface";
    public static final String METHOD_GET_BLUETOOTH_DEVICE_LIST = "get_bt_device_list";
    public static final String METHOD_GET_BLUETOOTH_DEVICE = "get_bt_device";
    public static final String METHOD_SET_BLUETOOTH_DEVICE = "set_bt_device";
    public static final String METHOD_INTERFACE_INFO = "interface_info";
    public static final String METHOD_HUD_TEST = "hud_test_msg";

    /* =========================================================
     * Foreground notification
     * ========================================================= */

    /** Название канала foreground-уведомлений */
    public static final String FOREGROUND_CHANNEL =
            "Статус подключения платформы";

    /** ID notification channel */
    public static final String NOTIFICATION_CHANNEL_ID =
            "Dixom notification";

    /** ID foreground-уведомления */
    public static final int NOTIFICATION_ID = 225732;

    /* =========================================================
     * USB parameters
     * ========================================================= */

    /** USB Vendor ID устройства */
    public static final int VENDOR_ID = 61525;

    /** Основной Product ID */
    public static final int PRODUCT_ID = 6;

    /** Product ID bootloader (вариант m) */
    public static final int PRODUCT_ID_BOOT_m = 7;

    /** Product ID bootloader (вариант c12) */
    public static final int PRODUCT_ID_BOOT_c12 = 8;

    /** Максимальная длина сообщения */
    public static final int MAX_MESSAGE_LENGTH = 255;

    /* =========================================================
     * Message protocol types
     * ========================================================= */

    /**
     * Типы сообщений протокола обмена.
     *
     * <p>
     * Значения соответствуют ASCII-символам
     * и кодируются в управляющем байте frameControl.
     * </p>
     */
    public static class MsgType {

        /** UTF-8 строка, цельное сообщение */
        public static final char UTF8_WHOLE = 33;          // !

        /** UTF-8 строка, начало фрагмента */
        public static final char UTF8_PART_START = 34;    // "

        /** UTF-8 строка, промежуточный фрагмент */
        public static final char UTF8_PART = 35;          // #

        /** UTF-8 строка, конец фрагмента */
        public static final char UTF8_PART_END = 36;      // $

        /** Base64 данные, цельное сообщение */
        public static final char BASE64_WHOLE = 37;       // %

        /** Base64 данные, начало фрагмента */
        public static final char BASE64_PART_START = 38;  // &

        /** Base64 данные, промежуточный фрагмент */
        public static final char BASE64_PART = 39;        // '

        /** Base64 данные, конец фрагмента */
        public static final char BASE64_PART_END = 40;    // (

        /** Сырые байты, цельное сообщение */
        public static final char BYTE_WHOLE = 41;         // )

        /** Сырые байты, начало фрагмента */
        public static final char BYTE_PART_START = 42;    // *

        /** Сырые байты, промежуточный фрагмент */
        public static final char BYTE_PART = 43;          // +

        /** Сырые байты, конец фрагмента */
        public static final char BYTE_PART_END = 44;      // ,
    }

    /* =========================================================
     * Transport interface types
     * ========================================================= */

    /**
     * Типы интерфейсов обмена данными.
     */
    public enum Transprort {
        USB_HID,        // USB Custom HID
        USB_CDC,        // USB CDC
        BLUETOOTH_SPP,  // Bluetooth SPP
        RESERVED1,
        RESERVED2,
        RESERVED3,
        NONE            // Нет активного интерфейса
    }

    /* =========================================================
     * Application state
     * ========================================================= */

    /**
     * Состояние Flutter-приложения.
     */
    public enum AppState {
        RESUME,
        INACTIVE,
        PAUSED,
        DETACHED,
        NOT_ALLOWED
    }

    /* =========================================================
     * Work / connection state
     * ========================================================= */

    /**
     * Универсальные состояния работы, подключения и операций.
     */
    public enum WorkState {
        NOT_INITIALIZED,
        CONNECTED,
        CONNECTING,
        DISCONNECTED,
        DISCONNECTING,
        NOT_FOUND,
        AVAILABLE,
        NOT_AVAILABLE,
        OK,
        ERROR,
        ENABLE,
        DISABLE,
        CHANGED,
        NO_PERMISSION,
        NOT_SUPPORTED,
        COMPLETE,
        NOT_COMPLETE,
        BLUETOOTH_OFF
    }

    /* =========================================================
     * Transfer statistics state
     * ========================================================= */

    /**
     * Типы статистики передачи данных.
     */
    public enum TransferStatus {
        STATUS,
        STATUS_IN,
        STATUS_OUT,
        SUCCESS_IN,
        SUCCESS_OUT,
        ERROR_IN,
        ERROR_OUT
    }

    /* =========================================================
     * Bluetooth technology
     * ========================================================= */

    /**
     * Поддерживаемые Bluetooth-технологии.
     */
    public enum BtTechnology {
        GATT,
        SPP
    }
}
