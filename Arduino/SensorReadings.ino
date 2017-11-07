/*
 * This class contains all the methods for reading various sensors and interpreting their data
 *
 * Created 29 October 2017
 * By Florian Moehle
 */

dht11 DHT11;

 /*
 * Reads the brightness sensor and returns a conclusion of the sensor values in textual form
 */
float readPlantBrightnessLevel() {
  int photocellReading = readAnalogValueFromMuxPin(BRIGHTNESS_INPUT);
  float percentage = calculatePercentage(photocellReading);
  debugReadings("Plant Brightness", percentage);
  return percentage;
}

float readRoomTemperature() {
  int roomTemperature = DHT11.read(DHT11_INPUT);
  debugReadings("Room Temperature (°C)", (float) DHT11.temperature);
  return DHT11.temperature;
}

float readRoomHumidity() {
  int roomHumidity = DHT11.read(DHT11_INPUT);
  debugReadings("Room Humidity (%)", (float) DHT11.humidity);
  return DHT11.humidity;
}

float readPlantWaterLevel() {
  int analogReading = readAnalogValueFromMuxPin(WATER_INPUT);
  float percentage = calculatePercentage(analogReading);
  debugReadings("Water Level (%)", (float) percentage);
  return percentage;
}

