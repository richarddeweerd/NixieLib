#include "Arduino.h"
#include "Nixie_Sensor.h"

RF_Sensor::RF_Sensor(){
}

void RF_Sensor::SetValues(int temp, byte hm, bool Bat, unsigned long tm){
	BatWarn = Bat;	
	
	Temp = temp;

	if (temp < TempMin){
		TempMin = temp;
		TempMin_T = tm;
	} else {	
		if (temp > TempMax){
			TempMax = temp;
			TempMax_T = tm;
		}
	}
	if (temp == TempMin){
		TempMinMaxLed=1;
	} else {
		TempMinMaxLed=0;
	} 
	if (temp == TempMax){
		TempMinMaxLed +=2;
	} 
	

	Hum = hm;
	if (hm < HumMin){
		HumMin = hm;
		HumMin_T = tm;
	} else {	
		if (hm > HumMax){
			HumMax = hm;
			HumMax_T = tm;
		}
	}
	if (hm == HumMin){
		HumMinMaxLed=1;
	} else {
		HumMinMaxLed=0;
	} 
	if (hm == HumMax){
		HumMinMaxLed +=2;
	} 
}

void RF_Sensor::Init(byte adr, int temp, byte hm, bool Bat, unsigned long tm){
	Address = adr;

	BatWarn = Bat;	
	Temp = temp;

	TempMin = temp;
	TempMax = temp;
	TempMin_T = tm;
	TempMax_T = tm;

	Hum = hm;
	HumMin = hm;
	HumMax = hm;
	HumMin_T = tm;
	HumMax_T = tm;

	HumMinMaxLed=0;
	TempMinMaxLed=0;
}

void RF_Sensor::ClearMinMax(unsigned long tm){
	TempMin = Temp;
	TempMax = Temp;
	HumMin = Hum;
	HumMax = Hum;

	TempMin_T = tm;
	TempMax_T = tm;
	HumMin_T = tm;
	HumMax_T = tm;
	HumMinMaxLed=0;
	TempMinMaxLed=0;
}


Baro_Sensor::Baro_Sensor(){
	MinMaxLed=0;
	_New = true;
}

void Baro_Sensor::ClearMinMax(unsigned long tm){
	PressureMin = Pressure;
	PressureMax = Pressure;
	PressMin_T = tm;
	PressMax_T = tm;
	MinMaxLed=0;
}

void Baro_Sensor::SetPressure(int press, unsigned long tm){
	Pressure = press;
	if (_New == false){
		if (press < PressureMin){
			PressureMin = press;
			PressMin_T = tm;
		} else {	
			if (press > PressureMax){
				PressureMax = press;
				PressMax_T = tm;
			}
		}
		if (press == PressureMin){
			MinMaxLed=1;
		} else {
			MinMaxLed=0;
		} 
		if (press == PressureMax){
			MinMaxLed +=2;
		} 
			

		
	} else {
		ClearMinMax(tm);
		_New = false;
	}
}

void Baro_Sensor::SetTemp(short tmp){
	Temp = tmp;
}
