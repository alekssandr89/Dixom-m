package dixom.Service;

import android.content.Context;

import androidx.annotation.NonNull;

import java.util.Objects;

import dixom.Config;
import dixom.Const;
import dixom.DebugHelper;
import dixom.Service.Connection.ConnectionManager;
import dixom.Service.Connection.DataFrame;
import dixom.Service.Connection.TransportEventListener;
import dixom.Service.Connection.TransportStatus;
import io.flutter.embedding.engine.FlutterEngine;
import io.flutter.embedding.engine.FlutterEngineCache;
import io.flutter.plugin.common.MethodCall;
import io.flutter.plugin.common.MethodChannel;

/**
 * Класс {@code FlutterServiceBridge} реализует мост для взаимодействия между
 * Android-сервисом и Flutter-приложением.
 *
 * <p>Обеспечивает двунаправленную коммуникацию через {@link MethodChannel}, обрабатывая
 * вызовы методов из Flutter и отправляя события обратно в Flutter.
 * Также реализует интерфейс {@link TransportEventListener} для получения обновлений
 * о состоянии транспортных соединений и получаемых данных.</p>
 *
 * <p>Основные функции класса:
 * <ul>
 *   <li>Инициализация и управление {@link FlutterEngine} и каналами обмена сообщениями;</li>
 *   <li>Обработка входящих вызовов из Flutter и выполнение соответствующих команд сервиса;</li>
 *   <li>Отправка уведомлений и данных из Android-сервиса во Flutter;</li>
 *   <li>Слежение за состоянием соединений через {@link TransportStatus} и обновление UI (HUD);</li>
 *   <li>Управление настройками и состояниями сервиса на основе команд Flutter;</li>
 * </ul>
 * </p>
 *
 * <p>Используется для интеграции нативного Android-сервиса с Flutter-приложением
 * в архитектуре, где сервис отвечает за работу с транспортными интерфейсами (USB, Bluetooth и др.).</p>
 *
 * @see io.flutter.embedding.engine.FlutterEngine
 * @see io.flutter.plugin.common.MethodChannel
 * @see dixom.Service.Connection.TransportEventListener
 * @see dixom.Service.Connection.ConnectionManager
 */
public class FlutterServiceBridge implements MethodChannel.MethodCallHandler, TransportEventListener {

    /**
     * Текущее состояние Flutter-приложения относительно сервиса (например, активное, приостановленное, отключенное)
     */
    public static Const.AppState StatusFlutterApp = Const.AppState.DETACHED;

    private FlutterEngine flutterEngine;
    private final Context context;
    private final ConnectionManager connectionManager;

    /**
     * Канал для получения вызовов из Flutter в Android
     */
    public MethodChannel ChannelDartToAndroid = null;

    /**
     * Канал для отправки сообщений из Android в Flutter
     */
    public MethodChannel ChannelAndroidToDart = null;

    private final HudDisplay hudDisplay;

    /**
     * Конструктор.
     * Устанавливает слушателей транспортных статусов для мониторинга состояния соединений.
     *
     * @param context контекст приложения
     * @param connectionManager менеджер транспортных соединений
     * @param hudDisplay отображение HUD (Head-Up Display)
     */
    public FlutterServiceBridge(Context context, ConnectionManager connectionManager, HudDisplay hudDisplay) {
        this.context = context;
        this.connectionManager = connectionManager;
        this.hudDisplay = hudDisplay;

        // Подписка на изменения статуса для разных транспортов
        this.connectionManager.UsbHid.transportStatus.setListener(this);
        this.connectionManager.UsbCdc.transportStatus.setListener(this);
        this.connectionManager.BluetoothSpp.transportStatus.setListener(this);
        this.connectionManager.Receiver.setListener(this);
    }

