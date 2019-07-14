/*
   This class contains all the methods for reading various sensors and interpreting their data

   Created 29 October 2017
   By Florian Moehle
*/

dht11 DHT11;

/*
   reads the temperature/humidity and stores it
   if we read the values from the sensor, each time we actually want to retrieve them, measuring errors would occur
   (eg. humidity of 160% because the sensor has no time to settle)
*/
void readTemperatureSensor() {
  DHT11.read(DHT11_INPUT);
}

float readRoomTemperature() {
  debugReadings("Room Temperature (°C)", DHT11.temperature);
  return DHT11.temperature;
}

float readRoomHumidity() {
  debugReadings("Room Humidity (%)", DHT11.humidity);
  return DHT11.humidity;
}

void setWaterLevels() {
  pinMode(WATER_OUTPUT, OUTPUT); // our program only consists of a setup method so we can call it here without issues
  digitalWrite(WATER_OUTPUT, HIGH);
  for (int i = 0; i < 4; i++) {
    float analogReading = readAnalogValueFromMuxPin(PLANT_INPUTS[i]);
    float percentage = calculatePercentage(analogReading);
    debugReadings("Water Level (%)", percentage);
    Firebase.setFloat(PLANT_FIREBASE_PATHS[i] + "/water_level", percentage);
    debugReadings("Water Level (%)", percentage);
  }
  digitalWrite(WATER_OUTPUT, LOW);
}
