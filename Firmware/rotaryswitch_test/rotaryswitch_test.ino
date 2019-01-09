const int S1 = 6;
const int S2 = 5;
const int S3 = 21; // Pin 21 is also labeled 'SCL' on the Feather
const int BTN = 9;

bool S1s;
bool S2s;
bool S3s;
bool BTNs;

void setup() {
  Serial.begin(9600);
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(BTN, INPUT);
}

void loop() {

  S1s = digitalRead(S1);
  S2s = digitalRead(S2);
  S3s = digitalRead(S3);
  BTNs = digitalRead(BTN);
  Serial.print("S1: ");
  Serial.print(S1s);
  Serial.print(" S2: ");
  Serial.print(S2s);
  Serial.print(" S3: ");
  Serial.print(S3s);
  Serial.print(" BTN: ");
  Serial.println(BTNs);
  delay(300);

  // if (digitalRead(S1) == HIGH){
  //  Serial.println("S1");
  //  delay(20);
  // }
  //
  //if (digitalRead(S2) == HIGH){
  //  Serial.println("S2");
  //  delay(20);
  // }
  //
  // if (digitalRead(S3) == HIGH){
  //  Serial.println("S3");
  //  delay(20);
  // }
}
