/*
 * This class contains all the methods for reading various sensors and interpreting their data
 */

#include "Util.h" // contains all the helper methods

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
