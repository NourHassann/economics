/*
MQ-2 Pin	Arduino Pin
VCC	        5V
GND	        GND
A0	        A0
D0	        Not used 
*/
const int mq2Pin = A0;  // Analog pin 

void setup() {
  Serial.begin(9600);   // Start serial communication
}

void loop() {
  int sensorValue = analogRead(mq2Pin);  // Read analog value
  Serial.print("MQ-2 Sensor Value: ");
  Serial.println(sensorValue);          // Print value to Serial Monitor

  delay(1000);  // Wait 1 second between readings
}
/*
results must be 150–300 (out of 1023) in clean air.
The sensor needs 20 seconds to 2 minutes after powering on before stable readings
*/