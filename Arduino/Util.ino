/*
 * This class contains all the helper methods used throughout this project
 *
 * Created 29 October 2017
 * By Florian Moehle
 */

/*
 * Recurring method, prints out the name of the sensor and it's value which was measured
 */
void debugReadings(String sensorName, float sensorValue) {
  Serial.print("Reading" + sensorName + ": ");
  Serial.println(sensorValue);
  Serial.println("------------------------");
}

/*
 * returns the percentage of the value provided in comparison to the maximum analog input value 1023
 */
float calculatePercentage(float value) {
  return (value / 1023.0) * 100;
}

/*    
 * run once on startup to connect to the WIFI_SSID specified in the config.h file
 */
void instantiateWifiConnection() {
  byte retryCount = 0;
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting to SSID " + String(WIFI_SSID));
  while (WiFi.status() != WL_CONNECTED) {
    if (retryCount >= 10) {
      showConnectionErrorMessage();
      break;
    }
    Serial.print(".");
    delay(500);
    retryCount++; 
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void showConnectionErrorMessage() {
  Serial.println("An Error occured while connection to SSID " + String(WIFI_SSID));
}

