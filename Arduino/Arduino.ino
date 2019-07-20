/*
 * Arduino IOT Demo
 * 
 * This project connects to an online database service (Firebase) and regularly pushes readings from various sensors such as temperature, brightness, humidity and water level of a plant.
 * Almost all sensors had to be connected to a Multiplexer as the ESP-12E only has a single analog input connection 
 * The data from the database is then read by an Android application and shown to the user.
 * 
 * The circuit:
 * * Multiplexer's (CMOS4051) select pins connected to pins D2-D4
 * * Multiplexer's analog output connected to pin A0
 * * Temperature/Humidity sensor (DHT11) connected to pin D1;
 * * Jumper wire from RST to D0 (for deep sleep, HAS to be disconnected during boot/upload))
 * 
 * Created 29 October 2017
 * By Florian Moehle
 */

// external libraries, not created by me
#include <ESP8266WiFi.h> // WiFi connection library
#include <FirebaseArduino.h> // Firebase database library
#include <dht11.h> // Temperature Sensor library

// Board GPIO PINS
#define MUX_A D4
#define MUX_B D3
#define MUX_C D2
#define WATER_OUTPUT D5 // used to read the plant's water level and turned off to prevent corosion
#define ANALOG_INPUT A0 // our only analog input which is connected to the multiplexer's output
#define DHT11_INPUT D1 // serial connection to our temperature sensor
// Multiplexer analog inputs
const int PLANT_INPUTS[] = {0, 1, 2, 3}; // multiplexer input index for the water level
const String PLANT_FIREBASE_PATHS[] = {"plant_1", "plant_2", "plant_3", "plant_4"};

#include "Secrets.h" // sensitive keys/passwords have been extracted to another file so they are not directly visible
// "Multiplexer.ino" contains code for accessing the multiplexer's input pins and passing through their values
// "SensorReadings.ino" contains all the methods for reading each sensor's value and interpreting them for upload
// "Util.ino" contains all the helper methods and our pin values (debugging, etc.)

void setup() {
  Serial.begin(115200); // A serial monitor can always be attached to track raw sensor readings
  
  instantiateMux(); // sets up the Mux's I/O connections so we can read from it
  instantiateWifiConnection(); // connects to WiFi network specified in the config.h

  if (WiFi.status() == WL_CONNECTED) {
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); // connects to firebase database
    updateData(); // update data and upload
  }

  Serial.println("Entering deep sleep");
  ESP.deepSleep(3.6e9); // deep-sleep for 60 minutes, then restart the setup method
}

/*
 * read sensor values with the helper methods from SensorReadings.ino and upload the to the database
 */
void updateData() {
  readTemperatureSensor(); // Has to be called seperately, the library reads the temperature/humidity once and stores it internally so we can later use it, without calling the read function again
  const float roomTemperature = readRoomTemperature();
  const float roomHumidity = readRoomHumidity();

  setWaterLevels();
  Firebase.setFloat("room_temperature", roomTemperature);
  Firebase.setFloat("room_humidity", roomHumidity);
}

void loop() {
  // No code here as the ESP runs the code specified in the setup method when exiting deep-sleep mode
}
