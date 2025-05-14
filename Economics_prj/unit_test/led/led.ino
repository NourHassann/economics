int ledPin = 9;       // PWM pin connected to LED
int brightness = 0;   // Current brightness
int fadeAmount = 5;   // How much to change the brightness each loop

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  analogWrite(ledPin, brightness);  // Set the brightness

  brightness = brightness + fadeAmount;  // Change brightness

  // Reverse the direction of fading at the ends
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }

  delay(30);  // Wait for a short time before next loop
}
