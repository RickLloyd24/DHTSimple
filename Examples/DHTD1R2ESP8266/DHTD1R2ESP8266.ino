/* ESP8266 D1 R2 */
#include <Arduino.h>
#include <DHTSimple.h>

#define DHT1PIN D5
#define DHTSensorType 2   //DHT11 is type 1, all others are type 2
float t = 0;
float h = 0;

DHTSimple DHT1(DHT1PIN, DHTSensorType); //

void setup() {
  /* Setup Serial Interface */
  Serial.begin (115200);
  delay (500);
  Serial.println ("");
  Serial.println("Booting");

  DHT1.begin();
  delay(2000);
  Serial.println("DHT Sensor Started");
}

void loop() {
   int errcnt = 0;
   String s = DHT1.getData(h, t);
   if (s == "OK") {
     t = t * 9 / 5 + 32;
     Serial.println("Humidity = " + String(h, 1) + " %, " + "Temp = " + String(t, 1) + " F, Errors " + String(errcnt));
   }
   else {
     Serial.println("Error = " + s); errcnt++;
   }
   delay(3000);
}
