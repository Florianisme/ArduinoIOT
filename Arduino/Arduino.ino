/*
 * Arduino IOT
 * 
 * This project connects to an online database service (Firebase) and regularly pushes readings from various sensors such as temperature, brightness, humidity and water level of a plant.
 * All sensors had to be connected to a Multiplexer as the ESP-12E only has a single analog input connection 
 * The data from the database is then read by an Android application and shown to the user.
 * 
 * The circuit:
 * * Multiplexer (CMOS4051) select pins connected to pins D0-D2
 * * Multiplexer's analog output connected to pin A0
 * * Temperature/Humidity sensor (DHT11) connected to pin D3;
 * 
 * Created 29 October 2017
 * By Florian Moehle
 */

// external libraries, not created by me
#include <ESP8266WiFi.h> // WiFi connetction library
#include <FirebaseArduino.h> // Firebase library
#include <dht11.h> // Temperature Sensor library

#include "Secrets.h" // sensitive keys/passwords have been extracted to another file so they are not directly visible
#include "CMOS_4051.h" // contains code for accessing the multiplexer's input pins and passing through their values
#include "SensorValues.h" // contains all the methods for reading each sensor's value and interpreting them for upload
// "Util.h" // contains all the helper methods for debugging

void setup() {
  Serial.begin(9600); // A serial monitor can always be attached to track raw sensor readings

  instantiateMux(); // sets up the Mux's IO connections so we can read from it
  instantiateWifiConnection(); // connects to WiFi network specified in the config.h
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);// connects to firebase database
}

void loop() {
  Serial.println("Refreshing readings");
  Serial.println();
  
  float plantBrightness = readPlantBrightnessLevel();
  float plantWaterLevel = readPlantWaterLevel();
  float roomTemperature = readRoomTemperature();
  float roomHumidity = readRoomHumidity();
  
  Firebase.setFloat("plant_brightness", plantBrightness); // brightness value is stored in the database with the key "plant_brightness"
  Firebase.setFloat("plant_water_level", plantWaterLevel);
  Firebase.setFloat("room_temperature", roomTemperature);
  Firebase.setFloat("room_humidity", roomHumidity);

  delay(1000 * 30);
  //delay(1000 * 60 * 30); // refresh every 30 minutes
}


