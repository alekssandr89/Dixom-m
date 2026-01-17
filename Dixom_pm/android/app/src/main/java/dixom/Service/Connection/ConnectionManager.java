package dixom.Service.Connection;

import android.app.Service;
import android.content.Context;
import dixom.Config;
import dixom.Const;
import dixom.Service.DixomService;
import dixom.Service.FlutterServiceBridge;
import dixom.Service.HudDisplay;
import dixom.Service.Notification;

/**
 * Класс ConnectionManager управляет всеми транспортными интерфейсами
 * (Bluetooth SPP, USB HID, USB CDC) для обмена данными с устройством.
 * <p>
 * Он отвечает за подключение, отключение и выбор активного интерфейса,
 * а также за отправку и получение данных через эти интерфейсы.
 * </p>
 * <p>
 * Основные функции:
 * <ul>
 *     <li>Инициализация и хранение объектов интерфейсов передачи данных</li>
 *     <li>Поддержка статусов подключения для каждого интерфейса</li>
 *     <li>Выбор текущего активного интерфейса для обмена данными</li>
 *     <li>Управление потоками приёма данных</li>
 *     <li>Отправка данных (строковых и байтовых) на активный интерфейс</li>
 *     <li>Обновление уведомлений о статусе подключения</li>
 * </ul>
 * </p>
 *
 * <p>Использование:</p>
 * <pre>{@code
 * ConnectionManager connectionManager = new ConnectionManager(service, context, hudDisplay);
 * connectionManager.Connect_Interface(Const.Transprort.BLUETOOTH_SPP, Const.WorkState.ENABLE, Const.WorkState.DISABLE);
 * }</pre>
 */
public class ConnectionManager {

    private final int DATA_TYPE_BYTE = 0;
    private final int DATA_TYPE_STRING = 1;

    /** Объект приёмника входящих сообщений */
    final public Receiver Receiver;

    /** Объект для работы с USB HID */
    public UsbHid UsbHid;

    /** Объект для работы с USB CDC */
    public UsbCdc UsbCdc;

    /** Объект для работы с Bluetooth SPP */
    public BluetoothSpp BluetoothSpp;

    /** Объект для управления уведомлениями */
    public Notification notification;

    /** Текущий статус подключения (есть ли хоть одно подключение) */
    public Const.WorkState StatusConnection = Const.WorkState.NOT_INITIALIZED;

    /** Статус потока чтения сообщений из очереди */
    public Const.WorkState StatusMessageReaderThread = Const.WorkState.DISABLE;

    /** Текущий активный интерфейс обмена данными */
    public Const.Transprort CurrentExchangeInterface = Const.Transprort.NONE;

    /** Предыдущий активный интерфейс обмена данными */
    public Const.Transprort OldCurrentExchangeInterface = Const.Transprort.NONE;

    /**
     * Конструктор, инициализирующий все транспортные интерфейсы и уведомления.
     *
     * @param mService экземпляр Android Service
     * @param context контекст приложения
     * @param hudDisplay объект для отображения HUD-информации
     */
    public ConnectionManager(Service mService, Context context, HudDisplay hudDisplay) {
        Receiver = new Receiver(context, this, hudDisplay);
        BluetoothSpp = new BluetoothSpp(context, this);
        UsbHid = new UsbHid(context, this);
        UsbCdc = new UsbCdc(context, this);
        notification = new Notification(mService, this);
    }

    /**
     * Определяет текущий активный интерфейс на основе состояний подключения.
     * Выбирает интерфейс с приоритетом: USB HID > USB CDC > Bluetooth SPP.
     */
    public void Set_Current_Exchange_Interface() {
        if (UsbHid.transportStatus.getState() == Const.WorkState.CONNECTED) {
            CurrentExchangeInterface = Const.Transprort.USB_HID;
        } else if (UsbCdc.transportStatus.getState() == Const.WorkState.CONNECTED) {
            CurrentExchangeInterface = Const.Transprort.USB_CDC;
        } else if (BluetoothSpp.transportStatus.getState() == Const.WorkState.CONNECTED) {
            CurrentExchangeInterface = Const.Transprort.BLUETOOTH_SPP;
        } else {
            CurrentExchangeInterface = Const.Transprort.NONE;
        }
    }

