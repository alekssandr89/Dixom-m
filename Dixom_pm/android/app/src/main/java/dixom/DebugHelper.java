package dixom;

import android.content.Context;
import android.util.Log;
import android.widget.Toast;

public class DebugHelper {

    private static Context appContext;

    public static void init(Context context) {
        appContext = context.getApplicationContext();
    }

    public static void showToast(String message) {
        if (appContext != null) {
            Toast.makeText(appContext, message, Toast.LENGTH_SHORT).show();
        }
    }

    public static void showLog(String tag, String message) {
        Log.d(tag, message); // стандартный Android лог
    }

}
