int solenoidPin = A0;

void setup() {
 Serial.begin(9600);
 pinMode(solenoidPin, OUTPUT);
 digitalWrite(solenoidPin, LOW);

}

void loop() {
  digitalWrite(solenoidPin, LOW);
  delay(500);
  digitalWrite(solenoidPin, HIGH);
  delay(500);

}
