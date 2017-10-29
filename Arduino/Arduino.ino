/*
 * Arduino IOT
 * 
 * This project connects to an online database service (Firebase) and regularly pushes readings from various sensors such as temperature and brightness.
 * The data from the database is then read by an Android application and shown to the user.
 * 
 * The circuit:
 * * brightness sensor to A0
 * 
 * Created 29 October 2017
 * By Florian Moehle
 */

// external libraries, not created by me
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// sensitive keys/passwords have been extracted to another file so they are not directly visible
#include "config.h"

// static sensor IO
#define SENSOR_LIGHT A0

void setup() {
  Serial.begin(9600);
  
  instantiateWifiConnection();

  // connect to firebase database
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  String brightness = readBrightnessLevel();
  Firebase.setString("brightness", brightness); // brightness value is stored in the database with the key "brightness"
  
  //delay(1000 * 60 * 30); // refresh every 30 minutes
}

/*
 * Reads the brightness sensor and returns a conclusion of the sensor values in textual form
 */
String readBrightnessLevel() {
  int photocellReading = analogRead(SENSOR_LIGHT);
  debugReadings("Brightness", photocellReading);
  
  if (photocellReading < 100) {
    return "Dark";
  } else if (photocellReading < 300) {
    return "Dim";
  } else if (photocellReading < 500) {
    return "Mildly illuminated";
  } else if (photocellReading < 800) {
    return "Brightly illuminated";
  } else {
    return "Very brightly illuminated";
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


