/*
 * Arduino IOT
 * 
 * This project connects to an online database service (Firebase) and regularly pushes readings from various sensors such as temperature, brightness, humidity and water level of a plant.
 * All sensors had to be connected to a Multiplexer as the ESP-12E only has a single analog input connection 
 * The data from the database is then read by an Android application and shown to the user.
 * 
 * The circuit:
 * * Multiplexer connected to pins D0-D2
 * * Multiplexer's analog output connected to pin A0
 * 
 * Created 29 October 2017
 * By Florian Moehle
 */

// external libraries, not created by me
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#include "config.h" // sensitive keys/passwords have been extracted to another file so they are not directly visible
#include "CMOS_4051.h" // contains code for accessing the multiplexer's input pins and passing throguh their values

void setup() {
  Serial.begin(9600);

  instantiateMux(); // sets up the Mux's IO connections so we can read from it
  instantiateWifiConnection(); // connects to WiFi network specified in the config.h
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);// connect to firebase database
}

void loop() {
  String brightness = readPlantBrightnessLevel();
  Firebase.setString("plant_brightness", brightness); // brightness value is stored in the database with the key "plant_brightness"
  
  //delay(1000 * 60 * 30); // refresh every 30 minutes
}

/*
 * Reads the brightness sensor and returns a conclusion of the sensor values in textual form
 */
String readPlantBrightnessLevel() {
  int photocellReading = readAnalogValueFromMuxPin(0);
  debugReadings("Plant Brightness", photocellReading);
  
  if (photocellReading < 100) {
    return "Sehr Dunkel";
  } else if (photocellReading < 300) {
    return "Dunkel";
  } else if (photocellReading < 500) {
    return "mittelmäßig Hell";
  } else if (photocellReading < 800) {
    return "Hell";
  } else {
    return "Sehr Hell";
  }
}

/*
 * Recurring method, prints out the name of the sensor and it's value which was measured
 */
void debugReadings(String sensorName, int sensorValue) {
  Serial.print("Reading" + sensorName + ": ");
  Serial.println(sensorValue);
  Serial.println("------------------------");
}

/*
 * run once on startup to connect to the WIFI_SSID specified in the config.h file
 */
void instantiateWifiConnection() {
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}


