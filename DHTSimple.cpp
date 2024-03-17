/*
//-------------------------------------------------------------------------------------
// DHTSimple.h
// Arduino library for DHT Sensors
// Created by Rick Lloyd February 2024
// Tested with      : DHT11 and DHT22
// Tested with MCU  : Arduino Uno, Mega, ESP32, ESP8266 
//-------------------------------------------------------------------------------------
*/
#include <Arduino.h>
#include "DHTSimple.h"

DHTSimple::DHTSimple(int datapin, int SensorType)  	//constructor number of devices and size of array
{   
  pin = datapin;   
  Sensor = SensorType;
}	
void DHTSimple::begin(void)  	
{   
  pinMode(pin, INPUT_PULLUP);
  if (DebugFlag) {pinMode(DebugPin, OUTPUT); digitalWrite(DebugPin, HIGH); }
}	
void DHTSimple::debug(int outpin)  	
{   
  DebugPin = outpin;
  DebugFlag = true; 
}
String DHTSimple::getData(float &h, float &t) {
  h = 0; t = 0;	
  String err = ReadSensor(h, t);
  //Serial.println("Err " + err);  
  return err;
}	
String DHTSimple::getDataRetry (float &h, float &t) {
  String err = "";	
  h = 0; t = 0;
  for (int i = 0; i < 2; i++) {
    err = ReadSensor(h, t);	
    if(err == "OK") return err;
	delay(10);
  }
  h = -1; t = -1;
  return err;
}  
 	  
String DHTSimple::ReadSensor(float &h, float &t) {
  #define MaxTime 4294967295  //micros() time just before rollover
  unsigned int seg[5];
  
  int dout; unsigned int CheckSum;
  unsigned long sTime; unsigned long RunTime;

/* Avoid Microseconds Overflow */
  //Serial.println("getData Subroutine"); delay(1000);
  sTime = micros();
  RunTime = (Sensor == 2) ? 6000 : 25000;
  if (sTime > (MaxTime - RunTime)) {
    unsigned long WaitTime = (MaxTime - sTime);
    //Serial.println("Waiting " + String(WaitTime) + " microseconds for Rollover");
    delayMicroseconds(WaitTime + 10);
  }
  //if (DebugFlag) Serial.println("Debug Pin " + String(DebugPin));
 
  sTime = micros();
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);                      // Wake Up Sensor       
  if (DebugFlag) digitalWrite(DebugPin, LOW);   //Trigger
  unsigned int StartDelay = (Sensor == 2) ? 1500 : 19000;
  delayMicroseconds(StartDelay);               // delay start based upon sensor type
  if (DebugFlag) digitalWrite(DebugPin, HIGH);      
  digitalWrite(pin, HIGH);                     // Start Sensor       
  delayMicroseconds(30);  
  pinMode(pin, INPUT_PULLUP);
  if (DebugFlag) digitalWrite(DebugPin, LOW);
  if (FallingEdge(40) == -1) return "Sensor Bad";    //Sensor did not pull down signal  
  if (RisingEdge(120) == -1) return "No Start Bit";  //Sensor did not pull Up signal
  sTime = micros();
  if (FallingEdge(120) == -1) return "No Start Bit End";  
  int StartBitDuration = micros() - sTime;
  if (StartBitDuration < 60 || StartBitDuration > 100) return "Start Bit Short";
/* Read 40 bits of data into 5 Segments */  
  for (int j = 0; j < 5; j++) {                  // loop through 5 segments 40 bits                         
    seg[j] = 0;
    for (int i = 0; i < 8; i++) {                // get a segment of data
	  sTime = micros();
	  if (DebugFlag) digitalWrite(DebugPin, HIGH);    //Trigger  
	  if (RisingEdge(150) == -1) return "Data Read Error";
	  if (FallingEdge(150) == -1) return "Data Read Error";
      //delayMicroseconds(60 - ProcessorDelay);
	  if (DebugFlag) digitalWrite(DebugPin, LOW);    
	  dout = ((micros() - sTime) > 90) ? 1 : 0;
      seg[j] = (seg[j] << 1) | dout;
    }
  }
/* Check if Temperature is Negative */  
  if (DebugFlag) digitalWrite(DebugPin, HIGH);
  boolean negflag = false;
  if (bitRead(seg[2],7) == 1) {
    bitClear(seg[2],7);
    negflag = true;
  }
  CheckSum = seg[0] + seg[1] + seg[2] + seg[3];
  if (seg[4] == 0 && CheckSum == 0) return "No Data";
  CheckSum = CheckSum % 256;
  if (CheckSum != seg[4]) return "Checksum Error";
  t = float(seg[2] * 256 + seg[3]) / 10;
  if(negflag) t = t * -1;
  h = float(seg[0] * 256 + seg[1]) / 10;
  //Serial.println("Execution Time = " + String(micros() - StartTime) + " microseconds");
  return "OK";
}

int DHTSimple::RisingEdge(unsigned int maxwait){
  unsigned long etime = micros() + maxwait;
  while (digitalRead(pin) == LOW) {  
    if (micros() > etime) return -1;   
  }
  return 1;
}
int DHTSimple::FallingEdge(unsigned int maxwait){
  unsigned long etime = micros() + maxwait;
  while (digitalRead(pin) == HIGH) {  
    if (micros() > etime)  return -1; 
  } 	
  return 0;
}