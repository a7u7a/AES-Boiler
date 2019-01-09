//We always have to include the library
#include "LedControl.h"

/*
  Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
  pin 12 is connected to the DataIn
  pin 11 is connected to the CLK
  pin 10 is connected to LOAD
 ***** Please set the number of devices you have *****
  But the maximum default of 8 MAX72XX wil also work.
*/

 // Set pins and number of devices
LedControl lc = LedControl(10, 12, 11, 2);

/* we always wait a bit between updates of the display */
unsigned long delaytime = 500;

/*
  This time we have more than one device.
  But all of them have to be initialized
  individually.
*/

int wait = 50;
// Rotary switch pins
const int S1 = 6;
const int S2 = 5;
const int S3 = 21; // Also labeled 'SCL' on the Feather

int buttonState = 0;         // variable for reading the pushbutton status
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buttonPin, INPUT);
  //we have already set the number of devices when we created the LedControl
  int devices = lc.getDeviceCount();
  //we have to init all devices in a loop
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

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(500);

  
  buttonState = digitalRead(S1);
  //read the number cascaded devices
  int devices = lc.getDeviceCount();

  for (int i = 0; i < 9; i++) {
    if (buttonState == HIGH) {
      printNumber(i);
      lc.clearDisplay(wait);
      Serial.println("hi");
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
