// ======== BLYNK SETUP ========
#define BLYNK_TEMPLATE_ID "TMPL2CXBhRYG3"
#define BLYNK_TEMPLATE_NAME "Nour"
#define BLYNK_AUTH_TOKEN "H4h1ol34gcbPsUe4iBfD5y90-ktpJRj-"

// Import necessary libraries for WiFi and Blynk
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
BlynkTimer timer;

// Network Credentials
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "nour";  // WiFi SSID
char pass[] = "123456789";  // WiFi Password

// ======== PINS CONFIGURATION ========
const int motorIn1 = 14;   // Motor control pin 1 
const int motorIn2 = 27;   // Motor control pin 2 
const int motorEnable = 26; // Motor enable pin 

const int ldrPin = 34;     // LDR sensor pin 
const int ledPin = 25;     // LED pin 
const int led2Pin = 33;    // LED pin
const int smokePin = 35;   // Smoke sensor pin 

// ======== CONTROL VARIABLES ========
int motorSpeed = 0;  // Motor speed (0-255)
bool motorOn = false;  // Motor ON/OFF state
bool motorDirection = true;  // Motor direction (true for one direction, false for the opposite)
bool dimmingEnabled = false;  // Flag to enable/disable dimming

int brightness = 0;  // LED brightness for dimming
int fadeAmount = 15;  // Amount of brightness change for dimming
unsigned long lastDimUpdate = 0;  // Last update time for dimming logic

// ======== BLYNK WRITE HANDLERS ========
BLYNK_WRITE(V6) {
  motorSpeed = param.asInt();  // Get motor speed from the app (V6 pin)
}

BLYNK_WRITE(V7) {
  motorOn = param.asInt();  // Get motor ON/OFF status from the app (V7 pin)
}

BLYNK_WRITE(V2) {
  motorDirection = param.asInt();  // Get motor direction from the app (V2 pin)
}

BLYNK_WRITE(V8) {
  dimmingEnabled = param.asInt();  // Get dimming enable/disable status from the app (V8 pin)
}

void setup() {
  // Initialize Serial communication
  Serial.begin(9600);

  // Set pin modes
  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  pinMode(motorEnable, OUTPUT);
  pinMode(ledPin, OUTPUT);

  // Connect to Blynk
  Blynk.begin(auth, ssid, pass);

  // Set a timer to send sensor data every 1 second
  timer.setInterval(1000L, sendSensorData);
}

void loop() {
  // Handle Blynk communication
  Blynk.run();

  // Run the timer
  timer.run();

  // Control the motor based on app inputs
  controlMotor();

  // Handle the LED dimming
  handleDimming();
}

// ======== MOTOR CONTROL FUNCTION ========
void controlMotor() {
  if (motorOn) {
    // Set motor direction
    digitalWrite(motorIn1, motorDirection ? HIGH : LOW);
    digitalWrite(motorIn2, motorDirection ? LOW : HIGH);
    // Set motor speed
    analogWrite(motorEnable, motorSpeed);
  } else {
    // Turn off motor if motorOn is false
    analogWrite(motorEnable, 0);
  }
}

// ======== DIMMING FUNCTION ========
void handleDimming() {
  if (dimmingEnabled) {
    // Smooth fade in/out effect
    if (millis() - lastDimUpdate > 30) {
      analogWrite(ledPin, brightness);
      brightness += fadeAmount;
      // Reverse the fade direction when limits are reached
      if (brightness <= 0 || brightness >= 255) {
        fadeAmount = -fadeAmount;
      }
      lastDimUpdate = millis();
    }
  } else {
    // Turn off LED if dimming is not enabled
    analogWrite(ledPin, 0);
    brightness = 0;
  }
}

// ======== SENSOR DATA FUNCTION (For Blynk) ========
void sendSensorData() {
  // Read LDR sensor value and map it to PWM range (0-255)
  int ldrValue = analogRead(ldrPin);
  int ledPWM = map(ldrValue, 0, 1023, 0, 255);
  analogWrite(led2Pin, ledPWM);  // Optional: if not dimming, control LED brightness with LDR value
  Blynk.virtualWrite(V1, ledPWM);  // Send LDR PWM value to the app (V1 pin)

  // Read smoke sensor value
  int smokeValue = analogRead(smokePin);
  Blynk.virtualWrite(V0, smokeValue);  // Send smoke sensor value to the app (V0 pin)

  // Send current motor speed to the app (V6 pin)
  Blynk.virtualWrite(V6, motorSpeed);
}
