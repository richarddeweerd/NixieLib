#include "Arduino.h"
#include "Nixie_Sensor.h"

RF_Sensor::RF_Sensor(){
}

void RF_Sensor::SetTemp(float tmp, unsigned long tm){
	Temp = tmp;
	if (_NewTemp == false){
		if (tmp < TempMin){
			TempMin = tmp;
			TempMin_T = tm;
		} else {	
			if (tmp > TempMax){
				TempMax = tmp;
				TempMax_T = tm;
			}
		}
		if (tmp == TempMin){
			TempMinMaxLed=1;
		} else {
			TempMinMaxLed=0;
		} 
		if (tmp == TempMax){
			TempMinMaxLed +=2;
		} 
	} else {
		TempMin = tmp;
		TempMax = tmp;
		TempMin_T = tm;
		TempMax_T = tm;
		_NewTemp = false;
	}
	
	//SetDewPoint();
}

void RF_Sensor::SetHum(byte hm, unsigned long tm){
	if (hm==100){
		hm=99;
	}
	Hum = hm;
	if (_NewHum == false){
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
	} else {
		HumMin = hm;
		HumMax = hm;
		HumMin_T = tm;
		HumMax_T = tm;
		_NewHum = false;
	}


		
	//SetDewPoint();
}

void RF_Sensor::Init(byte adr){
	Address = adr;

	_NewTemp = true;
	_NewHum = true;

	Temp = 0;
	TempMin = 0;
	TempMax = 0;
	Hum = 0;
	HumMin = 0;
	HumMax = 0;
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

void RF_Sensor::SetDewPoint(){
	if ((_NewHum == false)&&(_NewTemp == false)){
	DewPoint = Temp - (100-Hum)*pow(((Temp+273.15)/300),2)/5 - 0.00135*pow(Hum-84,2) + 0.35;
	}
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
