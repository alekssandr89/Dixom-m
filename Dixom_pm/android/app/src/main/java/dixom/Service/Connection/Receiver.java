package dixom.Service.Connection;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import dixom.Config;
import dixom.Const;
import dixom.DebugHelper;
import dixom.Service.FlutterServiceBridge;
import dixom.Service.HudDisplay;
import io.flutter.plugin.common.MethodChannel;

/**
 * Класс Receiver отвечает за приём, буферизацию и обработку входящих данных,
 * полученных через разные транспортные интерфейсы (USB CDC, USB HID, Bluetooth SPP).
 *
 * <p>Основные обязанности класса:
 * <ul>
 *   <li>Приём данных в виде объектов DataFrame из разных источников;</li>
 *   <li>Буферизация входящих сообщений в потокобезопасной очереди;</li>
 *   <li>Асинхронная проверка очереди сообщений в отдельном потоке {@link MessageChecker};</li>
 *   <li>Передача данных на HUD (head-up display) для отображения;</li>
 *   <li>Отправка широковещательных сообщений в систему Android;</li>
 *   <li>Переадресация данных в Flutter приложение через платформенный канал, если оно активно;</li>
 *   <li>Обновление статусов транспортных интерфейсов в менеджере соединений {@link ConnectionManager};</li>
 *   <li>Реализация обратного вызова через {@link TransportEventListener} для уведомления о новых данных.</li>
 * </ul>
 * </p>
 *
 * <p>Обеспечивает синхронизацию доступа к очереди сообщений,
 * а также корректную работу с потоками для избежания блокировок и потери данных.</p>
 */
public class Receiver {

    /** Потокобезопасная очередь для хранения принятых сообщений DataFrame */
    private final List<DataFrame> receivedMessageQueue;

    /** Поток проверки и обработки очереди сообщений */
    private MessageChecker messageCheckerThread;

    /** Контекст приложения */
    private final Context mContext;

    /** Менеджер соединений, для обновления состояния интерфейсов */
    private final ConnectionManager connectionManager;

    /** Отображение HUD-информации */
    private final HudDisplay hudDisplay;

    /** Слушатель событий транспорта для передачи данных дальше */
    private TransportEventListener transportEventListener;

    /**
     * Конструктор Receiver
     * @param context контекст приложения
     * @param connectionManager менеджер соединений для обновления статусов
     * @param hudDisplay интерфейс HUD для отображения данных
     */
    public Receiver(Context context, ConnectionManager connectionManager, HudDisplay hudDisplay){
        mContext = context;
        this.connectionManager = connectionManager;
        this.hudDisplay = hudDisplay;
        receivedMessageQueue =  Collections.synchronizedList(new ArrayList<>());
    }

    /** Устанавливает слушатель событий приёма данных */
    public void setListener(TransportEventListener listener) {
        this.transportEventListener = listener;
    }

    /** Константа для широковещательного намерения о новых данных */
    public static final String NEW_DADA_DETECTED = "com.example.dixom_bs.rx_data";

    /** Интент для рассылки данных другим компонентам системы */
    Intent intent = new Intent(NEW_DADA_DETECTED);

    /**
     * Запускает поток для проверки очереди сообщений,
     * если поток ещё не запущен.
     */
    public void launchMessageCheckerThread(){
        if (connectionManager.StatusMessageReaderThread == Const.WorkState.DISABLE) {
            connectionManager.StatusMessageReaderThread = Const.WorkState.ENABLE;
            messageCheckerThread = new MessageChecker();
            messageCheckerThread.start();
        }
    }

    /** Останавливает поток проверки очереди сообщений */
    public void stopMessageCheckerThread(){
        if (messageCheckerThread != null) {
            messageCheckerThread.stopThis();
            connectionManager.StatusMessageReaderThread = Const.WorkState.DISABLE;
        }
    }

    /**
     * Добавляет новое сообщение DataFrame в очередь для последующей обработки
     * @param dataFrame принятое сообщение
     */
    public void addMessageToQueue(DataFrame dataFrame) {
        receivedMessageQueue.add(dataFrame);
    }

