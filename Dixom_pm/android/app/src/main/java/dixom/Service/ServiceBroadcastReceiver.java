package dixom.Service;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.os.Build;

import androidx.core.app.ActivityCompat;

import dixom.Config;
import dixom.Const;
import dixom.Service.Connection.ConnectionManager;

/**
 * ServiceBroadcastReceiver — системный {@link BroadcastReceiver},
 * обеспечивающий взаимодействие фонового сервиса Dixom
 * с событиями операционной системы и внешними командами.
 *
 * <p>
 * Класс принимает и обрабатывает широковещательные события (Broadcast),
 * связанные с:
 * <ul>
 *     <li>отправкой команд и данных в активный интерфейс обмена</li>
 *     <li>изменением состояния Bluetooth-адаптера</li>
 *     <li>отключением USB-устройств</li>
 *     <li>состоянием Flutter-приложения</li>
 * </ul>
 * </p>
 *
 * <p>
 * Вся логика реакции на события делегируется в {@link ConnectionManager},
 * который отвечает за управление физическими интерфейсами
 * (USB HID, USB CDC, Bluetooth SPP).
 * </p>
 *
 * <p>
 * Основные функции класса:
 * <ul>
 *     <li>Маршрутизация строковых и бинарных данных,
 *         поступающих через Broadcast, в активный транспорт</li>
 *     <li>Корректное отключение интерфейсов при системных событиях
 *         (выключение Bluetooth, извлечение USB-устройства)</li>
 *     <li>Управление механизмом автоматического переподключения Bluetooth</li>
 * </ul>
 * </p>
 *
 * <p>
 * Класс не хранит состояние соединений и не выполняет
 * низкоуровневые операции ввода-вывода —
 * он выполняет роль адаптера между системой Android
 * и сервисной логикой приложения.
 * </p>
 *
 * <p>
 * Используется исключительно внутри сервисного слоя приложения
 * и не предназначен для прямого взаимодействия с UI.
 * </p>
 */

public class ServiceBroadcastReceiver extends BroadcastReceiver {

    /** Менеджер подключений */
    private final ConnectionManager manager;

    public ServiceBroadcastReceiver(ConnectionManager manager) {
        this.manager = manager;
    }

    // =========================================================
    // Broadcast handling
    // =========================================================

    @Override
    public void onReceive(Context context, Intent intent) {
        if (intent == null) return;

        String action = intent.getAction();
        if (action == null) return;

        switch (action) {

            case Const.ACTION_COMMAND_SEND:
                handleSendString(intent);
                break;

            case Const.ACTION_COMMAND_SEND_BYTES:
                handleSendBytes(intent);
                break;

            case Const.ACTION_FLUTTER_APP_STATE:
                handleFlutterAppState(intent);
                break;

            case BluetoothAdapter.ACTION_STATE_CHANGED:
                handleBluetoothAdapterState(intent);
                break;

            case UsbManager.ACTION_USB_DEVICE_DETACHED:
                handleUsbDetached(intent);
                break;
        }
    }


    // =========================================================
    // Handlers
    // =========================================================

    /** Обработка отправки строковых данных */
    private void handleSendString(Intent intent) {
        String data = intent.getStringExtra(Const.BROADCAST_EXTRA_DATA);
        if (data != null) {
            manager.Sending_Data_To_Interface(data);
        }
    }

    /** Обработка отправки байтов */
    private void handleSendBytes(Intent intent) {
        byte[] data = intent.getByteArrayExtra(Const.BROADCAST_EXTRA_DATA);
        if (data != null) {
            manager.Sending_Data_To_Interface(data);
        }
    }

    /** Состояние Flutter-приложения */
    private void handleFlutterAppState(Intent intent) {
        String message = intent.getStringExtra(Const.NAME_APP_STATE);
        if (message == null) return;

        if ("app start".equals(message)) {
            // зарезервировано
        } else if ("app close".equals(message)) {
            // зарезервировано
        }
    }

    /** Состояние Bluetooth-адаптера */
    private void handleBluetoothAdapterState(Intent intent) {
        int state = intent.getIntExtra(
                BluetoothAdapter.EXTRA_STATE,
                BluetoothAdapter.ERROR
        );

        if (state == BluetoothAdapter.STATE_OFF) {
            manager.Disconnect_Interface(
                    Const.Transprort.BLUETOOTH_SPP,
                    Const.WorkState.ENABLE
            );
            manager.BluetoothSpp.stopReconnect();
        }else if(state == BluetoothAdapter.STATE_ON){
            manager.BluetoothSpp.startReconnect();
        }
    }

    /** Отключение USB-устройства */
    private void handleUsbDetached(Intent intent) {
        UsbDevice device =
                intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);

        if (device == null) return;

        if (device.getVendorId() == Const.VENDOR_ID &&
                (device.getProductId() == Const.PRODUCT_ID
                        || device.getProductId() == Const.PRODUCT_ID_BOOT_m
                        || device.getProductId() == Const.PRODUCT_ID_BOOT_c12)) {

            manager.Disconnect_Interface(
                    Const.Transprort.USB_HID,
                    Const.WorkState.DISABLE
            );

            manager.Disconnect_Interface(
                    Const.Transprort.USB_CDC,
                    Const.WorkState.ENABLE
            );
        }
    }

    // =========================================================
    // Utils
    // =========================================================

    /**
     * Проверка, что Bluetooth-устройство является целевым.
     *
     * @return true — устройство подходит
     */
    private boolean isTargetBluetoothDevice(Context context, BluetoothDevice device) {
        if (device == null) return false;

        /* Проверка разрешений Android 12+ */
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            if (ActivityCompat.checkSelfPermission(
                    context,
                    Manifest.permission.BLUETOOTH_CONNECT
            ) != PackageManager.PERMISSION_GRANTED) {
                return false;
            }
        }

        String deviceName = device.getName();
        if (deviceName == null) return false;

        return deviceName.equals(Const.DEF_BLUETOOTH_DEVICE)
                || deviceName.equals(Const.DEF_BLUETOOTH_DEVICE_2);
    }
}
