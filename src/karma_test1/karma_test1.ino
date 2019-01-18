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
int usrBtnPin = 9;  // User button
int staBtnPin = 20; // State change button. Pin also labeled 'SDA' on the Feather

int usrBtnState = 0;
int staBtnState = 0;

// Solenoid pin
int solenoidPin = A0;

int wait = 50;
unsigned long delaytime = 30;
int karma = 10;
int kDelta = 0;

void setup()
{
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
  for (int address = 0; address < devices; address++)
  {
    /*The MAX72XX is in power-saving mode on startup*/
    lc.shutdown(address, false);
    /* Set the brightness to a medium values */
    lc.setIntensity(address, 15);
    /* and clear the display */
    lc.clearDisplay(address);
  }
}

void loop()
{
  //Starting karma points
  printKarmaNum(karma);
  // Amount to modifiy karma points with
  kDelta = random(-999,999);
  karma = modKarma(karma, kDelta);

  delay(1000);
  // Clear display
  int devices = lc.getDeviceCount();
  for (int address = 0; address < devices; address++)
  {
    lc.clearDisplay(address);
  }

}

int modKarma(int k1, int kd)
{
  
  int k2 = k1 + kd;
  // If kDelta is smaller than 0
  if (kd < 0)
  {
    for (int countK = k1; countK >= k2; countK--)
    {
      printKarmaNum(countK);
      delay(5);
    }
  }
  else
  {
    for (int countK = k1; countK <= k2; countK++)
    {
      printKarmaNum(countK);
      delay(5);
    }
  }
  return k2;
}

// Function to display Karma number
void printKarmaNum(int v)
{
  int ones;
  int tens;
  int hundreds;
  int thousands;

  boolean negative = false;

  if (v < -999 || v > 999)
    return;
  if (v < 0)
  {
    negative = true;
    v = v * -1;
  }
  ones = v % 10;
  v = v / 10;
  tens = v % 10;
  v = v / 10;
  hundreds = v;

  if (negative)
  {
    //print character '-' in the leftmost column
    lc.setChar(2, 0, '-', false);
  }
  else
  {
    //print a blank in the sign column
    lc.setChar(2, 0, ' ', false);
  }
  //Now print the number digit by digit
  lc.setDigit(2, 1, (byte)hundreds, false);
  lc.setDigit(2, 2, (byte)tens, false);
  lc.setDigit(2, 3, (byte)ones, false);
}