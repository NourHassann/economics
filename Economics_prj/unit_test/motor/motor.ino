/*
L293D Pin
IN1	Arduino pin 8
IN2	Arduino pin 9
EN1	(PWM capable)
OUT1	Motor wire 1
OUT2	Motor wire 2
VSS	5V 
VS	Motor power  6V–12V)
GND	Common ground 
*/
const int in1 = 8;
const int in2 = 9;
const int enA = 10;

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enA, OUTPUT);
}

void loop() {
  // Move Forward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, 200);  // Adjust speed (0-255)
  delay(2000);

  // Stop briefly
  analogWrite(enA, 0);
  delay(500);

  // Move Backward
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, 200);  // Same speed
  delay(2000);

  // Stop briefly
  analogWrite(enA, 0);
  delay(500);
}
