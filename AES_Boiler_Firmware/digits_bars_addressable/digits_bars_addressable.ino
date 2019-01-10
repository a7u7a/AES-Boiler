// First attempt at abstracting 7seg and bar displays

//We always have to include the library
#include "LedControl.h"

 // Set pins and number of devices
 // Arguments: LedControl(dataPin,clockPin,csPin,numDevices)
LedControl lc = LedControl(10, 12, 11, 3);

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
unsigned long delaytime = 10;


void setup() {
  Serial.begin(9600);
  Serial.println("Kettle controller booting up!");

  // Initialize pins
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(solenoidPin, OUTPUT);
  digitalWrite(solenoidPin, LOW);
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(usrBtnPin, INPUT);
  pinMode(staBtnPin, INPUT);

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
  int devices=lc.getDeviceCount();
  

  // Iterate over devices
  for(int address=0;address<devices;address++) {
    // Iterate over rows
    for(int row=0;row<8;row++) {
      // Iterate over columns
      for(int col=0;col<8;col++) {
        lc.setLed(address,row,col,true);
        delay(delaytime);
      }
    }
}

// Activate solenoid for a second
digitalWrite(solenoidPin, HIGH);
delay(1000);
digitalWrite(solenoidPin, LOW);

// Clear display
for(int address=0;address<devices;address++) {
    lc.clearDisplay(address);
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
