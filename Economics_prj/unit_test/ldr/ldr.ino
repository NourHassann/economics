/*
one leg to 5v 
other to resistor and pin in esp 
resistor leg to gnd (220 ohm)

connect led on pin 2 of the esp
*/
int sensor;

void setup()
{
  Serial.begin(9600);
  pinMode(2,OUTPUT);
}

void  loop()
{
  sensor=analogRead(A0);

    Serial.print("SENSOR VALUE=");
    Serial.println(sensor);
    delay(300);

 if(sensor<30)
 {
  digitalWrite(2,HIGH);  
 }
 else
 {
  digitalWrite(2,LOW);
 }
}
