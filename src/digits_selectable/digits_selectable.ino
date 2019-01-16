// Select a position with the rotary switch and light up the corresponding digits in the display

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
unsigned long delaytime = 30;


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
  //int position;
/*
  if (digitalRead(S1) == HIGH){
    position = 0;
    Serial.println("Position 0");
    lc.clearDisplay(1);
    lc.clearDisplay(2);
    delay(1);
  }
  if (digitalRead(S2) == HIGH){
    position = 1;
    Serial.println("Position 1");
    lc.clearDisplay(1);
    lc.clearDisplay(2);
    delay(1);
  }
  if (digitalRead(S3) == HIGH){
    position = 2;
    Serial.println("Position 2");
    lc.clearDisplay(1);
    lc.clearDisplay(2);
    delay(1);
  }
  */
  int devices=lc.getDeviceCount();

for(int position = 0; position<=2;position++){
  for(int count = 0;count<10;count++){
  printSelNum(count, position);
  delay(delaytime*2);
  }
}

   // Print numbers on karma display counter
  /*for(int countK = 0;countK<999;countK++){
  printKarmaNum(countK);
  delay(delaytime);
  } 
  */
}

  // Function to display Karma number
  void printKarmaNum(int v) {
    int ones;
    int tens;
    int hundreds;
    int thousands;

    boolean negative = false;

    if (v < -9999 || v > 9999)
      return;
    if (v < 0) {
      negative = true;
      v = v * -1;
    }
    ones = v % 10;
    v = v / 10;
    tens = v % 10;
    v = v / 10; 
    hundreds = v % 10;
    v = v / 10;
    thousands = v;

    if (negative) {
      //print character '-' in the leftmost column
      lc.setChar(0, 3, '-', false);
    }
    else {
      //print a blank in the sign column
      lc.setChar(0, 3, ' ', false);
    }
    //Now print the number digit by digit
    lc.setDigit(2, 0, (byte)thousands, false);
    lc.setDigit(2, 1, (byte)hundreds, false);
    lc.setDigit(2, 2, (byte)tens, false);
    lc.setDigit(2, 3, (byte)ones, false);
  }


// Function to display selector number
// Arguments: printSelNum(display value, selection position)
  void printSelNum(int v, int p) {
    int ones;
    int onesDigit;
    int tensDigit;
    int address;

    boolean negative = false;

    if (p == 0){
    address = 1;
    tensDigit = 3;
    onesDigit = 4; 
    }
    if (p == 1){
    address = 1;
    tensDigit = 5;
    onesDigit = 6;

    }
    if(p == 2){
    address = 2;
    tensDigit = 4;
    onesDigit = 5;
    }

    if (v < -9 || v > 9)
      return;
    if (v < 0) {
      negative = true;
      v = v * -1;
    }
    ones = v % 10;
    v = v / 10;

    if (negative) {
      //print character '-' in the leftmost column
      lc.setChar(address, tensDigit, '-', false);
    }
    else {
      //print a blank in the sign column
      lc.setChar(address, tensDigit, ' ', false);
    }
    //Now print the number digit by digit
    //lc.setDigit(address, tensDigit, (byte)tens, false);
    lc.setDigit(address, onesDigit, (byte)ones, false);
  }
