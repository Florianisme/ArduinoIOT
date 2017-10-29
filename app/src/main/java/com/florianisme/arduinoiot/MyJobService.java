package com.florianisme.arduinoiot;

import android.app.job.JobParameters;
import android.app.job.JobService;
import android.content.Intent;

/**
 * Created by Florian Möhle on 29.10.2017
 */

public class MyJobService extends JobService {
        private static final String TAG = "SyncService";

        @Override
        public boolean onStartJob(JobParameters params) {
            Intent service = new Intent(getApplicationContext(), UpdateDataService.class);
            getApplicationContext().startService(service);
            Util.scheduleJob(getApplicationContext()); // reschedule the job
            return true;
        }

        @Override
        public boolean onStopJob(JobParameters params) {
            return true;
        }

    }
