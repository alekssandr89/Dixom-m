package dixom.Service;

import static android.content.Context.WINDOW_SERVICE;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.PixelFormat;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.provider.Settings;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.WindowManager;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.RequiresApi;

import java.nio.charset.StandardCharsets;
import java.util.Objects;

import dixom.Config;
import dixom.Const;
import dixom.Service.Connection.DataFrame;
import ru.dixom.pm.R;

/**
 * HudDisplay — класс отображения HUD (overlay) уведомлений поверх всех приложений.
 *
 * <p>
 * Используется сервисом для вывода краткой информации (например, уровня громкости),
 * поступающей от внешнего устройства, без необходимости открытия UI приложения.
 * </p>
 *
 * <p>
 * HUD отображается только при выполнении условий:
 * <ul>
 *     <li>Разрешено overlay-окно (SYSTEM_ALERT_WINDOW)</li>
 *     <li>Включена настройка HUD в {@link Config}</li>
 *     <li>Приложение Flutter не находится в активном состоянии</li>
 * </ul>
 * </p>
 *
 * <p>
 * Реализована автоматическая логика скрытия HUD по таймауту
 * с использованием отдельного управляющего потока.
 * </p>
 *
 * <p>
 * Поддерживается Android API 26+ (TYPE_APPLICATION_OVERLAY).
 * </p>
 */
public class HudDisplay {

    /** Флаг наличия разрешения на overlay */
    private boolean permissionOverlay = false;

    /** TextView для отображения текстовой информации */
    private TextView Volume;

    /** ProgressBar для отображения уровня значения (например, громкости) */
    private ProgressBar VolumeLvl;

    /** Основной layout HUD */
    private RelativeLayout topView;

    /** WindowManager для управления overlay-окном */
    private WindowManager windowManager;

    /** Параметры overlay-окна */
    WindowManager.LayoutParams topParams;

    /** Размеры экрана */
    public static int height, width;

    /** Таймаут отображения HUD (мс) */
    private int serviceHudTimeout = 5000;

    /** Счётчик времени отображения */
    private int serviceHudCounter = 0;

    /** Поток управления HUD */
    private serviceHud serviceHudThread;

    /** Состояние потока HUD */
    private boolean serviceHudThreadState = false;

    /** Контекст сервиса */
    Context mContext;

