package com.florianisme.arduinoiot;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.util.Map;

import butterknife.BindView;
import butterknife.ButterKnife;

/**
 * Created by Florian Möhle on 31.10.2017
 */

public class MainActivity extends AppCompatActivity {

    private final String TAG = "MainActivity";

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
        ButterKnife.bind(this); // Binds views from our layout to usable Java objects
        setTitle(R.string.app_title);

        firebaseDatabase = FirebaseDatabase.getInstance(); // initialization of our Database reference, the url can be found in the google-service.json file
        firebaseDatabase.setPersistenceEnabled(true);

        // initial call to download fresh data
        reloadData();

        // this adds a swipe-to-refresh listener to our layout so we can swipe down to refresh our data from the database manually
        swipeRefreshLayout.setOnRefreshListener(this::reloadData);
    }

    /*
        used to reload data from the database, calls the downloading method for each TextView in our layout
     */
    private void reloadData() {
        setWaterLevels();
        setDataFromDatabase("room_temperature", roomTemperature);
        setDataFromDatabase("room_humidity", roomHumidity);
    }

    private void setWaterLevels() {
        Plant[] plants = createPlantObjects();
        for (Plant plant : plants) {
            DatabaseReference plantReference = firebaseDatabase.getReference().child(plant.getFirebaseId());
            plantReference.addListenerForSingleValueEvent(new ValueEventListener() {
                @Override
                public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                    setPlantViewData((Map<String, Object>)dataSnapshot.getValue(), plant);
                }

                @Override
                public void onCancelled(@NonNull DatabaseError databaseError) {

                }
            });
        }
    }

    private void setPlantViewData(Map<String, Object> childs, Plant plant) {
        View plantLayout = findViewById(plant.getLayoutId());
        TextView plantName = plantLayout.findViewById(R.id.plant_name);
        TextView plantWaterLevel = plantLayout.findViewById(R.id.plant_water_level);
        TextView plantWaterLevelWarning = plantLayout.findViewById(R.id.plant_water_level_warning);

        plantName.setText(String.valueOf(childs.get("name")));
        Object waterLevel = childs.get("water_level");
        plantWaterLevel.setText(waterLevel + "%");
        plantWaterLevelWarning.setVisibility(getWarningLevelVisibility(Double.valueOf(String.valueOf(waterLevel))));
    }

    private int getWarningLevelVisibility(Double waterLevel) {
        return waterLevel > 30D && !Double.isNaN(waterLevel) ? View.INVISIBLE : View.VISIBLE;
    }

    private Plant[] createPlantObjects() {
        return new Plant[]{
                new Plant(R.id.plant1, "plant_1"),
                new Plant(R.id.plant2, "plant_2"),
                new Plant(R.id.plant3, "plant_3"),
                new Plant(R.id.plant4, "plant_4")
        };
    }

    /*
        Reads data from the specified childName from the database and sets it as the textField's value
     */
    private void setDataFromDatabase(final String childName, final TextView textView) {
        // this adds a listener to the specified child in the database and runs an asynchronous task in the background,
        // downloading the data and putting it into a DataSnapshot object
        firebaseDatabase.getReference().child(childName).addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                // Task succeeded
                Log.d(TAG, "Task succeeded for field name " + childName);
                if (dataSnapshot.exists())
                    textView.setText(interpretMeasurementData(childName, Double.valueOf(String.valueOf(dataSnapshot.getValue()))));
                else
                    textView.setText(R.string.error_no_value);

                swipeRefreshLayout.setRefreshing(false); // stops the spinning animation
            }

            @Override
            public void onCancelled(DatabaseError databaseError) {
                // Task failed
                Log.d(TAG, "Task failed for field name " + childName + " with error message: " + databaseError.getMessage());
                textView.setText(R.string.error_database);
                swipeRefreshLayout.setRefreshing(false); // stops the spinning animation
            }
        });
    }

    String interpretMeasurementData(String childName, double value) {
        switch (childName) {
            case "room_temperature":
                return (value + " °C");
            case "room_humidity":
                return (Math.round(value) + "%");
        }
        return "Error";
    }
}
