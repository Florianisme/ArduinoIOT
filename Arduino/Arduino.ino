/*
 * Arduino IOT Demo
 * 
 * This project connects to an online database service (Firebase) and regularly pushes readings from various sensors such as temperature, brightness, humidity and water level of a plant.
 * Almost all sensors had to be connected to a Multiplexer as the ESP-12E only has a single analog input connection 
 * The data from the database is then read by an Android application and shown to the user.
 * 
 * The circuit:
 * * Multiplexer's (CMOS4051) select pins connected to pins D0-D2
 * * Multiplexer's analog output connected to pin A0
 * * Temperature/Humidity sensor (DHT11) connected to pin D3;
 * 
 * Created 29 October 2017
 * By Florian Moehle
 */

// external libraries, not created by me
#include <ESP8266WiFi.h> // WiFi connection library
#include <FirebaseArduino.h> // Firebase database library
#include <dht11.h> // Temperature Sensor library

#include "Secrets.h" // sensitive keys/passwords have been extracted to another file so they are not directly visible
// "Multiplexer.ino" contains code for accessing the multiplexer's input pins and passing through their values
// "SensorReadings.ino" contains all the methods for reading each sensor's value and interpreting them for upload
// "Util.ino" contains all the helper methods (debugging, etc.)

void setup() {
  Serial.begin(9600); // A serial monitor can always be attached to track raw sensor readings

  instantiateMux(); // sets up the Mux's I/O connections so we can read from it
  instantiateWifiConnection(); // connects to WiFi network specified in the config.h
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);// connects to firebase database

  updateData();

  ESP.deepSleep(30e6); // deep-sleep for 30 seconds, then restart the setup method
}

void updateData() {
  const float plantBrightness = readPlantBrightnessLevel(); // read sensor values with the helper methods from SensorReadings.ino and store them in constants
  const float plantWaterLevel = readPlantWaterLevel();
  const float roomTemperature = readRoomTemperature();
  const float roomHumidity = readRoomHumidity();
  
  Firebase.setFloat("plant_brightness", plantBrightness); // brightness value is stored in the database with the key "plant_brightness"
  Firebase.setFloat("plant_water_level", plantWaterLevel);
  Firebase.setFloat("room_temperature", roomTemperature);
  Firebase.setFloat("room_humidity", roomHumidity);
}

void loop() {
  // No code here as the ESP runs the code specified in the setup method when exiting deep-sleep mode
}


