#include <Arduino.h>
#include <DHTSimple.h>

#define DHTPIN1 26
#define DHTPIN2 27
#define DHTSensorType 2   //DHT11 is type 1, all others are type 2


float t1 = 0; float h1 = 0;
float t2 = 0; float h2 = 0;
String s1 = ""; String s2 = "";
unsigned long TaskTime = 0;

DHTSimple DHT1(DHTPIN1, DHTSensorType);
DHTSimple DHT2(DHTPIN2, DHTSensorType);


void setup() {
  /* Setup Serial Interface */
  Serial.begin (115200);
  delay (500);
  Serial.println ("");
  Serial.println("Booting");

  /* Setup GPIO Pin Modes */
   DHT1.begin();
   DHT2.begin();
   delay(1000);
   Serial.println("DHT Sensors Begun");
}

void loop() {
  unsigned long curtime = millis();
  
/* 10 Second Tasks */  
  if (curtime > TaskTime) {                                                     
    TaskTime = TaskTime + 5000;                       //Every 5 seconds
    s1 = DHT1.getData(h1, t1);
    //Serial.println("Return " + s1);
    s2 = DHT2.getDataRetry(h2, t2);
    if (s1 == "OK") {
      t1 = t1 * 9 / 5 + 32;
      Serial.print("Sensor #1 = " + String(h1, 1) + " %, " + String(t1, 1) + " F : ");
    }
    else {
      Serial.print(s1);
    }
    if (s2 == "OK") {
      t2 = t2 * 9 / 5 + 32;
      Serial.print("Sensor #2 = " + String(h2, 1) + " %, " + String(t2, 1) + " F");
    }
    else {
      Serial.print(s1);  
    }
    Serial.println();
  }  
}
