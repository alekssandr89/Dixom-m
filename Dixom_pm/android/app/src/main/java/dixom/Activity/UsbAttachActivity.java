package dixom.Activity;

import android.app.PendingIntent;
import android.content.Intent;

import ru.dixom.pm.MainActivity;
import io.flutter.embedding.android.FlutterActivity;

public class UsbAttachActivity extends FlutterActivity {
    @Override
    protected void onResume()
    {
        super.onResume();
        Intent notificationStartIntent = new Intent(this, MainActivity.class);
        PendingIntent pendingIntent = PendingIntent.getActivity(this, 0, notificationStartIntent, PendingIntent.FLAG_IMMUTABLE);
        Intent serviceIntent = new Intent(this, dixom.Service.DixomService.class);
        serviceIntent.putExtra("pendingIntent", pendingIntent);
        startService(serviceIntent);
        finish();
    }
}