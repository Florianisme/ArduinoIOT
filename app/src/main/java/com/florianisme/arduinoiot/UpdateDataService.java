package com.florianisme.arduinoiot;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.IBinder;
import android.support.annotation.Nullable;

/**
 * Created by Florian Möhle on 29.10.2017
 */

class UpdateDataService extends Service {

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {

        Notification.Builder builder = new Notification.Builder(getApplicationContext());
        builder.setContentTitle("This is the title");
        builder.setContentText("This is the text");
        builder.setSubText("Some sub text");
        builder.setNumber(101);
        builder.setTicker("Fancy Notification");
        builder.setOngoing(true);
        Notification notification = builder.build();
        NotificationManager notificationManger =
                (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
        notificationManger.notify(01, notification);


        return Service.START_NOT_STICKY;
    }

}
