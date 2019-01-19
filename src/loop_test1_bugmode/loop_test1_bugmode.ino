// Working version presented at BRE on Saturday, January 19 2019.

//We always have to include the library
#include "LedControl.h"

// Set pins and number of devices
// Arguments: LedControl(dataPin,clockPin,csPin,numDevices)
LedControl lc = LedControl(10, 12, 11, 3);

// Addresses of individual leds in bar graph
const int barGreen[36][3] = {{1, 0, 2}, {1, 0, 4}, {1, 0, 6}, {1, 0, 0}, {1, 1, 2}, {1, 1, 4}, {1, 1, 6}, {1, 1, 0}, {1, 2, 2}, {1, 2, 4}, {1, 2, 6}, {1, 2, 0}, {0, 0, 2}, {0, 0, 4}, {0, 0, 6}, {0, 0, 0}, {0, 1, 2}, {0, 1, 4}, {0, 1, 6}, {0, 1, 0}, {0, 2, 2}, {0, 2, 4}, {0, 2, 6}, {0, 2, 0}, {0, 3, 2}, {0, 3, 4}, {0, 3, 6}, {0, 3, 0}, {0, 4, 2}, {0, 4, 4}, {0, 4, 6}, {0, 4, 0}, {0, 5, 2}, {0, 5, 4}, {0, 5, 6}, {0, 5, 0}};
const int barRed[36][3] = {{1, 0, 1}, {1, 0, 3}, {1, 0, 5}, {1, 0, 7}, {1, 1, 1}, {1, 1, 3}, {1, 1, 5}, {1, 1, 7}, {1, 2, 1}, {1, 2, 3}, {1, 2, 5}, {1, 2, 7}, {0, 0, 1}, {0, 0, 3}, {0, 0, 5}, {0, 0, 7}, {0, 1, 1}, {0, 1, 3}, {0, 1, 5}, {0, 1, 7}, {0, 2, 1}, {0, 2, 3}, {0, 2, 5}, {0, 2, 7}, {0, 3, 1}, {0, 3, 3}, {0, 3, 5}, {0, 3, 7}, {0, 4, 1}, {0, 4, 3}, {0, 4, 5}, {0, 4, 7}, {0, 5, 1}, {0, 5, 3}, {0, 5, 5}, {0, 5, 7}};

// Limit for 2 minute each state: 485000
const int timerLimit = 8000;
int kettleState = 0;

// 2 minutes = 120000
int interval = 120000;

unsigned long previousMillis = 0; // will store last time LED was updated

/* Karma Modifiers table:
gridstate: 
(Reference: statekey = LEDrange)
1 = Low = 1-8
2 = Mid = 9-18
3 = High = 19-26
storagestate:
1 = Low = 1-3
2 = Mid = 4-7
3 = High = 8-10

sample output:
kettleState: 2, 24, 1, 4, 8, -2
kettleState: 3, 4, 9, -4, -8, 8
kettleState: 4, 4, 1, -4, -4, 8
kettleState: 1, 21, 9, 4, 4, -8
kettleState: 2, 23, 2, 4, 8, -2
kettleState: 3, 6, 9, -4, -8, 8
kettleState: 4, 4, 2, -4, -4, 8
kettleState: 1, 20, 9, 4, 4, -8

*/
// {gridRangeMin, gridRangeMax, storeRangeMin, storeRangeMax, boilmodifier, storemodifier, givemodifier}
const int kMods[4][7] = {{19, 26, 8, 10, 4, 4, -8},
                         {19, 26, 2, 3, 4, 8, -2},
                         {4, 8, 8, 10, -4, -8, 8},
                         {4, 8, 2, 3, -4, -4, 8}};

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
int selBoil = 0;
int selStore = 0;
int selGive = 0;
int nrgcost = 2;
int kDelta = 0;

bool boil = false;

bool bugMode = false;

//variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;
unsigned long last_button_time = 0;

void setup()
{
  Serial.begin(9600);
  //delay(1000);
  //Serial.println("Kettle controller booting up!");

  // Initialize pins
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(solenoidPin, OUTPUT);
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(usrBtnPin, INPUT);
  pinMode(staBtnPin, INPUT);

  attachInterrupt(staBtnPin, bugTrigger, RISING);

  /*
For some reason the assigned Solenoidpin goes high when the board is powered up,
this will make the kettle boil water. This step will press the kettle button again to turn it off:
*/
  digitalWrite(solenoidPin, LOW);
  delay(500);
  digitalWrite(solenoidPin, HIGH);
  delay(500);
  digitalWrite(solenoidPin, LOW);

  // Initialize display drivers
  int devices = lc.getDeviceCount();
  for (int address = 0; address < devices; address++)
  {
    /*The MAX72XX is in power-saving mode on startup*/
    lc.shutdown(address, false);
    /* Set the brightness to a medium values */
    lc.setIntensity(address, 0);
    /* and clear the display */
    lc.clearDisplay(address);
  }
}

