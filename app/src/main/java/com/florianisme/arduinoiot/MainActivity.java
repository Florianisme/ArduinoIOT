package com.florianisme.arduinoiot;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.widget.SwipeRefreshLayout;
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
    TextView roomTemperature;
    @BindView(R.id.room_humidity)
    TextView roomHumidity;

    @BindView(R.id.main_swipe_refresh)
    SwipeRefreshLayout swipeRefreshLayout;

    FirebaseDatabase firebaseDatabase;

    /*
        called when the Activity is started, instantiates objects, binds views and starts the database logic
     */
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main); // set's the current layout
        ButterKnife.bind(this); // Binds views from our layout to usable Objects
        setTitle(R.string.app_title);

        firebaseDatabase = FirebaseDatabase.getInstance(); // initialization of our Database reference, the url can be found in the google-service.json file

        reloadData();

        // this adds a swipe-to-refresh listener to our layout so we can swipe down to refresh our data from the database manually
        swipeRefreshLayout.setOnRefreshListener(new SwipeRefreshLayout.OnRefreshListener() {
            @Override
            public void onRefresh() {
                reloadData();
            }
        });
    }

    /*
        used to reload data from the database, calls the downloading method for each TextView in our layout
     */
    private void reloadData() {
        setDataFromDatabase("plant_brightness", plantBrightness);
        setDataFromDatabase("plant_water_level", plantWaterLevel);
        setDataFromDatabase("room_temperature", roomTemperature);
        setDataFromDatabase("room_humidity", roomHumidity);
    }

    /*
        Reads data from the specified childName from the database and sets it as the textField's value
     */
    private void setDataFromDatabase(final String childName, final TextView textField) {
        // this adds a listener to the specified child in the database and runs an asynchronous task in the background,
        // downloading the data and putting it into a DataSnapshot object
        firebaseDatabase.getReference().child(childName).addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                // Task succeeded
                Log.d(TAG, "Task succeeded for field name " + childName);
                if (dataSnapshot.exists())
                    textField.setText((String) dataSnapshot.getValue());
                else
                    textField.setText(R.string.error_no_value);

                swipeRefreshLayout.setRefreshing(false); // stops the spinning animation
            }

            @Override
            public void onCancelled(DatabaseError databaseError) {
                // Task failed
                Log.d(TAG, "Task failed for field name " + childName + " with error message: " + databaseError.getMessage());
                textField.setText(R.string.error_database);
                swipeRefreshLayout.setRefreshing(false); // stops the spinning animation
            }
        });
    }
}
