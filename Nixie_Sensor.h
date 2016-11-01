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
		byte Address = 0;		
		int Temp;
		int TempMin;
		int TempMax;

		bool BatWarn;
		
		byte Hum;
		byte HumMin;
		byte HumMax;

		unsigned long TempMin_T;
		unsigned long TempMax_T;
		unsigned long HumMin_T;
		unsigned long HumMax_T;
		
		byte TempMinMaxLed;
		byte HumMinMaxLed;
		
		void Init(byte adr, int temp, byte hm, bool Bat, unsigned long tm);
		
		void SetValues(int temp, byte hm, bool Bat, unsigned long tm);
		
		void ClearMinMax(unsigned long tm);
		
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