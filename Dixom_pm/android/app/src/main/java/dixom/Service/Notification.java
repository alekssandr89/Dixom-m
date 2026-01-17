package dixom.Service;

import static android.content.Context.NOTIFICATION_SERVICE;

import android.Manifest;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ServiceInfo;
import android.graphics.Color;
import android.os.Build;
import androidx.core.app.NotificationCompat;

import dixom.Activity.UsbAttachActivity;
import dixom.Config;
import dixom.Const;
import dixom.DebugHelper;
import dixom.Service.Connection.ConnectionManager;
import ru.dixom.pm.R;

/**
 * Notification — управляющий класс foreground-уведомления сервиса Dixom.
 *
 * <p>
 * Класс инкапсулирует всю логику работы с системными уведомлениями Android:
 * создание канала уведомлений, запуск и поддержание foreground-сервиса,
 * а также динамическое обновление уведомления в зависимости от состояния
 * подключения к устройству Dixom.
 * </p>
 *
 * <p>
 * Основные задачи класса:
 * <ul>
 *     <li>Запуск сервиса в foreground-режиме для предотвращения его остановки системой</li>
 *     <li>Создание {@link NotificationChannel} (Android 8.0+)</li>
 *     <li>Проверка разрешения POST_NOTIFICATIONS (Android 13+)</li>
 *     <li>Формирование уведомления на основе состояния {@link ConnectionManager}</li>
 *     <li>Обновление уведомления при изменении интерфейса или статуса соединения</li>
 * </ul>
 * </p>
 *
 * <p>
 * Уведомление отображает:
 * <ul>
 *     <li>Текущее состояние соединения (подключено / нет связи)</li>
 *     <li>Активный интерфейс обмена (USB HID, USB CDC, Bluetooth SPP)</li>
 * </ul>
 * </p>
 *
 * <p>
 * Класс не наследуется от {@link android.app.Notification}, несмотря на название,
 * а является сервисным helper-классом для управления foreground-уведомлением.
 * </p>
 *
 * <p>
 * Поведение уведомлений управляется настройками {@link Config}:
 * если уведомления отключены в настройках сервиса,
 * foreground-режим не запускается.
 * </p>
 *
 * <p>
 * Используется исключительно внутри сервисного слоя приложения.
 * </p>
 */
public class Notification {
    /**
     * Конструктор.
     * Сохраняет ссылки на Context и Service
     * и сразу пытается запустить foreground-сервис.
     */

    private final PendingIntent pendingIntent;
    private final Service mService;
    private final ConnectionManager connectionManager;

    public Notification(Service mService, ConnectionManager connectionManager) {
        this.mService = mService;
        this.connectionManager = connectionManager;
        pendingIntent = createPendingIntent();
        startDixomForegroundService();
    }

    private PendingIntent createPendingIntent() {
        Context context = mService;
        Intent intent = new Intent(context, UsbAttachActivity.class);
        int flags = PendingIntent.FLAG_IMMUTABLE;
        return PendingIntent.getActivity(context, 0, intent, flags);
    }

