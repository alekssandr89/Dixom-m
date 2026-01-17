package dixom.Service.Connection;

import static dixom.Const.ACTION_USB_PERMISSION;

import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.hardware.usb.UsbConstants;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbEndpoint;
import android.hardware.usb.UsbInterface;
import android.hardware.usb.UsbManager;
import android.hardware.usb.UsbRequest;
import android.os.Build;

import java.nio.ByteBuffer;
import java.util.HashMap;

import dixom.Config;
import dixom.Const;
import dixom.Service.FlutterServiceBridge;


public class UsbHid{

    public UsbInterface usbInterface;
    public UsbDevice usbDevice;
    private UsbEndpoint usbEndpointRx;
    private UsbEndpoint usbEndpointTx;
    private UsbDeviceConnection usbDeviceConnection;
    private UsbRequest receiver;
    private ReceiverThreadUsbHID receiverThreadUsbHid;
    Context mContext;
    private final ConnectionManager manager;
    public final TransportStatus transportStatus;

    public UsbHid(Context context, ConnectionManager manager) {
        transportStatus = new TransportStatus(Const.Transprort.USB_HID, Const.WorkState.NOT_INITIALIZED, 0, 0, 0, 0);                  //Состояние интерфейса Usb CDC
        this.manager = manager;
        mContext = context;
    }

    public Const.WorkState Connect() {

        UsbManager usbManager = (UsbManager) mContext.getSystemService(Context.USB_SERVICE);

        if(transportStatus.getState() == Const.WorkState.CONNECTED){
            return transportStatus.getState() ;
        }

        transportStatus.setState(Const.WorkState.ERROR);
        usbDevice = null;

        if (usbManager != null) {
            try {

                HashMap<String, UsbDevice> devices = usbManager.getDeviceList();

                for (HashMap.Entry<String, UsbDevice> devUsb : devices.entrySet()) {

                    usbDevice = devUsb == null ? null : devUsb.getValue();

                    if (usbDevice != null && usbDevice.getVendorId() == Const.VENDOR_ID
                            && (usbDevice.getProductId() == Const.PRODUCT_ID
                                    || usbDevice.getProductId() == Const.PRODUCT_ID_BOOT_m
                                    || usbDevice.getProductId() == Const.PRODUCT_ID_BOOT_c12)) {
                        try {

                            // Проверяем разрешение подключения к девайсу
                            if (!usbManager.hasPermission(usbDevice)) {
                                // Если нет разрешения открываем диалоговое окно для запроса на разрешение
                                // подключения
                                PendingIntent permissionIntent = PendingIntent.getBroadcast(mContext, 0,
                                        new Intent(ACTION_USB_PERMISSION), PendingIntent.FLAG_IMMUTABLE);
                                usbManager.requestPermission(usbDevice, permissionIntent);
                                transportStatus.setState(Const.WorkState.NO_PERMISSION);
                            } else {
                                // Если есть разрешение то подключаемся к девайсу
                                transportStatus.setState(Const.WorkState.CONNECTING);
                                usbDeviceConnection = usbManager.openDevice(usbDevice);
                                usbInterface = usbDevice.getInterface(1);
                                if (usbDeviceConnection != null) {
                                    try {
                                        usbDeviceConnection.claimInterface(usbInterface, true);
                                        if (UsbConstants.USB_DIR_OUT == usbInterface.getEndpoint(1).getDirection()) {
                                            usbEndpointTx = usbInterface.getEndpoint(1);

                                            if (UsbConstants.USB_DIR_IN == usbInterface.getEndpoint(0)
                                                    .getDirection()) {
                                                usbEndpointRx = usbInterface.getEndpoint(0);
                                                try {
                                                    receiver = new UsbRequest();
                                                    if (receiver.initialize(usbDeviceConnection, usbEndpointRx)) {
                                                        // Успешное подключение, запускаем поток приёма данных о USB HID
                                                        receiverThreadUsbHid = new ReceiverThreadUsbHID();
                                                        receiverThreadUsbHid.start();
                                                        transportStatus.setState(Const.WorkState.CONNECTED);
                                                    } else {
                                                        transportStatus.setState(manager.Disconnect_Interface(Const.Transprort.USB_HID,  Const.WorkState.ENABLE));
                                                    }
                                                } catch (Exception ex) {
                                                    transportStatus.setState(Const.WorkState.ERROR);
                                                }
                                            } else {
                                                transportStatus.setState(Const.WorkState.NOT_FOUND);
                                            }
                                        } else {
                                            transportStatus.setState(Const.WorkState.NOT_FOUND);
                                        }
                                    } catch (Exception e) {
                                        transportStatus.setState( Const.WorkState.ERROR);
                                    }
                                } else {
                                    transportStatus.setState(Const.WorkState.ERROR);
                                }
                            }
                        } catch (Exception ex) {
                            transportStatus.setState(Const.WorkState.NO_PERMISSION);
                        }
                        break;
                    } else {
                        transportStatus.setState(Const.WorkState.NOT_FOUND);
                    }
                }
            } catch (Exception ex) {
                transportStatus.setState(Const.WorkState.ERROR);
            }
        }

        return transportStatus.getState();
    }

    public Const.WorkState Disconnect() {

        if (transportStatus.getState() == Const.WorkState.DISCONNECTED) {
            return transportStatus.getState();
        }

        if (usbDevice != null) {
            if (receiverThreadUsbHid != null) {
                receiverThreadUsbHid.stopThis();
            }
            usbDeviceConnection.releaseInterface(usbInterface);
            usbDeviceConnection.close();
            usbDevice = null;
            usbEndpointTx = null;
            usbEndpointRx = null;
            transportStatus.setState(Const.WorkState.DISCONNECTED);
            return Const.WorkState.DISCONNECTED;
        } else {
            transportStatus.setState(Const.WorkState.ERROR);
            return Const.WorkState.ERROR;
        }
    }

