/* Wemos D1 R32 Board */
#include <Arduino.h>
#include <DHTSimple.h>

#define DHT1PIN 14
#define DebugPin 12
#define Test 13
#define DHTSensorType 1   //DHT11 is type 1, all others are type 2
float t = 0;
float h = 0;

DHTSimple DHT1(DHT1PIN, DHTSensorType); //

void setup() {
/* Setup Serial Interface */
  Serial.begin (115200);
  delay (500);
  Serial.println ("");
  Serial.println("Booting");

  pinMode(Test, OUTPUT);
  digitalWrite(Test, HIGH);
/* Setup DHT and activate Debug Interface */  
  DHT1.debug(DebugPin);
  DHT1.begin();
  delay(2000);
  Serial.println("DHT Sensor Started");
}

void loop() {
   String s = DHT1.getDataRetry(h, t);
   if (s == "OK") {
     t = t * 9 / 5 + 32;
     Serial.println("Humidity = " + String(h, 1) + " %, " + "Temperature = " + String(t, 1) + " F");
   }
   else {
     Serial.println("Error = " + s);  
   }
   delay(3000);
}
