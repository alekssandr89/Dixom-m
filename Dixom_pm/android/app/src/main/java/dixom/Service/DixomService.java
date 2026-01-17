package dixom.Service;

import android.annotation.SuppressLint;
import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbManager;
import android.os.Build;
import android.os.IBinder;

import androidx.annotation.NonNull;

import dixom.Config;
import dixom.Const;
import dixom.DebugHelper;
import dixom.Service.Connection.ConnectionManager;

/**
 * Главный сервис приложения.
 *
 * Назначение:
 *  - Управление подключениями (Bluetooth / USB)
 *  - Получение системных Broadcast-событий
 *  - Связь Flutter ↔ Android через FlutterServiceBridge
 *
 * Жизненный цикл:
 *  - onCreate()   — инициализация
 *  - onStartCommand() — запуск логики и Flutter engine
 *  - onDestroy()  — корректное завершение
 */
public class DixomService extends Service {

    public ConnectionManager connectionManager;
    private BroadcastReceiver serviceBroadcast;
    private final Intent intentForServiceMessage = new Intent(Const.ACTION_COMMAND_SEND_TO_INTERFACE);
    private FlutterServiceBridge flutterBridge;

    // =========================================================
    // Service lifecycle
    // =========================================================

    /**
     * Вызывается при создании сервиса.
     * Здесь выполняется вся базовая инициализация:
     *  - очередность ВАЖНА!!!
     */
    @SuppressLint("UnspecifiedRegisterReceiverFlag")
    @Override
    public void onCreate() {
        super.onCreate();

        DebugHelper.init(this);
        Config.initializeServiceSettings(this);
        HudDisplay hudDisplay = new HudDisplay(this);
        connectionManager = new ConnectionManager(this,this, hudDisplay);
        flutterBridge = new FlutterServiceBridge(this, connectionManager, hudDisplay);
        serviceBroadcast = new ServiceBroadcastReceiver(connectionManager);
        IntentFilter filter = createIntentFilter();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
            registerReceiver(serviceBroadcast, filter, Context.RECEIVER_NOT_EXPORTED);
        } else {
            registerReceiver(serviceBroadcast, filter);
        }
    }
    /**
     * Вызывается при старте сервиса.
     * Здесь:
     *  - запускается Flutter engine
     *  - выполняется подключение интерфейсов
     */
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {

        flutterBridge.startFlutterEngine();

        /* Подключение интерфейсов обмена */
        connectionManager.Connect_Interface(
                Const.Transprort.BLUETOOTH_SPP,
                Const.WorkState.DISABLE,
                Const.WorkState.DISABLE
        );

        connectionManager.Connect_Interface(
                Const.Transprort.USB_CDC,
                Const.WorkState.DISABLE,
                Const.WorkState.DISABLE
        );

        connectionManager.Connect_Interface(
                Const.Transprort.USB_HID,
                Const.WorkState.CHANGED,
                Const.WorkState.DISABLE
        );

        connectionManager.notification.startDixomForegroundService();

        return START_STICKY;
    }

    /**
     * Вызывается при удалении задачи из recent apps.
     * Используется для автоматического перезапуска сервиса.
     */
    @Override
    public void onTaskRemoved(Intent rootIntent) {
        Intent restartIntent = new Intent(getApplicationContext(), DixomService.class);
        restartIntent.setPackage(getPackageName());
        startService(restartIntent);
        super.onTaskRemoved(rootIntent);
    }

    /**
     * Корректное завершение работы сервиса.
     *  - отключение интерфейсов
     *  - остановка Flutter engine
     *  - снятие BroadcastReceiver
     */
    @Override
    public void onDestroy() {

        connectionManager.Disconnect_Interface(
                Const.Transprort.BLUETOOTH_SPP,
                Const.WorkState.DISABLE
        );
        connectionManager.Disconnect_Interface(
                Const.Transprort.USB_CDC,
                Const.WorkState.DISABLE
        );
        connectionManager.Disconnect_Interface(
                Const.Transprort.USB_HID,
                Const.WorkState.ENABLE
        );

        FlutterServiceBridge.StatusFlutterApp = Const.AppState.NOT_ALLOWED;

        flutterBridge.destroyFlutterEngine();

        if (serviceBroadcast != null) {
            unregisterReceiver(serviceBroadcast);
        }

        super.onDestroy();
    }

    /**
     * Сервис не поддерживает bind.
     */
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    // =========================================================
    // Broadcast & messaging
    // =========================================================

    /**
     * Формирование IntentFilter для системных событий.
     */
    @NonNull
    private static IntentFilter createIntentFilter() {
        IntentFilter filter = new IntentFilter(Const.ACTION_COMMAND_SEND);

        filter.addAction(BluetoothAdapter.ACTION_STATE_CHANGED);
        filter.addAction(BluetoothDevice.ACTION_ACL_CONNECTED);
        filter.addAction(BluetoothDevice.ACTION_ACL_DISCONNECTED);
        filter.addAction(BluetoothDevice.ACTION_ACL_DISCONNECT_REQUESTED);

        filter.addAction(UsbManager.ACTION_USB_DEVICE_DETACHED);

        filter.addAction(Const.ACTION_COMMAND_STM_CHECK);
        filter.addAction(Const.ACTION_COMMAND_SEND_BYTES);
        filter.addAction(Const.ACTION_FLUTTER_APP_STATE);

        return filter;
    }

    /**
     * Отправка сообщения из сервиса интерфейсам через Broadcast.
     *
     * @param key ключ сообщения
     * @param msg данные
     */
    public void sendServiceMessageToInterfaces(String key, String msg) {
        intentForServiceMessage.putExtra(Const.BROADCAST_EXTRA_KEY, key);
        intentForServiceMessage.putExtra(Const.BROADCAST_EXTRA_DATA, msg);
        sendBroadcast(intentForServiceMessage);
    }
}
