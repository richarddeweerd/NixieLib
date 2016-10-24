/*
   Morse.h - Library for flashing Morse code.
   Created by David A. Mellis, November 2, 2007.
   Released into the public domain.
 */
#ifndef Nixie_Sensor_h
#define Nixie_Sensor_h

#include "Arduino.h"

class RF_Sensor
{
	public:
		RF_Sensor();
		byte Address;
		float Temp;
		float TempMin;
		float TempMax;
		float DewPoint;
		byte Hum;
		byte HumMin;
		byte HumMax;
		byte TempMinMaxLed;
		byte HumMinMaxLed;
		
		void Init(byte adr);
		void SetTemp(float temp, unsigned long tm);
		void SetHum(byte hm, unsigned long tm);
		void ClearMinMax(unsigned long tm);
		unsigned long TempMin_T;
		unsigned long TempMax_T;
		unsigned long HumMin_T;
		unsigned long HumMax_T;
		
	private:
		boolean  _NewTemp;
		boolean  _NewHum;
		void SetDewPoint();
};

class Baro_Sensor
{
	public:
		Baro_Sensor();
		short Temp;
		int Pressure;
		int PressureMin;
		int PressureMax;
		byte MinMaxLed;

		unsigned long PressMin_T;
		unsigned long PressMax_T;


		void Init(byte adr);

		void SetPressure(int press, unsigned long tm);
		void SetTemp(short tmp);
		void ClearMinMax(unsigned long tm);
		
	private:
		boolean  _New;
};


#endif