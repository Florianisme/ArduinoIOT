/*
 * This class contains all the methods for reading various sensors and interpreting their data
 *
 * Created 29 October 2017
 * By Florian Moehle
 */

#include "Util.h" // contains all the helper methods
#include <dht11.h> // Temperature Sensor library

dht11 DHT11;

 /*
 * Reads the brightness sensor and returns a conclusion of the sensor values in textual form
 */
float readPlantBrightnessLevel() {
  int photocellReading = readAnalogValueFromMuxPin(0);
  float percentage = (photocellReading / 1024.0) * 100;
  debugReadings("Plant Brightness", percentage);
  return percentage;
}

float readRoomTemperature() {
  int roomTemperature = DHT11.read(D3);
  debugReadings("Room Temperature (°C)", (float) DHT11.temperature);
  return DHT11.temperature;
}

float readRoomHumidity() {
  int roomHumidity = DHT11.read(D3);
  debugReadings("Room Humidity (%)", (float) DHT11.humidity);
  return DHT11.humidity;
}

float readPlantWaterLevel() {
  int analogReading = readAnalogValueFromMuxPin(1);
  float percentage = (analogReading / 1024.0) * 100;
  debugReadings("Water Level (%)", (float) percentage);
  return percentage;
}