    /**
     * Инициализирует FlutterEngine и настраивает каналы связи.
     * Обеспечивает прием и передачу сообщений между Flutter и Android.
     */
    public void startFlutterEngine() {
        try {
            flutterEngine = new FlutterEngine(context);
            flutterEngine = FlutterEngineCache.getInstance().get(Const.FLUTTER_ENGINE_ID);
            assert flutterEngine != null;

            ChannelDartToAndroid = new MethodChannel(flutterEngine.getDartExecutor().getBinaryMessenger(),
                    Const.EXCHANGE_CHANNEL_FROM_DART_TO_ANDROID);
            ChannelDartToAndroid.setMethodCallHandler(this);

            ChannelAndroidToDart = new MethodChannel(flutterEngine.getDartExecutor().getBinaryMessenger(),
                    Const.EXCHANGE_CHANNEL_FROM_ANDROID_TO_DART);
        } catch (Exception e) {
            StatusFlutterApp = Const.AppState.DETACHED;
        }
    }

    /**
     * Закрывает и уничтожает FlutterEngine, освобождая ресурсы.
     */
    public void destroyFlutterEngine() {
        if (flutterEngine != null) {
            flutterEngine.destroy();
        }
    }

    /**
     * Обработка вызовов из Flutter через {@link MethodChannel}.
     * Реализует логику выполнения команд, включая отправку данных, управление соединениями и настройками.
     *
     * @param call объект с информацией о вызове метода из Flutter
     * @param result объект для отправки результата вызова обратно в Flutter
     */
    @Override
    public void onMethodCall(@NonNull MethodCall call, @NonNull MethodChannel.Result result) {
        switch (call.method) {
            case Const.MAIN_METHOD_SEND_STRING:
                String strData = call.argument(Const.MAIN_ARGUMENT);
                result.success(connectionManager.Sending_Data_To_Interface(strData).ordinal());
                break;

            case Const.MAIN_METHOD_SEND_BYTE:
                byte[] bytes = call.argument(Const.MAIN_ARGUMENT);
                result.success(connectionManager.Sending_Data_To_Interface(bytes).ordinal());
                break;

            case Const.MAIN_METHOD_SERVICE:
                String command = Objects.requireNonNull(call.argument(Const.MAIN_ARGUMENT));
                result.success(serviceStringControl(command).ordinal());
                break;

            case Const.MAIN_METHOD_CHECK:
                result.success(Const.WorkState.AVAILABLE.ordinal());
                break;

            case Const.METHOD_GET_SETTINGS:
                result.success(getServiceSettings());
                break;

            case Const.METHOD_GET_CONNECTION:
                String interfaceId = Objects.requireNonNull(call.argument(Const.MAIN_ARGUMENT));
                result.success(getConnectionInfo(interfaceId));
                break;

            case Const.METHOD_GET_CONNECTION_STATUS:
                result.success(String.valueOf(connectionManager.StatusConnection.ordinal()));
                break;

            case Const.METHOD_GET_BLUETOOTH_DEVICE_LIST:
                result.success(connectionManager.BluetoothSpp.getDeviceList());
                break;

            case Const.METHOD_SET_BLUETOOTH_DEVICE:
                Config.setBluetoothName(Objects.requireNonNull(call.argument(Const.MAIN_ARGUMENT)));
                result.success(Config.GetBluetoothName());
                break;

            case Const.METHOD_GET_BLUETOOTH_DEVICE:
                result.success(Config.GetBluetoothName());
                break;

            case Const.METHOD_HUD_TEST:
                String response = String.valueOf(hudDisplay.hudShowTestMessage(Objects.requireNonNull(call.argument(Const.MAIN_ARGUMENT))).ordinal());
                // TODO: Разобраться с повторной отправкой результата
                result.notImplemented();
                break;

            default:
                result.notImplemented();
                break;
        }
    }

