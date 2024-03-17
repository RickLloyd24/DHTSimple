/*
//-------------------------------------------------------------------------------------
// DHTSimple.h
// Arduino library for DHT Sensors
// Created by Rick Lloyd February 2024
// Tested with      : DHT11 and DHT22
// Tested with MCU  : Arduino Uno, Mega, ESP32, ESP8266 
//-------------------------------------------------------------------------------------
*/

#ifndef DHTSimple_h
#define DHTSimple_h

#include "Arduino.h"

class DHTSimple
{
  public:
	DHTSimple(int datapin, int SensorType);  	                   	//constructor
	void begin();   							 	                //DHT data pin mode input pull up
	void debug(int outpin);
	String getData(float &h, float &t);								//get himidity and Temperature
	String getDataRetry(float &h, float &t);						//get himidity and Temperature
  private:
	uint8_t pin; 									    			//DHT Data pin
	int RisingEdge(unsigned int maxwait);
	int FallingEdge(unsigned int maxwait);
	String ReadSensor(float &h, float &t);
	int Sensor = 2;
	int DebugPin = 26;
	bool DebugFlag = false;
};	

#endif