package com.florianisme.arduinoiot;

/**
 * Created by Florian Möhle on 29.10.2017
 */

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class MyStartServiceReceiver extends BroadcastReceiver {

    @Override
    public void onReceive(Context context, Intent intent) {
        Util.scheduleJob(context);
    }
}