    /**
     * Внутренний поток для проверки и обработки очереди сообщений
     */
    private class MessageChecker extends Thread {
        private volatile boolean isStopped;
        DataFrame dataFrame;

        @Override
        public void run() {
            while (!isStopped) {
                try {
                    if (receivedMessageQueue.isEmpty()) {
                        // Очередь пуста — спим 10 мс, чтобы не нагружать CPU
                        MessageChecker.sleep(10);
                    } else {
                        // Берём первое сообщение из очереди
                        dataFrame = receivedMessageQueue.get(0);
                        // Удаляем обработанное сообщение из очереди
                        receivedMessageQueue.remove(0);
                        sendMessageBackClass(dataFrame);
                    }
                } catch (Exception e) {
                    // При ошибке прерываем поток и меняем статус
                    isStopped = false;
                    connectionManager.StatusMessageReaderThread = Const.WorkState.DISABLE;
                }
            }
        }

        /** Останавливает поток */
        public void stopThis() {
            isStopped = true;
        }
    }

    /**
     * Обрабатывает сообщение из очереди, распределяя данные по соответствующим каналам:
     * - Отображение на HUD
     * - Отправка широковещательного сообщения в систему
     * - Отправка данных в Flutter приложение, если оно активно
     *
     * @param dataFrame принятое сообщение
     */
    private void sendMessageBackClass(DataFrame dataFrame){
        // Отображение данных на HUD (запланировано доработать)
        hudDisplay.setHudInfo(dataFrame);

        // В зависимости от интерфейса кладём данные в соответствующий интент
        if(dataFrame.getTransportInterface() == Const.Transprort.USB_CDC){
            intent.putExtra("cdc", dataFrame.getBufferArray());
        } else if(dataFrame.getTransportInterface() == Const.Transprort.BLUETOOTH_SPP){
            intent.putExtra("bt", dataFrame.getBufferArray());
        } else if(dataFrame.getTransportInterface() == Const.Transprort.USB_HID){
            intent.putExtra("hid", dataFrame.getBufferArray());
        }

        // Отправка широковещательного сообщения в систему Android
        mContext.sendBroadcast(intent);

        // Если Flutter-приложение активно, отправляем данные через платформенный канал
        if(FlutterServiceBridge.StatusFlutterApp != Const.AppState.DETACHED){
            Message message = handler.obtainMessage();
            message.obj = dataFrame;
            handler.sendMessage(message);
        }
    }

    /**
     * Обработчик сообщений для передачи данных из фона в главный поток,
     * где происходит обновление статусов и вызов callback
     */
    Handler handler = new Handler(Looper.getMainLooper()) {
        @Override
        public void handleMessage(Message message) {
            DataFrame dataFrame = (DataFrame) message.obj;

            // Обновляем статус приёма в соответствующем транспортном интерфейсе
            if(dataFrame.getTransportInterface() == Const.Transprort.USB_HID){
                connectionManager.UsbHid.transportStatus.update(Const.TransferStatus.SUCCESS_IN);
            } else if(dataFrame.getTransportInterface() == Const.Transprort.USB_CDC){
                connectionManager.UsbCdc.transportStatus.update(Const.TransferStatus.SUCCESS_IN);
            } else if(dataFrame.getTransportInterface() == Const.Transprort.BLUETOOTH_SPP){
                connectionManager.BluetoothSpp.transportStatus.update(Const.TransferStatus.SUCCESS_IN);
            }

            // Уведомляем слушателя о поступлении новых данных
            notifyChanged(dataFrame);
        }
    };

    /**
     * Вызывает callback слушателя для уведомления о поступлении данных
     * @param dataFrame принятое сообщение
     */
    private void notifyChanged(DataFrame dataFrame) {
        if (transportEventListener != null) {
            transportEventListener.onDataReceived(dataFrame);
        }
    }
}
