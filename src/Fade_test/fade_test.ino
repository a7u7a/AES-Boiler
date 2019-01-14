// Simple fade test
// Will light up all LEDs and then enter into a linear fade in and out loop 

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
  delay(1000);
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
  
  // Iterate over MAX devices
  for(int address=0;address<devices;address++) {
    lc.setIntensity(address, 15);
    // Iterate over rows(Digits)
    for(int row=0;row<8;row++) {
      // Iterate over columns(Segments)
      // Light up individual LEDs
      for(int col=0;col<8;col++) {
        lc.setLed(address,row,col,true);
        delay(delaytime);
      }
    }
}

// Fade loop
while(1){
for (int intensity=15; intensity>0;intensity--){
  //Serial.println("intensity");
  for(int address=0;address<devices;address++) {
    // Iterate over rows(Digits)
        lc.setIntensity(address, intensity);
        delay(6);
        //delay(delaytime);
      }
    }
  //delay(1);


for (int intensity=0; intensity<=15;intensity++){
  //Serial.println("intensity");
  for(int address=0;address<devices;address++) {
    // Iterate over rows(Digits)
        lc.setIntensity(address, intensity);
        delay(12);
        //delay(delaytime);
      }
    }
delay(200);
}

// Clear display
for(int address=0;address<devices;address++) {
    lc.clearDisplay(address);
  }
}