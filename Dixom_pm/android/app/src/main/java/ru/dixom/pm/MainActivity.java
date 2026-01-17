package ru.dixom.pm;

import android.app.PendingIntent;
import android.content.Intent;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;

import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;

import io.flutter.embedding.android.FlutterActivity;
import io.flutter.embedding.engine.FlutterEngine;
import io.flutter.embedding.engine.FlutterEngineCache;

public class MainActivity extends FlutterActivity {

    final String ACTION_FLUTTER_APP_STATE = "ru.dixom.service.flutter_app_state";
    final String NAME_APP_STATE = "state";
    final String FLUTTER_ENGINE_ID = "dixom_engine_id";

    @RequiresApi(api = Build.VERSION_CODES.M)
    @Override
    public void configureFlutterEngine(@NonNull FlutterEngine flutterEngine) {
        super.configureFlutterEngine(flutterEngine);

        // Сохраняем ссылку на FlutterEngine в FlutterEngineCache
        FlutterEngineCache.getInstance().put(FLUTTER_ENGINE_ID, flutterEngine);

        // Запускаем сервис, если он не запущен
        //startService(new Intent(this, DixomService.class));

        // Извлекаем PendingIntent из Intent
        // Создаем Intent для запуска сервиса
        Intent notificationStartIntent = new Intent(this, MainActivity.class);
        PendingIntent pendingIntent = PendingIntent.getActivity(this, 0, notificationStartIntent, PendingIntent.FLAG_IMMUTABLE);

        Intent serviceIntent = new Intent(this, dixom.Service.DixomService.class);
        serviceIntent.putExtra("pendingIntent", pendingIntent);

        startService(serviceIntent);

        // Уведомляем сервис что приложение запущено и можно обмениваться данными через плафторменный канал
        //На случай если сервис был не запущен чуть задержим отправку информации что приложение запущено
        delayedMethod(10);

    }


    public void delayedMethod(long delayMillis) {
        new Handler(Looper.getMainLooper()).postDelayed(new Runnable() {
            @Override
            public void run() {
                Intent intent = new Intent(ACTION_FLUTTER_APP_STATE);
                intent.putExtra(NAME_APP_STATE, "app start");
                sendBroadcast(intent);
            }
        }, delayMillis);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        // Уведомляем сервис что приложение закрыто и нужно прекратить обмен данными через платформенный канал
        Intent intent = new Intent(ACTION_FLUTTER_APP_STATE);
        intent.putExtra(NAME_APP_STATE, "app close");
        sendBroadcast(intent);
    }

}
