package dixom.Service.Connection;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Base64;
import android.widget.Toast;

import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.UUID;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import dixom.Config;
import dixom.Const;
import dixom.DebugHelper;
import dixom.Service.FlutterServiceBridge;

/**
 * Класс BluetoothSpp реализует функциональность работы с Bluetooth SPP (Serial Port Profile)
 * для установления соединения, обмена данными и управления состоянием Bluetooth-соединения.
 *
 * <p>Основные возможности класса:
 * <ul>
 *   <li>Поиск и получение списка сопряжённых Bluetooth-устройств;</li>
 *   <li>Установка и разрыв соединения с выбранным устройством по SPP;</li>
 *   <li>Асинхронное подключение и обмен данными через поток {@link SPP_Exchange_Thread};</li>
 *   <li>Автоматическое переподключение при потере связи;</li>
 *   <li>Отслеживание статуса соединения через объект {@link TransportStatus};</li>
 *   <li>Обработка ошибок при передаче и приёме данных;</li>
 *   <li>Отправка данных в формате строк и байтов;</li>
 * </ul>
 * </p>
 *
 * <p>Класс предназначен для интеграции с менеджером соединений {@link ConnectionManager} и
 * приложением Flutter через {@link FlutterServiceBridge} (по необходимости).</p>
 *
 * <p>Использует стандартный UUID для Bluetooth SPP {@code "00001101-0000-1000-8000-00805F9B34FB"}.</p>
 */
public class BluetoothSpp {

    /** Bluetooth адаптер устройства */
    public BluetoothAdapter bluetoothAdapter;

    /** UUID для подключения по Bluetooth SPP */
    private final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    /** Bluetooth-сокет для передачи данных */
    private BluetoothSocket bluetoothSocket = null;

    /** Поток обмена данными через SPP */
    public SPP_Exchange_Thread SPP_Exchange_Thread;

    private final Context mContext;
    private final ConnectionManager manager;

    /** Объект для отслеживания статуса транспортного соединения */
    public final TransportStatus transportStatus;

    /**
     * Конструктор BluetoothSpp
     * @param context контекст приложения
     * @param manager менеджер соединений для взаимодействия с другими интерфейсами
     */
    public BluetoothSpp(Context context, ConnectionManager manager) {
        this.manager = manager;
        this.transportStatus = new TransportStatus(Const.Transprort.BLUETOOTH_SPP, Const.WorkState.NOT_INITIALIZED, 0, 0, 0, 0);
        this.mContext = context;
        this.bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        startReconnect();
    }

    /** Обработчик для механизма автоматического переподключения */
    private final Handler reconnectHandler = new Handler(Looper.getMainLooper());

    /** Флаг разрешения автоматического переподключения */
    private volatile boolean reconnectEnabled = false;

    /** Задача переподключения, вызываемая с задержкой */
    private final Runnable reconnectRunnable = new Runnable() {
        @Override
        public void run() {
            if (transportStatus.getState() != Const.WorkState.CONNECTED) {
                Const.WorkState state = manager.Connect_Interface(
                        Const.Transprort.BLUETOOTH_SPP,
                        Const.WorkState.CHANGED,
                        Const.WorkState.DISABLE);

                if (state != Const.WorkState.CONNECTED) {
                    // Планируем повторную попытку через 5 секунд, если не подключились
                    reconnectHandler.postDelayed(this, 5000);
                }
            } else {
                stopReconnect();
            }
        }
    };

    /**
     * Запускает автоматическое переподключение,
     * если оно не запущено и включено в настройках
     */
    public void startReconnect() {
        if (reconnectEnabled) return;
        reconnectEnabled = true;

        if (!(Config.serviceSettings[Config.SettingKey.CONNECTION_BLUETOOTH_SPP.ordinal()] == Const.WorkState.ENABLE.ordinal()))
            return;

        reconnectHandler.post(reconnectRunnable);
    }