    /**
     * Возвращает информацию о состоянии конкретного транспортного интерфейса.
     *
     * @param method строковый идентификатор метода Flutter
     * @return строка с текущим статусом и статистикой по передаче данных
     */
    String getConnectionInfo(String method) {
        Const.Transprort exchangeInterface = Const.Transprort.values()[Integer.parseInt(method)];
        TransportStatus interfaceInfo = null;

        if (exchangeInterface == Const.Transprort.USB_HID) {
            interfaceInfo = connectionManager.UsbHid.transportStatus;
        } else if (exchangeInterface == Const.Transprort.USB_CDC) {
            interfaceInfo = connectionManager.UsbCdc.transportStatus;
        } else if (exchangeInterface == Const.Transprort.BLUETOOTH_SPP) {
            interfaceInfo = connectionManager.BluetoothSpp.transportStatus;
        }

        assert interfaceInfo != null;

        return interfaceInfo.getState().ordinal() + " " +
                interfaceInfo.getState().ordinal() + " " +
                interfaceInfo.getState().ordinal() + " " +
                interfaceInfo.getExchangeInfo(Const.TransferStatus.SUCCESS_IN) + " " +
                interfaceInfo.getExchangeInfo(Const.TransferStatus.SUCCESS_OUT) + " " +
                interfaceInfo.getExchangeInfo(Const.TransferStatus.ERROR_IN) + " " +
                interfaceInfo.getExchangeInfo(Const.TransferStatus.ERROR_OUT);
    }

    /**
     * Получение текущих настроек сервиса в формате строки.
     *
     * @return строка с основными параметрами настроек
     */
    private String getServiceSettings() {
        return Config.serviceSettings[Config.SettingKey.CONNECTION_USB_HID.ordinal()] + " " +
                Config.serviceSettings[Config.SettingKey.CONNECTION_USB_CDC.ordinal()] + " " +
                Config.serviceSettings[Config.SettingKey.CONNECTION_BLUETOOTH_SPP.ordinal()] + " " +
                Config.serviceSettings[Config.SettingKey.HUD_ENABLED.ordinal()] + " " +
                Config.serviceSettings[Config.SettingKey.NOTIFICATIONS_ENABLED.ordinal()];
    }

    /**
     * Обрабатывает команды управления сервисом, поступающие из Flutter.
     *
     * @param method строковый идентификатор метода для Flutter
     * @return статус выполнения команды {@link Const.WorkState}
     */
    private Const.WorkState serviceStringControl(String method) {
        String[] methodArr = method.split(" ");
        try {
            switch (methodArr[0]) {
                case Const.METHOD_SET_CONNECTION:
                    return setConnection(methodArr);
                case Const.METHOD_SET_INTERFACE:
                    connectionManager.CurrentExchangeInterface = Const.Transprort.values()[Integer.parseInt(methodArr[1])];
                    connectionManager.updateConnectionStatus();
                    return Const.WorkState.OK;
                case Const.METHOD_APP_STATE:
                    return updateFlutterAppState(methodArr);
                case Const.METHOD_SET_SETTINGS:
                    return setSettings(methodArr);
                default:
                    return Const.WorkState.ERROR;
            }
        } catch (Exception e) {
            return Const.WorkState.ERROR;
        }
    }

    private Const.WorkState setConnection(String[] methodArr) {
        Const.Transprort exchangeInterface = Const.Transprort.values()[Integer.parseInt(methodArr[1])];
        Const.WorkState workState = Const.WorkState.values()[Integer.parseInt(methodArr[2])];

        if (workState == Const.WorkState.CONNECTED) {
            return connectionManager.Connect_Interface(exchangeInterface, Const.WorkState.ENABLE, Const.WorkState.ENABLE);
        } else if (workState == Const.WorkState.DISCONNECTED) {
            return connectionManager.Disconnect_Interface(exchangeInterface, Const.WorkState.ENABLE);
        } else {
            return Const.WorkState.ERROR;
        }
    }