    /**
     * Конструктор HUD.
     *
     * <p>
     * Выполняет первичную проверку разрешений и инициализацию параметров overlay.
     * </p>
     *
     * @param context контекст сервиса
     */
    public HudDisplay(Context context) {
        mContext = context;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            setupHud();
        }
    }

    /**
     * Отображение тестового сообщения HUD.
     *
     * @param message текст сообщения
     * @return статус выполнения
     */
    public Const.WorkState hudShowTestMessage(String message) {
        Const.WorkState state = setupHud();

        if (state == Const.WorkState.OK) {
            Volume.post(() -> Volume.setText(message));

            if (!serviceHudThreadState) {
                serviceHudTimeout = 1000;
                new Handler(Looper.getMainLooper()).post(() ->
                        windowManager.addView(topView, topParams)
                );
                startServiceHudThread();
            } else {
                serviceHudCounter = 0;
            }
            return Const.WorkState.OK;

        } else if (state == Const.WorkState.NO_PERMISSION) {
            Toast.makeText(mContext, "Нет разрешения", Toast.LENGTH_SHORT).show();
            return Const.WorkState.NO_PERMISSION;

        } else if (state == Const.WorkState.NOT_SUPPORTED) {
            Toast.makeText(mContext, "Не поддерживается", Toast.LENGTH_SHORT).show();
            return Const.WorkState.NOT_SUPPORTED;
        }

        Toast.makeText(mContext, "Ошибка инициализации HUD", Toast.LENGTH_SHORT).show();
        return Const.WorkState.ERROR;
    }

    /**
     * Обработка входящих данных и обновление HUD.
     *
     * <p>
     * Анализирует {@link DataFrame} и отображает данные,
     * если они соответствуют протоколу HUD.
     * </p>
     *
     * @param dataFrame входящий пакет данных
     */
    public void setHudInfo(DataFrame dataFrame) {

        if (Config.serviceSettings[Config.SettingKey.HUD_ENABLED.ordinal()]
                != Const.WorkState.ENABLE.ordinal()) {
            return;
        }

        if (dataFrame.getBuffer().get(0) == '4'
                && dataFrame.getBuffer().get(2) == '3'
                && dataFrame.getBuffer().get(4) == '0') {

            String str = new String(dataFrame.getBuffer().array(), StandardCharsets.UTF_8);
            String[] parts = str.split(" ");

            Volume.post(() -> Volume.setText("Громкость " + parts[3]));

            int value;
            try {
                value = Integer.parseInt(parts[3]);
            } catch (NumberFormatException e) {
                value = 0;
            }

            VolumeLvl.setProgress(value);
            showHud(1000);
        }
    }

    /**
     * Отображение HUD на заданный таймаут.
     *
     * @param timeOutShow время отображения в миллисекундах
     */
    public void showHud(int timeOutShow) {

        if (!serviceHudThreadState) {
            if (permissionOverlay
                    && FlutterServiceBridge.StatusFlutterApp != Const.AppState.RESUME) {

                serviceHudTimeout = timeOutShow;

                new Handler(Looper.getMainLooper()).post(() ->
                        windowManager.addView(topView, topParams)
                );
                startServiceHudThread();
            }
        } else {
            serviceHudCounter = 0;
        }
    }

    /** Закрытие HUD */
    public void Close_Hud() {
        if (topView != null) {
            windowManager.removeView(topView);
        }
    }

    /**
     * Проверка разрешений и инициализация HUD.
     *
     * @return статус операции
     */
    public Const.WorkState setupHud() {

        if (!Settings.canDrawOverlays(mContext)) {
            permissionOverlay = false;
            serviceHudThreadState = false;
            return Const.WorkState.NO_PERMISSION;
        }

        if (permissionOverlay) {
            return Const.WorkState.OK;
        }

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            return overlayParam();
        }

        return Const.WorkState.NOT_SUPPORTED;
    }

    /**
     * Настройка параметров overlay-окна.
     *
     * @return статус операции
     */
    @RequiresApi(api = Build.VERSION_CODES.O)
    public Const.WorkState overlayParam() {
        try {
            permissionOverlay = true;
            windowManager = (WindowManager) mContext.getSystemService(WINDOW_SERVICE);
            Screen_Info();

            topParams = new WindowManager.LayoutParams(
                    width,
                    height,
                    WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY,
                    WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
                    PixelFormat.TRANSLUCENT
            );

            topParams.gravity = Gravity.CENTER;
            topView = (RelativeLayout) LayoutInflater.from(mContext)
                    .inflate(R.layout.layout_overlay, null);

            VolumeLvl = topView.findViewById(R.id.volumeProgress);
            Volume = topView.findViewById(R.id.VolumeText);

            return Const.WorkState.OK;

        } catch (Exception e) {
            return Const.WorkState.ERROR;
        }
    }

    /**
     * Получение параметров экрана.
     */
    private void Screen_Info() {
        DisplayMetrics metrics = mContext.getResources().getDisplayMetrics();
        width = metrics.widthPixels;
        height = metrics.heightPixels;
    }

    /**
     * Внутренний поток управления временем отображения HUD.
     */
    private class serviceHud extends Thread {

        @Override
        public void run() {
            try {
                while (serviceHudThreadState) {
                    sleep(500);
                    serviceHudCounter += 500;

                    if (serviceHudCounter > serviceHudTimeout) {
                        serviceHudThreadState = false;
                    }
                }
                serviceHudTimeout = 2000;
                serviceHudCounter = 0;
                Close_Hud();
            } catch (Exception e) {
                serviceHudThreadState = false;
            }
        }

        public void stopThread() {
            serviceHudThreadState = false;
        }
    }

    /** Запуск потока HUD */
    private void startServiceHudThread() {
        if (!serviceHudThreadState) {
            serviceHudThreadState = true;
            serviceHudThread = new serviceHud();
            serviceHudThread.start();
        }
    }

    /** Остановка потока HUD */
    private void stopServiceHudThread() {
        if (serviceHudThread != null) {
            serviceHudThread.stopThread();
        }
    }
}