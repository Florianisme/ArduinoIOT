package com.florianisme.arduinoiot;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.TextView;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import butterknife.BindView;
import butterknife.ButterKnife;

/**
 * Created by Florian Möhle on 31.10.2017
 */

public class MainActivity extends AppCompatActivity {

    private final String TAG = "MainActivity";

    @BindView(R.id.plant_brightness)
    TextView plantBrightness;
    @BindView(R.id.plant_water_level)
    TextView plantWaterLevel;
    @BindView(R.id.room_temperature)
    TextView roomTemparature;
    @BindView(R.id.room_humidity)
    TextView roomHumidity;

    FirebaseDatabase firebaseDatabase;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main); // set's the current layout
        ButterKnife.bind(this); // Binds views from our layout to usable Objects

        firebaseDatabase = FirebaseDatabase.getInstance(); // initialization of our Database reference, the url can be found in the google-service.json file

        // this adds a listener to the specified child in the database and runs an asynchronous task in the background,
        // downloading the data and putting it into a DataSnapshot object
        firebaseDatabase.getReference().child("brightness").addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                // Task succeeded
                Log.d(TAG, "Task succeeded");
                plantBrightness.setText((String) dataSnapshot.getValue());
            }

            @Override
            public void onCancelled(DatabaseError databaseError) {
                // Task failed
                Log.d(TAG, "Task failed with error message: " + databaseError.getMessage());
            }
        });
    }
}
