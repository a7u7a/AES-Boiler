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

void setup()
{
  Serial.begin(9600);
  // set the digital pin as output:
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
  Serial.println(kettleState);
    if (kettleState >= 3)
    {
      kettleState = 0;
    }
  }

// Reset timer so it does not overflow
if (currentMillis>timerLimit)
{
  currentMillis = 0;
}

}
