/*
  Sketch for testing time limits of grid and storage state values
*/

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0; // will store last time LED was updated

// constants won't change:
const long interval = 1000; // interval at which to blink (milliseconds)
// 485000
const int timerLimit = 8000;

int kettleState = 0;

// set initial values
int grid = 0;
int stor1 = 0;
int selBoil = 0;
int selStore = 0;
int selGive = 0;

bool bugMode = false;

const int staBtnPin = 20; // State change button. Pin also labeled 'SDA' on the Feather

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
                         {19, 26, 1, 3, 4, 8, -2},
                         {1, 8, 8, 10, -4, -8, 8},
                         {1, 8, 1, 3, -4, -4, 8}};

void setup()
{
  Serial.begin(9600);
  attachInterrupt(staBtnPin, bugTrigger, RISING);
}

void loop()
{
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the difference
  // between the current time and last time you blinked the LED is bigger than
  // the interval at which you want to blink the LED.
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval)
  {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    kettleState++;
    Serial.print("kettleState: ");
    Serial.print(kettleState);
    Serial.print(", ");
    Serial.print(grid);
    Serial.print(", ");
    Serial.print(stor1);
    Serial.print(", ");
    Serial.print(selBoil);
    Serial.print(", ");
    Serial.print(selStore);
    Serial.print(", ");
    Serial.println(selGive);

    if (kettleState >= 4)
    {
      kettleState = 0;
    }

    if(bugMode)
    {
      Serial.println("lalala");
    }
  }

  grid = random(kMods[kettleState][0], kMods[kettleState][1]);
  stor1 = random(kMods[kettleState][2], kMods[kettleState][3]);
  selBoil = kMods[kettleState][4];
  selStore = kMods[kettleState][5];
  selGive = kMods[kettleState][6];

  // Reset timer so it does not overflow
  if (currentMillis > timerLimit)
  {
    currentMillis = 0;
  }
}

void bugTrigger()
{
  if (bugMode == false)
  {
    bugMode = true;
  }
  else
  {
    bugMode = false;
  }
}


// Add software debounce
// light up a dot to show