    /**
     * Метод подключения к выбранному интерфейсу или ко всем доступным.
     * Поддерживает режимы автоматического и ручного подключения.
     *
     * @param Interface интерфейс подключения
     * @param UpdateConnectionStatus флаг обновления статуса подключения
     * @param manualConnection флаг ручного подключения
     * @return состояние подключения после выполнения
     */
    public Const.WorkState Connect_Interface(Const.Transprort Interface, Const.WorkState UpdateConnectionStatus, Const.WorkState manualConnection) {
        Const.WorkState ChangedStateHID = UsbHid.transportStatus.getState();
        Const.WorkState ChangedStateCDC = UsbCdc.transportStatus.getState();
        Const.WorkState ChangedStateBluetoothSpp = BluetoothSpp.transportStatus.getState();
        Const.WorkState workState = Const.WorkState.ERROR;

        if ((Interface == Const.Transprort.BLUETOOTH_SPP && BluetoothSpp.transportStatus.getState() != Const.WorkState.CONNECTED) &&
                (Config.serviceSettings[Config.SettingKey.CONNECTION_BLUETOOTH_SPP.ordinal()] == Const.WorkState.ENABLE.ordinal() || manualConnection == Const.WorkState.ENABLE)) {
            workState = BluetoothSpp.Connect();
        }

        if ((Interface == Const.Transprort.USB_CDC && UsbCdc.transportStatus.getState() != Const.WorkState.CONNECTED) &&
                (Config.serviceSettings[Config.SettingKey.CONNECTION_USB_CDC.ordinal()] == Const.WorkState.ENABLE.ordinal() || manualConnection == Const.WorkState.ENABLE)) {
            workState = UsbCdc.Connect();
            if (UsbCdc.transportStatus.getState() == Const.WorkState.CONNECTED) {
                Set_Current_Exchange_Interface();
            }
        }

        if ((Interface == Const.Transprort.USB_HID && UsbHid.transportStatus.getState() != Const.WorkState.CONNECTED) &&
                (Config.serviceSettings[Config.SettingKey.CONNECTION_USB_HID.ordinal()] == Const.WorkState.ENABLE.ordinal() || manualConnection == Const.WorkState.ENABLE)) {
            workState = UsbHid.Connect();
            if (UsbHid.transportStatus.getState() == Const.WorkState.CONNECTED) {
                Set_Current_Exchange_Interface();
            }
        }

        if (UpdateConnectionStatus == Const.WorkState.ENABLE) {
            updateConnectionStatus();
        } else if (UpdateConnectionStatus == Const.WorkState.CHANGED) {
            if (ChangedStateHID != UsbHid.transportStatus.getState() || ChangedStateCDC != UsbCdc.transportStatus.getState() || ChangedStateBluetoothSpp != BluetoothSpp.transportStatus.getState()) {
                updateConnectionStatus();
            }
        }

        return workState;
    }

    /**
     * Метод отключения от выбранного интерфейса или от всех.
     *
     * @param Interface интерфейс отключения
     * @param UpdateConnectionStatus флаг обновления статуса подключения
     * @return состояние после отключения
     */
    public Const.WorkState Disconnect_Interface(Const.Transprort Interface, Const.WorkState UpdateConnectionStatus) {
        Const.WorkState WorkResult = Const.WorkState.ERROR;

        if ((Interface == Const.Transprort.USB_HID) && UsbHid.transportStatus.getState() == Const.WorkState.CONNECTED) {
            Sending_Data_To_Interface("0 17 " + Const.Transprort.USB_HID.ordinal());
            WorkResult = UsbHid.Disconnect();
            Set_Current_Exchange_Interface();
        }

        if ((Interface == Const.Transprort.BLUETOOTH_SPP) && BluetoothSpp.transportStatus.getState() == Const.WorkState.CONNECTED) {
            if (BluetoothSpp != null) {
                WorkResult = BluetoothSpp.Disconnect();
            }
            Set_Current_Exchange_Interface();
        }

        if ((Interface == Const.Transprort.USB_CDC) && UsbCdc.transportStatus.getState() == Const.WorkState.CONNECTED) {
            Sending_Data_To_Interface("0 17 " + Const.Transprort.USB_CDC.ordinal());
            WorkResult = UsbCdc.Disconnect();
            Set_Current_Exchange_Interface();
        }

        if (UpdateConnectionStatus == Const.WorkState.ENABLE) {
            updateConnectionStatus();
        }
        return WorkResult;
    }