void loop()
{

  int devices = lc.getDeviceCount();
  boil = false;

  unsigned long currentMillis = millis();

  // Get state for grid and storage and modifier values
  grid = random(kMods[kettleState][0], kMods[kettleState][1]);
  stor1 = random(kMods[kettleState][2], kMods[kettleState][3]);
  selBoil = kMods[kettleState][4];
  selStore = kMods[kettleState][5];
  selGive = kMods[kettleState][6];

  // Display starting grid and storage values
  displayBar(grid, stor1);
  delay(5);

  // Display selector values
  printSelNum(selBoil, 0);
  printSelNum(selStore, 1);
  printSelNum(selGive, 2);
  delay(5);

  // Display karma value
  int kd1 = karma;
  printKarmaNum(kd1);
  delay(5);

  fadeIn(15);
  delay(100);

  // Randomly turn kettle boiler on if bugMode is ON
  //randomBoil();

  // Wait for user to choose option and press Button
  // Set karma delta accordingly
  while (digitalRead(usrBtnPin) == LOW)
  {
    if (bugMode)
    {
      if (currentMillis - previousMillis >= interval)
      {
        previousMillis = currentMillis;
        boilNow();
      }
    }

    if (digitalRead(S1) == HIGH)
    {
      // Boil
      boil = true;
      kDelta = selBoil;
      nrgcost = 2;
    }
    if (digitalRead(S2) == HIGH)
    {
      // Store
      boil = false;
      kDelta = selStore;
      nrgcost = 0;
    }
    if (digitalRead(S3) == HIGH)
    {
      // Give
      boil = false;
      kDelta = selGive;
      nrgcost = 2;
    }
  }

  // Once button is pressed bugMode will be on
  bugMode = true;

  // Activate boiler if that was the selected option
  if (boil)
  {
    boilNow();
  }

  // Now we change the value of stor according to nrgcost of boiling
  stor2 = stor1 - nrgcost;

  // Now we blink the amount that will be modified
  modBlink(stor1 - stor2);

  // Animated transition to new storage value
  modBar(stor1, stor2);

  delay(500);
  karma = modKarma(karma, kDelta);

  delay(1000);

  fadeOut(15);

  // Clear display
  for (int address = 0; address < devices; address++)
  {
    lc.clearDisplay(address);
  }
  delay(1000);

  // Reset timer so it does not overflow
  if (currentMillis > timerLimit)
  {
    currentMillis = 0;
  }
}

// FUNCTIONS
void boilNow()
{
  digitalWrite(solenoidPin, HIGH);
  delay(300);
  digitalWrite(solenoidPin, LOW);
  boil = false;
}

// Modify bar display after user action
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
      delay(30);
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
    //delay(delaytime);
  }

  int lim = g + s;
  // display storage value
  for (int pos = g; pos < lim; pos++)
  {
    lc.setLed(barGreen[pos][0], barGreen[pos][1], barGreen[pos][2], true);
    //delay(delaytime);
  }
}

// Function to display selector number
// Arguments: printSelNum(display value, selection position)
void printSelNum(int v, int p)
{
  int ones;
  int onesDigit;
  int tensDigit;
  int address;
  boolean negative = false;

  if (p == 0)
  {
    address = 1;
    tensDigit = 3;
    onesDigit = 4;
  }
  if (p == 1)
  {
    address = 1;
    tensDigit = 5;
    onesDigit = 6;
  }
  if (p == 2)
  {
    address = 2;
    tensDigit = 4;
    onesDigit = 5;
  }

  if (v < -9 || v > 9)
    return;
  if (v < 0)
  {
    negative = true;
    v = v * -1;
  }
  ones = v % 10;
  v = v / 10;

  if (negative)
  {
    //print character '-' in the leftmost column
    lc.setChar(address, tensDigit, '-', false);
  }
  else
  {
    //print a blank in the sign column
    lc.setChar(address, tensDigit, ' ', false);
  }
  //Now print the number digit by digit
  //lc.setDigit(address, tensDigit, (byte)tens, false);
  lc.setDigit(address, onesDigit, (byte)ones, false);
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

// Fade in
// fadeIn(delayBetweenupdates)
void fadeIn(int d)
{
  int devices = lc.getDeviceCount();
  for (int intensity = 0; intensity <= 15; intensity++)
  {
    //Serial.println("intensity");
    for (int address = 0; address < devices; address++)
    {
      // Iterate over rows(Digits)
      lc.setIntensity(address, intensity);
      delay(d);
      //delay(delaytime);
    }
  }
}

// Fade out
// fadeOut(delayBetweenupdates)
void fadeOut(int d)
{
  int devices = lc.getDeviceCount();
  for (int intensity = 15; intensity > 0; intensity--)
  {
    //Serial.println("intensity");
    for (int address = 0; address < devices; address++)
    {
      // Iterate over rows(Digits)
      lc.setIntensity(address, intensity);
      delay(d);
      //delay(delaytime);
    }
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
      delay(150);
    }
  }
  else
  {
    for (int countK = k1; countK <= k2; countK++)
    {
      printKarmaNum(countK);
      delay(150);
    }
  }
  return k2;
}

/*
void bugTrigger()
{
  button_time = millis();
  if (button_time - last_button_time > 250)
  {
    if (bugMode == false)
    {
      bugMode = true;
      lc.setLed(2, 0, 0, true);
    }
    else
    {
      bugMode = false;
      lc.setLed(2, 0, 0, false);
    }
    last_button_time = button_time;
  }
}
*/

void randomBoil()
{
  if (bugMode)
  {
    int r = random(1, 4);
    if (r == 3)
    {
      boilNow();
    }
  }
}