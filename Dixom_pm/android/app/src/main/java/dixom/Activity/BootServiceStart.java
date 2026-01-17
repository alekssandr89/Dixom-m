package dixom.Activity;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;


public class BootServiceStart extends BroadcastReceiver {

    @Override
    public void onReceive(Context context, Intent intent) {
        Intent i = new Intent(context, UsbAttachActivity.class);  //MyActivity can be anything which you want to start on bootup...
        i.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        context.startActivity(i);
    }
}