    /**
     * Создаёт NotificationChannel для foreground-сервиса.
     * Обязателен для Android 8.0+ (API 26+).
     * <p>
     * IMPORTANCE_HIGH — чтобы уведомление было всегда видно.
     */
    private void createNotificationChannel() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {

            NotificationChannel serviceChannel = new NotificationChannel(
                    Const.NOTIFICATION_CHANNEL_ID,
                    Const.FOREGROUND_CHANNEL,
                    NotificationManager.IMPORTANCE_HIGH
            );

            serviceChannel.setDescription("Foreground Service Notification Channel");
            serviceChannel.enableLights(true);
            serviceChannel.setLightColor(Color.BLUE);
            serviceChannel.enableVibration(true);

            NotificationManager manager =
                    mService.getSystemService(NotificationManager.class);

            if (manager != null) {
                manager.createNotificationChannel(serviceChannel);
            }
        }
    }

    /**
     * Проверяет разрешение POST_NOTIFICATIONS.
     * Начиная с Android 13 (API 33) уведомления требуют явного разрешения.
     */
    private boolean hasNotificationPermission() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
            return mService.checkSelfPermission(
                    Manifest.permission.POST_NOTIFICATIONS
            ) == PackageManager.PERMISSION_GRANTED;
        }
        return true;
    }

    /**
     * Запускает foreground-сервис.
     * <p>
     * Вызывается:
     * - при создании Notification
     * - при повторном включении уведомлений
     * <p>
     * ВАЖНО:
     * startForeground вызывается ТОЛЬКО здесь.
     */
    public void startDixomForegroundService() {

        // Проверка: включены ли уведомления в настройках приложения
        if (Config.serviceSettings[Config.SettingKey.NOTIFICATIONS_ENABLED.ordinal()]
                != Const.WorkState.ENABLE.ordinal()) {
            return;
        }

        createNotificationChannel();

        if (!hasNotificationPermission()) {
            DebugHelper.showToast("Разрешите уведомления для стабильной работы сервиса");
            return;
        }

        try {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.UPSIDE_DOWN_CAKE) {
                // Android 14+
                mService.startForeground(
                        Const.NOTIFICATION_ID,
                        buildForegroundNotification(),
                        ServiceInfo.FOREGROUND_SERVICE_TYPE_DATA_SYNC
                );
            } else {
                // Android 8–13
                mService.startForeground(
                        Const.NOTIFICATION_ID,
                        buildForegroundNotification()
                );
            }

        } catch (SecurityException e) {
            DebugHelper.showToast("Система запретила запуск фонового сервиса");
        } catch (Exception e) {
            DebugHelper.showToast("Ошибка запуска сервиса: " + e.getMessage());
        }
    }

    /**
     * Собирает и возвращает Notification для foreground-сервиса.
     * <p>
     * Используется:
     * - при старте сервиса
     * - при обновлении уведомления
     */
    private android.app.Notification buildForegroundNotification() {

        // Заголовок уведомления — состояние соединения
        String statusText = (connectionManager.StatusConnection == Const.WorkState.CONNECTED)
                ? "Подключение установлено"
                : "Нет связи с Dixom";

        // Текст уведомления — активный интерфейс
        String activeInterface =
                mService.getString(R.string.conectUsbDeviceForNoty);

        switch (connectionManager.CurrentExchangeInterface) {
            case USB_HID -> activeInterface = mService.getString(R.string.ActiveInterfaceUsbHid);
            case USB_CDC -> activeInterface = mService.getString(R.string.ActiveInterfaceUsbCdc);
            case BLUETOOTH_SPP ->
                    activeInterface = mService.getString(R.string.ActiveInterfaceUsbBluetoothSpp);
        }

        // Иконка и цвет зависят от состояния подключения
        int iconRes = (connectionManager.StatusConnection == Const.WorkState.CONNECTED)
                ? R.drawable.ic_connection_ok
                : R.drawable.ic_connection_bad;

        int color = (connectionManager.StatusConnection == Const.WorkState.CONNECTED)
                ? Color.GREEN
                : Color.BLUE;

        return new NotificationCompat.Builder(mService, Const.NOTIFICATION_CHANNEL_ID)
                .setContentTitle(statusText)
                .setContentText(activeInterface)
                .setSmallIcon(iconRes)
                .setColor(color)
                .setOngoing(true)                 // нельзя смахнуть
                .setOnlyAlertOnce(true)           // не спамить
                .setCategory(android.app.Notification.CATEGORY_SERVICE)
                .setForegroundServiceBehavior(
                        NotificationCompat.FOREGROUND_SERVICE_IMMEDIATE
                )
                .setContentIntent(pendingIntent)
                .build();
    }

    /**
     * Обновляет уже показанное foreground-уведомление.
     */
    public void updateForegroundNotification() {

        if (Config.serviceSettings[Config.SettingKey.NOTIFICATIONS_ENABLED.ordinal()]
                != Const.WorkState.ENABLE.ordinal()) {
            return;
        }

        NotificationManager manager =
                (NotificationManager) mService.getSystemService(NOTIFICATION_SERVICE);

        if (manager != null) {
            manager.notify(
                    Const.NOTIFICATION_ID,
                    buildForegroundNotification()
            );
        }
    }

    /**
     * Включает уведомления:
     * - запускает foreground-сервис
     * - обновляет уведомление
     */
    public void enableNotification() {
        startDixomForegroundService();
        updateForegroundNotification();
    }

    /**
     * Выключает foreground-уведомление и переводит сервис
     * из foreground-состояния.
     */
    public void disableNotification() {
        mService.stopForeground(true);
    }
}
