/*
 * This class contains all the helper methods used throughout this project
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
