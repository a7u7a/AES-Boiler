// Bar display animation function back and forth
// The idea is to make a single function for initial values and later for when grid and storage values have been modified. keeping the animation style.
// todo: create function that takes a value and a color and displays the value in the specified color. Similar to printNum function

//We always have to include the library
#include "LedControl.h"

// Set pins and number of devices
// Arguments: LedControl(dataPin,clockPin,csPin,numDevices)
LedControl lc = LedControl(10, 12, 11, 3);

// Addresses of individual leds in bar graph
const int barGreen[36][3] = {{1, 0, 2}, {1, 0, 4}, {1, 0, 6}, {1, 0, 0}, {1, 1, 2}, {1, 1, 4}, {1, 1, 6}, {1, 1, 0}, {1, 2, 2}, {1, 2, 4}, {1, 2, 6}, {1, 2, 0}, {0, 0, 2}, {0, 0, 4}, {0, 0, 6}, {0, 0, 0}, {0, 1, 2}, {0, 1, 4}, {0, 1, 6}, {0, 1, 0}, {0, 2, 2}, {0, 2, 4}, {0, 2, 6}, {0, 2, 0}, {0, 3, 2}, {0, 3, 4}, {0, 3, 6}, {0, 3, 0}, {0, 4, 2}, {0, 4, 4}, {0, 4, 6}, {0, 4, 0}, {0, 5, 2}, {0, 5, 4}, {0, 5, 6}, {0, 5, 0}};
const int barRed[36][3] = {{1, 0, 1}, {1, 0, 3}, {1, 0, 5}, {1, 0, 7}, {1, 1, 1}, {1, 1, 3}, {1, 1, 5}, {1, 1, 7}, {1, 2, 1}, {1, 2, 3}, {1, 2, 5}, {1, 2, 7}, {0, 0, 1}, {0, 0, 3}, {0, 0, 5}, {0, 0, 7}, {0, 1, 1}, {0, 1, 3}, {0, 1, 5}, {0, 1, 7}, {0, 2, 1}, {0, 2, 3}, {0, 2, 5}, {0, 2, 7}, {0, 3, 1}, {0, 3, 3}, {0, 3, 5}, {0, 3, 7}, {0, 4, 1}, {0, 4, 3}, {0, 4, 5}, {0, 4, 7}, {0, 5, 1}, {0, 5, 3}, {0, 5, 5}, {0, 5, 7}};

// Rotary switch pins
const int S1 = 6;
const int S2 = 5;
const int S3 = 21; // Also labeled 'SCL' on the Feather

// Button pins
const int usrBtnPin = 9;  // User button
const int staBtnPin = 20; // State change button. Pin also labeled 'SDA' on the Feather

int usrBtnState = 0;
int staBtnState = 0;

// Solenoid pin
int solenoidPin = A0;

int wait = 50;

unsigned long delaytime = 15;

// set initial values
int grid = 0;
int stor1 = 0;
int stor2 = 0;
int karma = 10;

void setup()
{
  Serial.begin(9600);
  //delay(1000);
  //Serial.println("Kettle controller booting up!");

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
  // Start with random values
  grid = random(3, 26);
  stor1 = random(5, 10);
  // Display starting grid and storage values
  displayBar(grid, stor1);
  delay(500);
  // Now we change the value of stor
  stor2 = stor1 - random(2, 5);
  // Now we blink the amount that will be modified
  modBlink(stor1 - stor2);

  modBar(stor1, stor2);
  delay(500);
  // Clear display
  int devices = lc.getDeviceCount();
  for (int address = 0; address < devices; address++)
  {
    lc.clearDisplay(address);
  }
}

/*
 delay(500);
// Clear display
  int devices = lc.getDeviceCount();
for(int address=0;address<devices;address++) {
    lc.clearDisplay(address);
  }
 
}
*/

// modBar()
void modBar(int s1, int s2)
{
  int lim = grid + stor1;
  for (int pos = lim; pos >= (lim - (s1 - s2)); pos--)
  {
    lc.setLed(barGreen[pos][0], barGreen[pos][1], barGreen[pos][2], false);
    delay(delaytime * 5);
  }
}

// modBlink(storageModifier)
void modBlink(int sm)
{
  // blink 4 times
  for (int i = 0; i < 8; i++)
  {
    {
      for (int pos = stor1 + grid - sm; pos < stor1 + grid; pos++)
      {
        lc.setLed(barGreen[pos][0], barGreen[pos][1], barGreen[pos][2], false);
      }
      delay(40);
      for (int pos = stor1 + grid - sm; pos < stor1 + grid; pos++)
      {
        lc.setLed(barGreen[pos][0], barGreen[pos][1], barGreen[pos][2], true);
      }
      delay(80);
    }
  }
}

// displayBar(grid value, store value, color)
void displayBar(int g, int s)
{
  // display grid value
  for (int pos = 0; pos < g; pos++)
  {
    lc.setLed(barRed[pos][0], barRed[pos][1], barRed[pos][2], true);
    delay(delaytime);
  }

  int lim = g + s;
  // display storage value
  for (int pos = g; pos < lim; pos++)
  {
    lc.setLed(barGreen[pos][0], barGreen[pos][1], barGreen[pos][2], true);
    delay(delaytime);
  }
}