    /**
     * Обновляет уведомления о состоянии подключения
     * и управляет запуском/остановкой потока приёма данных.
     */
    public void updateConnectionStatus() {
        if (BluetoothSpp.transportStatus.getState() == Const.WorkState.CONNECTED ||
                UsbCdc.transportStatus.getState() == Const.WorkState.CONNECTED ||
                UsbHid.transportStatus.getState() == Const.WorkState.CONNECTED) {

            if (StatusConnection == Const.WorkState.DISCONNECTED || OldCurrentExchangeInterface != CurrentExchangeInterface) {
                OldCurrentExchangeInterface = CurrentExchangeInterface;
                Receiver.launchMessageCheckerThread();
                StatusConnection = Const.WorkState.CONNECTED;
                notification.updateForegroundNotification();
            }
            Sending_Data_To_Interface("0 5 "); // Запрос информации о интерфейсе обмена

        } else if (StatusConnection == Const.WorkState.CONNECTED) {
            OldCurrentExchangeInterface = Const.Transprort.NONE;
            StatusConnection = Const.WorkState.DISCONNECTED;
            Receiver.stopMessageCheckerThread();
            Set_Current_Exchange_Interface();
            notification.updateForegroundNotification();
        }
    }

    /**
     * Отправляет строковые данные на активный интерфейс.
     *
     * @param dataS строка данных для отправки
     * @return результат отправки
     */
    public Const.WorkState Sending_Data_To_Interface(String dataS) {
        byte[] dataB = new byte[0];
        return Sending_Data_To_Current_Interface(DATA_TYPE_STRING, dataS, dataB);
    }

    /**
     * Отправляет байтовые данные на активный интерфейс.
     *
     * @param dataB байтовый массив данных для отправки
     * @return результат отправки
     */
    public Const.WorkState Sending_Data_To_Interface(byte[] dataB) {
        String dataS = "";
        return Sending_Data_To_Current_Interface(DATA_TYPE_BYTE, dataS, dataB);
    }

    /**
     * Внутренний метод отправки данных (строковых или байтовых) на активный интерфейс.
     *
     * @param dataType тип данных (0 — байты, 1 — строка)
     * @param dataS строка данных
     * @param dataB байты данных
     * @return результат отправки
     */
    private Const.WorkState Sending_Data_To_Current_Interface(int dataType, String dataS, byte[] dataB) {
        if (CurrentExchangeInterface == Const.Transprort.USB_HID) {
            if (UsbHid == null) return Const.WorkState.ERROR;
            if (dataType == DATA_TYPE_STRING) {
                return UsbHid.Sending_Data(dataS);
            } else if (dataType == DATA_TYPE_BYTE) {
                return UsbHid.Sending_Data(dataB);
            }
        } else if (CurrentExchangeInterface == Const.Transprort.BLUETOOTH_SPP) {
            if (BluetoothSpp == null) return Const.WorkState.ERROR;
            if (dataType == DATA_TYPE_STRING) {
                return BluetoothSpp.SPP_Exchange_Thread.Sending_Data(dataS);
            } else if (dataType == DATA_TYPE_BYTE) {
                return BluetoothSpp.SPP_Exchange_Thread.Sending_Data(dataB);
            }
            try {
                Thread.sleep(500);
            } catch (InterruptedException ignore) {
            }
        } else if (CurrentExchangeInterface == Const.Transprort.USB_CDC) {
            if (UsbCdc.transportStatus.getState() == Const.WorkState.CONNECTED) {
                if (UsbCdc == null) return Const.WorkState.ERROR;
                if (dataType == DATA_TYPE_STRING) {
                    return UsbCdc.Sending_Data(dataS);
                } else if (dataType == DATA_TYPE_BYTE) {
                    return UsbCdc.Sending_Data(dataB);
                }
            }
        }
        return Const.WorkState.ERROR;
    }
}
