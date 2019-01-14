//We always have to include the library
#include "LedControl.h"

 // Set pins and number of devices
 // Arguments: LedControl(dataPin,clockPin,csPin,numDevices)
LedControl lc = LedControl(10, 12, 11, 2);

// Rotary switch pins
const int S1 = 6;
const int S2 = 5;
const int S3 = 21; // Also labeled 'SCL' on the Feather

// Button pins
int usrBtnPin = 9; // User button
int staBtnPin = 20; // State change button. Pin also labeled 'SDA' on the Feather

int usrBtnState = 0;
int staBtnState = 0;

// Solenoid pin
int solenoidPin = A0;

int wait = 50;
unsigned long delaytime = 500;

void setup() {
  Serial.begin(9600);
  Serial.println("Kettle controller booting up!");

  // Initialize pins
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(solenoidPin, OUTPUT);
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(usrBtnPin, INPUT);
  pinMode(staBtnPin, INPUT);
  digitalWrite(solenoidPin, LOW);

  // Initialize display drivers
  int devices = lc.getDeviceCount();
  for (int address = 0; address < devices; address++) {
    /*The MAX72XX is in power-saving mode on startup*/
    lc.shutdown(address, false);
    /* Set the brightness to a medium values */
    lc.setIntensity(address, 15);
    /* and clear the display */
    lc.clearDisplay(address);
  }
}

void loop() {

  buttonState = digitalRead(S1);
  //read the number cascaded devices
  int devices = lc.getDeviceCount();

  for (int i = 0; i < 9; i++) {
    if (buttonState == HIGH) {
      printNumber(i);
      lc.clearDisplay(wait);
      Serial.println("hola");
      delay(wait);
    } else {
       Serial.println("low");
      lc.setLed(0, 0, i, true);
      delay(wait);
      lc.setLed(0, 0, i, false);
      delay(wait);
    }
  }
}

  void printNumber(int v) {
    int ones;
    int tens;
    int hundreds;

    boolean negative = false;

    if (v < -999 || v > 999)
      return;
    if (v < 0) {
      negative = true;
      v = v * -1;
    }
    ones = v % 10;
    v = v / 10;
    tens = v % 10;
    v = v / 10; hundreds = v;
    if (negative) {
      //print character '-' in the leftmost column
      lc.setChar(0, 3, '-', false);
    }
    else {
      //print a blank in the sign column
      lc.setChar(0, 3, ' ', false);
    }
    //Now print the number digit by digit
    lc.setDigit(1, 0, (byte)hundreds, false);
    lc.setDigit(1, 0, (byte)tens, false);
    lc.setDigit(1, 0, (byte)ones, false);
  }