    public Const.WorkState Sending_Data(String dataS) {

        byte[] dataB = dataS.getBytes();
        return Sending_Data(dataB);
    }

    public Const.WorkState Sending_Data(byte[] dataB) {

        if (dataB.length != 0 && transportStatus.getState() == Const.WorkState.CONNECTED) {
            byte[] out_prefix = new byte[65];
            out_prefix[0] = 1;
            for (int i = 0; i < dataB.length; i++) {
                if (i < 64) {
                    out_prefix[i + 1] = dataB[i];
                } else {
                    break;
                }
            }
            try {
                if(usbDeviceConnection.bulkTransfer(usbEndpointTx, out_prefix, out_prefix.length, 1000) != -1){
                    transportStatus.update( Const.TransferStatus.SUCCESS_OUT);
                    return Const.WorkState.OK;

                }else{
                    transportStatus.update( Const.TransferStatus.ERROR_OUT);
                    return Const.WorkState.ERROR;
                }

            } catch (Exception ex) {
                transportStatus.setState(manager.Disconnect_Interface( Const.Transprort.USB_HID,  Const.WorkState.ENABLE));

                return transportStatus.getState();
            }
        } else {
            return Const.WorkState.ERROR;
        }
    }



    /**
     * Поток приёма данных от USB HID устройства.
     *
     * Отвечает за:
     *  - постановку UsbRequest на приём
     *  - ожидание данных (blocking)
     *  - разбор HID-пакетов по протоколу
     *  - сборку целых и составных сообщений
     *  - передачу сообщений в общую очередь
     */
    private class ReceiverThreadUsbHID extends Thread {

        /** Флаг остановки потока */
        private volatile boolean stopped;

        /** Размер HID пакета (берётся из endpoint, без магических чисел) */
        private final int packetSize = usbEndpointRx.getMaxPacketSize();

        /** Буфер для приёма одного HID пакета */
        private final ByteBuffer buffer = ByteBuffer.allocate(packetSize);

        /** Сообщение, собираемое из нескольких пакетов */
        private DataFrame dataFrame;

        /**
         * Основной цикл приёма данных.
         * Блокируется в requestWait() до получения HID пакета.
         */
        @Override
        public void run() {
            try {
                while (!stopped && usbDeviceConnection != null && receiver != null) {

                    // Ставим запрос на приём данных
                    queueRequest();

                    // Ожидаем завершения запроса
                    if (usbDeviceConnection.requestWait() != receiver) {
                        continue;
                    }

                    buffer.flip();

                    // Минимальный размер пакета:
                    // reportId (1) + msgType (1) + length (1)
                    if (buffer.remaining() < 3) {
                        continue;
                    }

                    // Первый байт — report ID (может быть 0)
                    buffer.get();

                    // Тип сообщения (беззнаковый байт)
                    char msgType = (char) (buffer.get() & 0xFF);

                    // Длина полезной нагрузки
                    int length = buffer.get() & 0xFF;

                    // Обработка сообщения по типу
                    handleMessage(msgType, length);
                }
            } catch (Exception e) {
                // Любая ошибка приёма считается обрывом соединения
                manager.Disconnect_Interface(
                        Const.Transprort.USB_HID,
                        Const.WorkState.ENABLE
                );
            }
        }

        /**
         * Ставит UsbRequest в очередь на приём данных.
         * Учитывает различия API до и после Android O.
         */
        private void queueRequest() {
            buffer.clear();
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                receiver.queue(buffer);
            } else {
                receiver.queue(buffer, packetSize);
            }
        }

        /**
         * Обрабатывает входящее сообщение по его типу.
         *
         * @param msgType тип сообщения протокола
         * @param length  ожидаемая длина полезной нагрузки
         */
        private void handleMessage(char msgType, int length) {

            if (msgType == Const.MsgType.BYTE_WHOLE) {

                // Целый кадр в одном пакете
                DataFrame dataFrame = new DataFrame(
                        Const.Transprort.USB_HID, msgType, length
                );
                readPayload(dataFrame, length);
                manager.Receiver.addMessageToQueue(dataFrame);

            } else if (msgType == Const.MsgType.BYTE_PART_START) {

                // Начало составного кадра
                dataFrame = new DataFrame(
                        Const.Transprort.USB_HID, msgType, length
                );
                readPayload(dataFrame, length);

            } else if (msgType == Const.MsgType.BYTE_PART && dataFrame != null) {

                // Продолжение составного кадра
                readPayload(dataFrame, dataFrame.getBuffer().remaining());

            } else if (msgType == Const.MsgType.BYTE_PART_END && dataFrame != null) {

                // Завершение составного кадра
                readPayload(dataFrame, dataFrame.getBuffer().remaining());
                manager.Receiver.addMessageToQueue(dataFrame);
                dataFrame = null;
            }
        }

        /**
         * Читает полезную нагрузку из HID буфера в сообщение.
         *
         * @param dataFrame  сообщение, в которое пишем данные
         * @param maxBytes   максимальное количество байт для чтения
         */
        private void readPayload(DataFrame dataFrame, int maxBytes) {
            for (int i = 0;
                 i < maxBytes && buffer.hasRemaining() && dataFrame.getBuffer().hasRemaining();
                 i++) {
                dataFrame.getBuffer().put(buffer.get());
            }
        }

        /**
         * Корректно останавливает поток приёма данных.
         * Вызывается при отключении устройства или остановке интерфейса.
         */
        public void stopThis() {
            stopped = true;
            try {
                if (receiver != null) receiver.cancel();
            } catch (Exception ignored) {}
        }
    }

}