    /**
     * Останавливает автоматическое переподключение
     */
    public void stopReconnect() {
        reconnectHandler.removeCallbacks(reconnectRunnable);
        reconnectEnabled = false;
    }

    private boolean connectionState = false;

    /**
     * Возвращает список имён сопряжённых Bluetooth-устройств
     * с проверкой прав доступа и состояния Bluetooth.
     * @return список имён устройств или сообщение об ошибке доступа/состояния
     */
    public List<String> getDeviceList() {
        List<String> pairedDevicesList = new ArrayList<>();

        if (ContextCompat.checkSelfPermission(mContext, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
            pairedDevicesList.add("Запрещен доступ к bluetooth");
            Toast.makeText(mContext, "Запрещен доступ к bluetooth", Toast.LENGTH_LONG).show();
            return pairedDevicesList;
        }

        if (bluetoothAdapter == null) {
            Toast.makeText(mContext, "Bluetooth не поддерживается на этой аппаратной платформе", Toast.LENGTH_LONG).show();
            pairedDevicesList.add("Bluetooth модуль не найден");
        } else {
            if (!bluetoothAdapter.isEnabled()) {
                Toast.makeText(mContext, "Bluetooth не включен", Toast.LENGTH_LONG).show();
                pairedDevicesList.add("Bluetooth отключен");
            }
        }

        Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();
        for (BluetoothDevice device : pairedDevices) {
            pairedDevicesList.add(device.getName());
        }
        return pairedDevicesList;
    }

    /**
     * Устанавливает соединение с Bluetooth-устройством,
     * имя которого совпадает с заданным в настройках.
     * Выполняет проверку доступности и прав.
     *
     * @return статус попытки подключения {@link Const.WorkState}
     */
    public Const.WorkState Connect() {
        if (bluetoothAdapter == null) {
            transportStatus.setState(Const.WorkState.NOT_SUPPORTED);
            return transportStatus.getState();
        }

        if (transportStatus.getState() == Const.WorkState.CONNECTED || connectionState) {
            return transportStatus.getState();
        }

        if (ContextCompat.checkSelfPermission(mContext, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
            return Const.WorkState.NO_PERMISSION;
        }

        Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();
        transportStatus.setState(Const.WorkState.CONNECTING);

        if (bluetoothAdapter.isEnabled()) {
            if (!pairedDevices.isEmpty()) {
                for (BluetoothDevice device : pairedDevices) {
                    if (device.getName().equals(Config.GetBluetoothName()) ||
                            device.getName().equals(Const.DEF_BLUETOOTH_DEVICE_2) ||
                            device.getName().equals(Const.DEF_BLUETOOTH_DEVICE)) {

                        String MAC = device.getAddress();
                        BluetoothDevice ConnectDevice = bluetoothAdapter.getRemoteDevice(MAC);
                        try {
                            bluetoothSocket = ConnectDevice.createRfcommSocketToServiceRecord(myUUID);
                            connectionState = true;
                            connectBluetoothAsync(bluetoothSocket);
                            break;
                        } catch (IOException e) {
                            e.printStackTrace();
                            transportStatus.setState(Const.WorkState.ERROR);
                        }
                    }
                    transportStatus.setState(Const.WorkState.NOT_FOUND);
                }
            } else {
                transportStatus.setState(Const.WorkState.NOT_FOUND);
            }
        } else {
            transportStatus.setState(Const.WorkState.BLUETOOTH_OFF);
        }

        return transportStatus.getState();
    }

    /**
     * Executor для выполнения подключения в фоновом потоке
     */
    private final ExecutorService bluetoothExecutor = Executors.newSingleThreadExecutor();

    /** Обработчик для постинга результатов на главный поток */
    private final Handler mainHandler = new Handler(Looper.getMainLooper());

    /**
     * Асинхронное подключение к Bluetooth-сокету
     * @param socket сокет для подключения
     */
    private void connectBluetoothAsync(BluetoothSocket socket) {
        connectionState = true;

        bluetoothExecutor.execute(() -> {
            BluetoothSocket resultSocket;

            try {
                if (ActivityCompat.checkSelfPermission(mContext, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
                    resultSocket = null;
                } else {
                    socket.connect();
                    resultSocket = socket;
                }
            } catch (IOException e) {
                e.printStackTrace();
                resultSocket = null;
            }

            BluetoothSocket finalSocket = resultSocket;

            mainHandler.post(() -> {
                connectionState = false;

                if (finalSocket != null) {
                    SPP_Exchange_Thread = new SPP_Exchange_Thread(finalSocket);
                    if (!SPP_Exchange_Thread.isAlive()) {
                        SPP_Exchange_Thread.start();
                    }
                    transportStatus.setState(Const.WorkState.CONNECTED);
                } else {
                    transportStatus.setState(Const.WorkState.NOT_AVAILABLE);
                }

                manager.Set_Current_Exchange_Interface();
                manager.updateConnectionStatus();
            });
        });
    }

    /**
     * Поток обмена данными с Bluetooth-устройством по SPP.
     * Реализует чтение и запись, обработку ошибок и контроль состояний.
     */
    public class SPP_Exchange_Thread extends Thread {

        private final InputStream connectedInputStream;
        private final OutputStream connectedOutputStream;
        private volatile boolean isStopped;

        public SPP_Exchange_Thread(BluetoothSocket socket) {
            InputStream in = null;
            OutputStream out = null;
            try {
                in = socket.getInputStream();
                out = socket.getOutputStream();
            } catch (IOException e) {
                e.printStackTrace();
            }
            connectedInputStream = in;
            connectedOutputStream = out;
        }

        /** Обработчик сообщений об ошибках передачи */
        Handler errorHandler = new Handler(Looper.getMainLooper()) {
            @Override
            public void handleMessage(Message message) {
                Const.TransferStatus transferStatus = (Const.TransferStatus) message.obj;
                if (transferStatus == Const.TransferStatus.ERROR_IN) {
                    transportStatus.update(Const.TransferStatus.ERROR_IN);
                } else if (transferStatus == Const.TransferStatus.ERROR_OUT) {
                    transportStatus.update(Const.TransferStatus.ERROR_OUT);
                }
            }
        };

        /** Отправляет сообщение об ошибке в основной поток */
        void setErrorMsg(Const.TransferStatus dir) {
            Message message = errorHandler.obtainMessage();
            message.obj = dir;
            errorHandler.sendMessage(message);
        }

        @Override
        public void run() {
            BufferedReader reader = new BufferedReader(new InputStreamReader(connectedInputStream));
            String line;
            byte ACK = 0x06; // Подтверждение
            byte NAK = 0x21; // Отрицательное подтверждение
            int crcAddedMessage = 0;

            while (!isStopped) {
                try {
                    if ((line = reader.readLine()) != null) {
                        if (line.length() > 3) {
                            try {
                                DataFrame dataFrame = new DataFrame(Const.Transprort.BLUETOOTH_SPP, line);

                                if (dataFrame.getCrc16() != -1) {
                                    if (!dataFrame.isRepeated()) {
                                        manager.Receiver.addMessageToQueue(dataFrame);
                                        crcAddedMessage = dataFrame.getCrc16();
                                    } else {
                                        if (crcAddedMessage != dataFrame.getCrc16()) {
                                            manager.Receiver.addMessageToQueue(dataFrame);
                                            crcAddedMessage = dataFrame.getCrc16();
                                        }
                                    }

                                    if (dataFrame.isResponseRequired()) {
                                        connectedOutputStream.write(ACK);
                                    }
                                } else {
                                    if (dataFrame.isResponseRequired()) {
                                        connectedOutputStream.write(NAK);
                                        setErrorMsg(Const.TransferStatus.ERROR_IN);
                                    }
                                }
                            } catch (Exception e) {
                                DebugHelper.showLog("bluetooth", "отключился");
                                setErrorMsg(Const.TransferStatus.ERROR_IN);
                            }
                        }
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                    mainHandler.post(() -> {
                        if (transportStatus.getState() == Const.WorkState.CONNECTED) {
                            manager.Disconnect_Interface(Const.Transprort.BLUETOOTH_SPP, Const.WorkState.ENABLE);
                            startReconnect();
                        }
                    });
                }
            }
        }

        private final char Stx = 2; // Начало текста
        private final char Eot = 4; // Конец передачи

        /**
         * Отправка байтовых данных по Bluetooth
         * @param dataB массив байтов для отправки
         * @return статус операции {@link Const.WorkState}
         */
        public Const.WorkState Sending_Data(byte[] dataB) {
            if (transportStatus.getState() == Const.WorkState.CONNECTED) {
                if (dataB.length < Const.MAX_MESSAGE_LENGTH) {
                    try {
                        byte[] data = Base64.encode(dataB, Base64.NO_WRAP);
                        char CRC8 = 100;
                        byte[] allData = new byte[4 + data.length];
                        int index = 0;

                        allData[index++] = (byte) Stx;
                        allData[index++] = (byte) Const.MsgType.BASE64_WHOLE;
                        allData[index++] = (byte) CRC8;
                        System.arraycopy(data, 0, allData, index, data.length);
                        index += data.length;
                        allData[index] = (byte) Eot;

                        connectedOutputStream.write(allData);
                        transportStatus.update(Const.TransferStatus.SUCCESS_OUT);
                        return Const.WorkState.OK;
                    } catch (Exception e) {
                        setErrorMsg(Const.TransferStatus.ERROR_OUT);
                        return Const.WorkState.ERROR;
                    }
                } else {
                    setErrorMsg(Const.TransferStatus.ERROR_OUT);
                    return Const.WorkState.ERROR;
                }
            } else {
                return Const.WorkState.ERROR;
            }
        }

        /**
         * Отправка строковых данных по Bluetooth
         * @param dataS строка для отправки
         * @return статус операции {@link Const.WorkState}
         */
        public Const.WorkState Sending_Data(String dataS) {
            if (transportStatus.getState() == Const.WorkState.CONNECTED) {
                byte[] dataB = dataS.getBytes();
                if (dataB.length < Const.MAX_MESSAGE_LENGTH) {
                    try {
                        char CRC8 = 100;
                        byte[] allData = new byte[4 + dataB.length];
                        int index = 0;

                        allData[index++] = (byte) Stx;
                        allData[index++] = (byte) Const.MsgType.UTF8_WHOLE;
                        allData[index++] = (byte) CRC8;
                        System.arraycopy(dataB, 0, allData, index, dataB.length);
                        index += dataB.length;
                        allData[index] = (byte) Eot;

                        connectedOutputStream.write(allData);
                        transportStatus.update(Const.TransferStatus.SUCCESS_OUT);
                        return Const.WorkState.OK;
                    } catch (Exception e) {
                        setErrorMsg(Const.TransferStatus.ERROR_OUT);
                        return Const.WorkState.ERROR;
                    }
                } else {
                    return Const.WorkState.ERROR;
                }
            } else {
                return Const.WorkState.ERROR;
            }
        }

        /**
         * Останавливает работу потока обмена данными
         */
        public void stopThis() {
            isStopped = true;
        }
    }

    /**
     * Разрывает соединение Bluetooth и освобождает ресурсы
     * @return текущий статус соединения {@link Const.WorkState}
     */
    public Const.WorkState Disconnect() {
        if (transportStatus.getState() == Const.WorkState.DISCONNECTED) {
            return transportStatus.getState();
        }

        try {
            if (SPP_Exchange_Thread != null) {
                SPP_Exchange_Thread.stopThis();
            }
            bluetoothSocket.close();
            transportStatus.setState(Const.WorkState.DISCONNECTED);
            return transportStatus.getState();
        } catch (IOException e) {
            e.printStackTrace();
            transportStatus.setState(Const.WorkState.ERROR);
            return transportStatus.getState();
        }
    }
}