    private Const.WorkState updateFlutterAppState(String[] methodArr) {
        switch (methodArr[1]) {
            case "resumed":
                StatusFlutterApp = Const.AppState.RESUME;
                DebugHelper.showLog("FlutterServiceBridge", "Приложение открыто");
                break;
            case "inactive":
                StatusFlutterApp = Const.AppState.INACTIVE;
                DebugHelper.showLog("FlutterServiceBridge", "Приложение приостановлено");
                break;
            case "paused":
                StatusFlutterApp = Const.AppState.PAUSED;
                DebugHelper.showLog("FlutterServiceBridge", "Приложение на паузе");
                break;
        }
        return Const.WorkState.OK;
    }

    Const.WorkState setSettings(String[] methodArr) {
        int parameter = Integer.parseInt(methodArr[1]);
        int data = Integer.parseInt(methodArr[2]);
        DebugHelper.showLog("help", methodArr[0] + " " + methodArr[1] + " " + methodArr[2]);
        Config.setServiceSetting(Config.SettingKey.values()[parameter], data);

        if (parameter == Config.SettingKey.NOTIFICATIONS_ENABLED.ordinal()) {
            if (data == Const.WorkState.ENABLE.ordinal()) {
                connectionManager.notification.enableNotification();
            } else {
                connectionManager.notification.disableNotification();
            }
        } else if (parameter == Config.SettingKey.HUD_ENABLED.ordinal()) {
            if (Const.WorkState.values()[data] == Const.WorkState.ENABLE) {
                return hudDisplay.setupHud();
            } else {
                return Const.WorkState.DISABLE;
            }
        } else if (parameter == Config.SettingKey.CONNECTION_BLUETOOTH_SPP.ordinal()) {
            if (Const.WorkState.values()[data] == Const.WorkState.ENABLE) {
                connectionManager.BluetoothSpp.startReconnect();
            } else {
                connectionManager.BluetoothSpp.stopReconnect();
            }
        }
        return Const.WorkState.OK;
    }

    /**
     * Метод вызывается при изменении статуса транспорта.
     * Отправляет обновленную информацию в Flutter, если приложение не отсоединено.
     *
     * @param method      строковый идентификатор метода для Flutter
     * @param transport   тип транспорта usb-hid, usb-cdc, bluetooth
     * @param status      статус работы
     * @param successIn   количество успешных входящих сообщений
     * @param successOut  количество успешных исходящих сообщений
     * @param errorIn     количество ошибок при приёме
     * @param errorOut    количество ошибок при отправке
     */
    @Override
    public void onTransportStatus(String method, Const.Transprort transport, Const.WorkState status,
                                  int successIn, int successOut, int errorIn, int errorOut) {
        if (StatusFlutterApp != Const.AppState.DETACHED && ChannelAndroidToDart != null) {
            int conStatus = status.ordinal();
            String message = method + " "
                    + transport.ordinal() + " "
                    + conStatus + " "
                    + conStatus + " "
                    + conStatus + " "
                    + successIn + " "
                    + successOut + " "
                    + errorIn + " "
                    + errorOut;
            sendToFlutterApp(Const.MAIN_METHOD_SERVICE, message);
        }
    }

    /**
     * Метод вызывается при получении новых данных.
     * Отправляет данные в Flutter-приложение.
     *
     * @param dataFrame объект с принятыми данными
     */
    @Override
    public void onDataReceived(DataFrame dataFrame) {
        sendToFlutterApp(Const.MAIN_ARGUMENT, dataFrame.getBufferArray());
    }

    /**
     * Отправляет метод и аргументы в Flutter через канал {@link MethodChannel}.
     *
     * @param method    имя метода для вызова во Flutter
     * @param arguments аргументы метода
     */
    public void sendToFlutterApp(String method, Object arguments) {
        ChannelAndroidToDart.invokeMethod(method, arguments, new MethodChannel.Result() {
            @Override
            public void success(Object result) {
                // Обработка успешного результата
            }
            @Override
            public void error(String errorCode, String errorMessage, Object errorDetails) {
                // Обработка ошибки
            }
            @Override
            public void notImplemented() {
                // Обработка не реализованного метода
            }
        });
    }
}
