#define BLYNK_TEMPLATE_ID "TMPL2CXBhRYG3"
#define BLYNK_TEMPLATE_NAME "Nour"
#define BLYNK_AUTH_TOKEN "H4h1ol34gcbPsUe4iBfD5y90-ktpJRj-"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
BlynkTimer timer;

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "nour";
char pass[] = "123456789";

// ===== PINS =====
const int motorIn1 = 14;
const int motorIn2 = 27;
const int motorEnable = 26;

const int ldrPin = 34;
const int led2Pin = 33; // LDR-controlled LED

const int smokePin = 35;
const int smokeRGB_R = 12;
const int smokeRGB_G = 13;

const int roomLEDPin = 32;   // First room LED (V9)
const int roomLEDPin2 = 23;  // Second room LED (V19)

const int relay1 = 15;
const int relay2 = 2;
const int relay3 = 4;

const int pot1 = 36;
const int pot2 = 39;
const int pot3 = 34;

// ===== VARIABLES =====
int motorSpeed = 0;
bool motorOn = false;
bool motorDirection = true;
bool ldrLedEnabled = true;

bool relayOverride = false;
bool relay1Manual = false, relay2Manual = false, relay3Manual = false;

// ===== BLYNK HANDLERS =====
BLYNK_WRITE(V6) { motorSpeed = param.asInt(); }
BLYNK_WRITE(V7) { motorOn = param.asInt(); }
BLYNK_WRITE(V2) { motorDirection = param.asInt(); }
BLYNK_WRITE(V5) { ldrLedEnabled = !param.asInt(); } // If V5 ON, disable LDR LED

BLYNK_WRITE(V9) {digitalWrite(roomLEDPin, param.asInt());}
BLYNK_WRITE(V19) {digitalWrite(roomLEDPin2, param.asInt());}

BLYNK_WRITE(V15) { relay1Manual = param.asInt(); relayOverride = true; }
BLYNK_WRITE(V17) { relay2Manual = param.asInt(); relayOverride = true; }
BLYNK_WRITE(V16) { relay3Manual = param.asInt(); relayOverride = true; }

BLYNK_WRITE(V18) {
  bool master = param.asInt();
  relayOverride = true;
  relay1Manual = master;
  relay2Manual = master;
  relay3Manual = master;
}

void setup() {
  Serial.begin(9600);

  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  pinMode(motorEnable, OUTPUT);

  pinMode(led2Pin, OUTPUT);
  pinMode(smokeRGB_R, OUTPUT);
  pinMode(smokeRGB_G, OUTPUT);
  pinMode(roomLEDPin, OUTPUT);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sendSensorData);
  timer.setInterval(1000L, handleRelays);
}

void loop() {
  Blynk.run();
  timer.run();
  controlMotor();
}

// ===== MOTOR CONTROL =====
void controlMotor() {
  if (motorOn) {
    digitalWrite(motorIn1, motorDirection ? HIGH : LOW);
    digitalWrite(motorIn2, motorDirection ? LOW : HIGH);
    analogWrite(motorEnable, motorSpeed);
  } else {
    analogWrite(motorEnable, 0);
  }
}

// ===== SENSOR DATA =====
void sendSensorData() {
  int ldrValue = analogRead(ldrPin);
  int ldrPWM = map(ldrValue, 0, 1023, 0, 255);
  if (ldrLedEnabled) analogWrite(led2Pin, ldrPWM);
  else analogWrite(led2Pin, 0);
  Blynk.virtualWrite(V1, ldrPWM);

  int smokeValue = analogRead(smokePin);
  Blynk.virtualWrite(V0, smokeValue);

  if (smokeValue > 800) {
    digitalWrite(smokeRGB_R, HIGH);
    digitalWrite(smokeRGB_G, LOW);
  } else {
    digitalWrite(smokeRGB_R, LOW);
    digitalWrite(smokeRGB_G, HIGH);
  }

  Blynk.virtualWrite(V6, motorSpeed);
}

// ===== RELAY LOGIC =====
void handleRelays() {
  int p1 = analogRead(pot1);
  int p2 = analogRead(pot2);
  int p3 = analogRead(pot3);

  int sum = p1 + p2 + p3;
  Blynk.virtualWrite(V11, p1);
  Blynk.virtualWrite(V12, p2);
  Blynk.virtualWrite(V13, p3);
  Blynk.virtualWrite(V14, sum);

  if (!relayOverride) {
    digitalWrite(relay1, sum > 10 ? HIGH : LOW);
    digitalWrite(relay2, sum > 20 ? HIGH : LOW);
    digitalWrite(relay3, sum > 30 ? HIGH : LOW);
  } else {
    digitalWrite(relay1, relay1Manual ? HIGH : LOW);
    digitalWrite(relay2, relay2Manual ? HIGH : LOW);
    digitalWrite(relay3, relay3Manual ? HIGH : LOW);
  }

  if (sum <= 0) {
    relay1Manual = relay2Manual = relay3Manual = false;
    relayOverride = false;
  }
}
