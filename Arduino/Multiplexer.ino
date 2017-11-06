/*
 * This class contains all the methods used for reading the sensor's values
 *
 * Created 29 October 2017
 * By Florian Moehle
 */

#define MUX_A D0
#define MUX_B D1
#define MUX_C D2

#define ANALOG_INPUT A0

/*
 * called in setup method
 */
void instantiateMux() {
  pinMode(MUX_A, OUTPUT);
  pinMode(MUX_B, OUTPUT);     
  pinMode(MUX_C, OUTPUT); 
}

/*
 * called each time the mux's input source is switched
 */
void changeMux(int c, int b, int a) {
  digitalWrite(MUX_C, c);
  digitalWrite(MUX_B, b);
  digitalWrite(MUX_A, a);
}

/*
 * Switches inputs of the mux, based on the pin we want to read from (passthrough)
 * ANALOG_INPUT is then connnected through the multiplexer to the according multiplexer input
 */
int readAnalogValueFromMuxPin(int pin) {
  switch (pin) {
    case 0:
      changeMux(LOW, LOW, LOW);
      break;
    case 1:
      changeMux(LOW, LOW, HIGH);
      break;
    case 2:
      changeMux(LOW, HIGH, LOW);
      break;
    case 3:
      changeMux(LOW, HIGH, HIGH);
      break;
    case 4:
      changeMux(HIGH, LOW, LOW);
      break;
    case 5:
      changeMux(HIGH, LOW, HIGH);
      break;
    case 6:
      changeMux(HIGH, HIGH, LOW);
      break;
    case 7:
      changeMux(HIGH, HIGH, HIGH);
      break;
    default:
      changeMux(LOW, LOW, LOW);
      break;
  }
  
  return analogRead(ANALOG_INPUT); // Value of the sensor connected to the specified pin
}



