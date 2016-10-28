#include "Arduino.h"
#include "Nixie.h"

Nixie_Display::Nixie_Display(byte c_latchp, byte c_clockp, byte c_datap, byte w_latchp, byte w_clockp, byte w_datap, byte c_highp, byte c_lowp, byte w_dotp, byte c_minp, byte c_maxp, byte bl_pin, byte hv_pin){
	_C_LatchPin = c_latchp;
	_C_ClockPin = c_clockp;
	_C_DataPin = c_datap;
	_W_LatchPin = w_latchp;
	_W_ClockPin = w_clockp;
	_W_DataPin = w_datap;
	_C_HighPin = c_highp;
	_C_LowPin = c_lowp;
	_W_DotPin = w_dotp;
	_W_MinPin = c_minp;
	_W_MaxPin = c_maxp;
	_BL_Pin = bl_pin;
	_HV_Pin = hv_pin;
	
	BackLightDay = 0;
	BackLightNight = 0;
	
	_BlinkPos = 0;
	_ValChanged = 0;
	
	DimStatus = 0;
	NightMode = 0;
	PulseCount = 0;
	
	pinMode(_C_LatchPin, OUTPUT);
	pinMode(_C_ClockPin, OUTPUT);
	pinMode(_C_DataPin, OUTPUT);
  
	pinMode(_W_LatchPin, OUTPUT);
	pinMode(_W_ClockPin, OUTPUT);
	pinMode(_W_DataPin, OUTPUT);
	
	pinMode(_C_HighPin, OUTPUT);
	pinMode(_C_LowPin, OUTPUT);
	pinMode(_W_DotPin, OUTPUT);
	pinMode(_W_MinPin, OUTPUT);
	pinMode(_W_MaxPin, OUTPUT);

	pinMode(_BL_Pin, OUTPUT);
	pinMode(_HV_Pin, OUTPUT);
	
	HV_Off();
	
	analogWrite(_BL_Pin,DimStatus); //cleanup needed
	
	digitalWrite(_C_HighPin, HIGH);
	digitalWrite(_C_LowPin, HIGH);
	digitalWrite(_W_DotPin, HIGH);
	digitalWrite(_W_MinPin, HIGH);
	digitalWrite(_W_MaxPin, HIGH);
}

void Nixie_Display::HV_On(){
	digitalWrite(_HV_Pin, HIGH);
}

void Nixie_Display::HV_Off(){
	digitalWrite(_HV_Pin, LOW);
}


void Nixie_Display::Leds_On(){
	digitalWrite(_C_HighPin, LOW);
	digitalWrite(_C_LowPin, LOW);
	digitalWrite(_W_DotPin, LOW);
	digitalWrite(_W_MinPin, LOW);
	digitalWrite(_W_MaxPin, LOW);
}

void Nixie_Display::Leds_Off(){
	digitalWrite(_C_HighPin, HIGH);
	digitalWrite(_C_LowPin, HIGH);
	digitalWrite(_W_DotPin, HIGH);
	digitalWrite(_W_MinPin, HIGH);
	digitalWrite(_W_MaxPin, HIGH);
}

void Nixie_Display::Date_Leds_On(){
	digitalWrite(_C_HighPin, HIGH);
	digitalWrite(_C_LowPin, LOW);
}
void Nixie_Display::Date_Leds_Off(){
	digitalWrite(_C_HighPin, HIGH);
	digitalWrite(_C_LowPin, HIGH);
}


void Nixie_Display::SetMinMaxLed(byte MinMaxL){
	switch (MinMaxL){
		case 0:
			digitalWrite(_W_MinPin, HIGH);
			digitalWrite(_W_MaxPin, HIGH);
			break;
		case 1:
			digitalWrite(_W_MinPin, LOW);
			digitalWrite(_W_MaxPin, HIGH);
			break;
		case 2:
			digitalWrite(_W_MinPin, HIGH);
			digitalWrite(_W_MaxPin, LOW);
			break;
		case 3:
			digitalWrite(_W_MinPin, LOW);
			digitalWrite(_W_MaxPin, LOW);
			break;
	}
}

void Nixie_Display::Pulse(){
	if (PulseCount != _lastPulse){
		_lastPulse = PulseCount;
		if (_ValChanged > 0){
			_ValChanged--;
		}
					

		if (NightMode == 0){
			if (ScreenSaverActive == 0){
				switch (_TimeDisplay){
					case 1:
						//Time displayed
						if (PulseCount < (PulsesPerSec/2)){
							Clock_Blink_On();
						} else {
							Clock_Blink_Off();
						}					
						break;
					case 2:
						Date_Leds_On();
						break;
				}

			} else {
				ScreenSaverPulse();
				_ScrStepCounter--;
				if (_ScrStepCounter == 0){
					ScreenSaverActive = 0;
				}
			}
		}
	}
	DimmerPulse();
}
	
void Nixie_Display::ShowTime(){
    _TimeDisplay = 1;
	
	byte byte1;
	byte byte2;
	byte byte3;
	
	byte1 = (Time.Hour / 10) << 4;
	byte1 = byte1 | (Time.Hour % 10);

	byte2 = (Time.Minute / 10) << 4;
	byte2 = byte2 | (Time.Minute % 10);
	
	byte3 = (Time.Second / 10) << 4;
	byte3 = byte3 | (Time.Second % 10);
	
	if (_BlinkPos > 0){
		//edit mode
		if (_ValChanged == 0){
			if (PulseCount < (PulsesPerSec/2)){			
				switch (_BlinkPos){
					case 1:
						byte1=0xBB;
						break;
					case 2:
						byte2=0xBB;
						break;
					case 3:
						byte3=0xBB;
						break;
				}							
			} 
		} 
	}
	
	digitalWrite(_C_LatchPin, LOW);
	shiftOut(_C_DataPin, _C_ClockPin, MSBFIRST, byte1); 
	shiftOut(_C_DataPin, _C_ClockPin, MSBFIRST, byte2); 
	shiftOut(_C_DataPin, _C_ClockPin, MSBFIRST, byte3); 	
	digitalWrite(_C_LatchPin, HIGH);
}

void Nixie_Display::ShowDate(){
    _TimeDisplay = 2;
	
	byte byte1;
	byte byte2;
	byte byte3;
	byte yr = (byte) (Time.Year - 30);
	
	byte1 = (Time.Day / 10) << 4;
	byte1 = byte1 | (Time.Day % 10);

	byte2 = (Time.Month / 10) << 4;
	byte2 = byte2 | (Time.Month % 10);
	
	byte3 = (yr / 10) << 4;
	byte3 = byte3 | (yr % 10);

	if (_BlinkPos > 0){
		//edit mode
		if (_ValChanged == 0){
			if (PulseCount < (PulsesPerSec/2)){			
				switch (_BlinkPos){
					case 4:
						byte1=0xBB;
						break;
					case 5:
						byte2=0xBB;
						break;
					case 6:
						byte3=0xBB;
						break;
				}							
			} 
		} 
	}
	
	digitalWrite(_C_LatchPin, LOW);
	shiftOut(_C_DataPin, _C_ClockPin, MSBFIRST, byte1); 
	shiftOut(_C_DataPin, _C_ClockPin, MSBFIRST, byte2); 
	shiftOut(_C_DataPin, _C_ClockPin, MSBFIRST, byte3); 	
	digitalWrite(_C_LatchPin, HIGH);	
}



		
void Nixie_Display::ShowTemp(byte sensor){
	byte byte1;
	byte byte2;
	byte byte3;
	
	byte b1,b2,b3,s;

	int temp;
	
	temp = (int) (RFSensor[sensor].Temp * 10);
	
	if (temp < 0 ){
		s = 8; // 8 = - sign
		temp = temp * -1;
	} else {
		s = 7; // 7 = - sign
	}
	

	if (temp >= 100){
		b1 = temp / 100;
	} else {
		b1 = 11;
	}
	
	temp = temp % 100;
	b2 = temp / 10;
	temp = temp % 10;
	b3 = temp;

	byte1 = sensor << 4;
	byte1 = byte1 | 11 ;

	byte2 = b1 << 4;
	byte2 = byte2 | b2 ;
	
	byte3 = b3 << 4;
	byte3 = byte3 | s;
	
	SetMinMaxLed(RFSensor[sensor].TempMinMaxLed);
	
	digitalWrite(_W_DotPin, LOW); 
	
	digitalWrite(_W_LatchPin, LOW);
	shiftOut(_W_DataPin, _W_ClockPin, MSBFIRST, byte1); 
	shiftOut(_W_DataPin, _W_ClockPin, MSBFIRST, byte2); 
	shiftOut(_W_DataPin, _W_ClockPin, MSBFIRST, byte3); 	
	digitalWrite(_W_LatchPin, HIGH);
}

void Nixie_Display::ShowHum(byte sensor){

	byte byte1;
	byte byte2;
	byte byte3;
	
	byte b1,b2;

	int hum = RFSensor[sensor].Hum;
	
	if (hum >= 10){
		b1 = hum / 10;
	} else {
		b1 = 11;
	}
	
	hum = hum % 10;
	b2 = hum;	
	
	byte1 = sensor << 4;
	byte1 = byte1 | 11 ;

	byte2 = b1 << 4;
	byte2 = byte2 | b2 ;
	
	byte3 = 2 ; // 2 = % sign
	
	SetMinMaxLed(RFSensor[sensor].HumMinMaxLed);
	
	digitalWrite(_W_DotPin, LOW); 
	
	digitalWrite(_W_LatchPin, LOW);
	shiftOut(_W_DataPin, _W_ClockPin, MSBFIRST, byte1); 
	shiftOut(_W_DataPin, _W_ClockPin, MSBFIRST, byte2); 
	shiftOut(_W_DataPin, _W_ClockPin, MSBFIRST, byte3); 	
	digitalWrite(_W_LatchPin, HIGH);
}
		
		
		

void Nixie_Display::ShowPressure(){
	
	
	byte byte1;
	byte byte2;
	byte byte3;
	
	int press = Baro.Pressure;
	
	byte b1,b2,b3,b4,b5;
	if (press >= 10000){
		b1 = press / 10000;
	} else {
		b1 = 11;
	}
		
	press = press % 10000;
	b2 = press / 1000;
	press = press % 1000;
	b3 = press / 100;
	press = press % 100;
	b4 = press / 10;
	press = press % 10;
	b5 = press;	

	
	byte1 = b1 << 4;
	byte1 = byte1 | b2 ;

	byte2 = b3 << 4;
	byte2 = byte2 | b4 ;
	
	byte3 = b5 << 4;
	byte3 = byte3 | 9 ; // 9 = pressure sign	
		
	
	SetMinMaxLed(Baro.MinMaxLed);
	
	digitalWrite(_W_DotPin, LOW); 
	digitalWrite(_W_LatchPin, LOW);
	shiftOut(_W_DataPin, _W_ClockPin, MSBFIRST, byte1); 
	shiftOut(_W_DataPin, _W_ClockPin, MSBFIRST, byte2); 
	shiftOut(_W_DataPin, _W_ClockPin, MSBFIRST, byte3); 	
	digitalWrite(_W_LatchPin, HIGH);
}


void Nixie_Display::SetupPage(byte _page, int _val){
	byte s;
	byte b1,b2;
	_TimeDisplay = 0;
	if (_val < 0 ){
		s = 8; // 8 = - sign
		_val = _val * -1;
	} else {
		s = 7; // 7 = - sign
	}
	b1 = _val / 100;
	if (b1 == 0){
		b1 = 11;
	}
	_val = _val % 100;
	b2 = _val / 10;
	if (b2 == 0){
		b2 = 11;
	}
	
	ShowClock_Dir((_page / 10),(_page %10),11,11,11,11);
	ShowWeather_Dir(11,b1,b2,_val %10,11,s);
	Leds_Off();
}

void Nixie_Display::SetupPage(byte _page, byte _val){
	byte b1,b2;
	_TimeDisplay = 0;
	
	b1 = _val / 100;
	if (b1 == 0){
		b1 = 11;
	}
	_val = _val % 100;
	b2 = _val / 10;
	if (b2 == 0){
		b2 = 11;
	}
	
	ShowClock_Dir((_page / 10),(_page %10),11,11,11,11);
	ShowWeather_Dir(11,b1,b2,_val %10,11,11);
	
	Leds_Off();
}

void Nixie_Display::SetupClock(byte _pos, byte chng){
	if (chng == 1){
		_ValChanged = 20;	
	}
	_BlinkPos =_pos;	
	if (_pos < 4) {
		ShowTime();	
	} else {
		ShowDate();	
	}
	
	ShowWeather_Dir(11,11,11,11,11,11);	
	digitalWrite(_W_DotPin, HIGH);
	digitalWrite(_W_MinPin, HIGH);
	digitalWrite(_W_MaxPin, HIGH);
}

void Nixie_Display::ShowCodeVersion(long ver){
	byte b1,b2,b3,b4,b5;
	Date_Leds_On();
	b1 = ver / 100000;
	ver = ver % 100000;
	b2 = ver / 10000;
	ver = ver % 10000;
	b3 = ver / 1000;
	ver = ver % 1000;
	b4 = ver / 100;
	ver = ver % 100;
	b5 = ver / 10;
	ver = ver % 10;
	ShowClock_Dir(b1,b2,b3,b4,b5,(byte)ver);
	ShowWeather_Dir(11,11,11,11,11,11);	
}

void Nixie_Display::ExitSetup(){	
	_BlinkPos = 0;
	_ValChanged = 0;
}




void Nixie_Display::ShowClock_Dir(byte d1, byte d2, byte d3, byte d4, byte d5, byte d6){

	byte byte1;
	byte byte2;
	byte byte3;
	
	byte1 = d1 << 4;
	byte1 = byte1 | d2 ;

	byte2 = d3 << 4;
	byte2 = byte2 | d4 ;
	
	byte3 = d5 << 4;
	byte3 = byte3 | d6 ;

	digitalWrite(_C_LatchPin, LOW);
	shiftOut(_C_DataPin, _C_ClockPin, MSBFIRST, byte1); 
	shiftOut(_C_DataPin, _C_ClockPin, MSBFIRST, byte2); 
	shiftOut(_C_DataPin, _C_ClockPin, MSBFIRST, byte3); 	
	digitalWrite(_C_LatchPin, HIGH);
	

}

void Nixie_Display::ShowWeather_Dir(byte d1, byte d2, byte d3, byte d4, byte d5, byte d6){
	byte byte1;
	byte byte2;
	byte byte3;
	
	byte1 = d1 << 4;
	byte1 = byte1 | d2 ;

	byte2 = d3 << 4;
	byte2 = byte2 | d4 ;
	
	byte3 = d5 << 4;
	byte3 = byte3 | d6 ;
	
	digitalWrite(_W_LatchPin, LOW);
	shiftOut(_W_DataPin, _W_ClockPin, MSBFIRST, byte1); 
	shiftOut(_W_DataPin, _W_ClockPin, MSBFIRST, byte2); 
	shiftOut(_W_DataPin, _W_ClockPin, MSBFIRST, byte3); 	
	digitalWrite(_W_LatchPin, HIGH);
}


void Nixie_Display::NightmodeStart(){
  NightMode = 1;
  HV_Off();
  DimmerStart(BackLightNight,25);
  Leds_Off();
}

void Nixie_Display::NightmodeWake(){
}

void Nixie_Display::NightmodeEnd(){
  NightMode = 0;
  HV_On();
  DimmerStart(BackLightDay,25);
}

void Nixie_Display::C_Blink_On(){
	if (NightMode != 1){
		switch (_TimeDisplay){
			case 0:
				digitalWrite(_C_HighPin, LOW);
				digitalWrite(_C_LowPin, LOW);
				break;
			case 1:
				digitalWrite(_C_HighPin, HIGH);
				digitalWrite(_C_LowPin, LOW);
				break;
			case 2:
				digitalWrite(_C_HighPin, HIGH);
				digitalWrite(_C_LowPin, HIGH);
				break;			
		}
	}
}

void Nixie_Display::Clock_Blink_On(){
	digitalWrite(_C_HighPin, LOW);
	digitalWrite(_C_LowPin, LOW);
}

void Nixie_Display::Clock_Blink_Off(){
	digitalWrite(_C_HighPin, HIGH);
	digitalWrite(_C_LowPin, HIGH);
}







void Nixie_Display::SecPulse(byte _pulse){
	if (_Setup_NoBlink > 0){
		_Setup_NoBlink--;
	}
	_Sec_Stat = _pulse;
}



void Nixie_Display::C_Blink_Off(){
	digitalWrite(_C_HighPin, HIGH);
	digitalWrite(_C_LowPin, HIGH);
}



void Nixie_Display::DimmerStart(byte target, byte speed){
	_DimTarget = target;
	_DimSpeed = speed;
	_LastDim = millis();
	DimActive = 1;
}

void Nixie_Display::DimmerPulse(){
	if (DimActive == 1){
		if (((millis() - _LastDim) >= _DimSpeed)|| (millis() < _LastDim)){
			_LastDim += _DimSpeed;
			if (_DimTarget > DimStatus){
				DimStatus++;
			} else {
				if (_DimTarget < DimStatus){
					DimStatus--;
				}
			}
			if (_DimTarget == DimStatus){
				DimActive = 0;
			}
			analogWrite(_BL_Pin,DimStatus);
		}
	}
}

void Nixie_Display::ScreenSaverPulse(){
	byte clearspeed = 1;
	byte scrspeed = 2;
	int pressure = Baro.Pressure;
	if ((_ScrSteps - _ScrStepCounter)<=21){
		//Clear screen
		int i = (_ScrSteps - _ScrStepCounter)/clearspeed;
		switch (i){			
			case 0:
				c5 = Time.Second / 10;
				c6 = 11;
				w1 = 11;
				break;
			case 1:
				c5 = 11;
				w2 = 11;

				break;
			case 2:
				c4 = 11;
				w3 = 11;
				break;
			case 3:
				c3 = 11;
				w4 = 11;
				break;
			case 4:
				c2 = 11;
				w5 = 11;
				break;
			case 5:
				c1 = 11;
				w6 = 11;
				break;
		}
	} 
	else 
	{
		if (_ScrStepCounter<=20){
			//Fill screen
			int i = _ScrStepCounter/clearspeed;

			switch (i){
				case 5:
					c1 = 11;
					c2 = 11;
					c3 = 11;
					c4 = 11;
					c5 = 11;
					c6 = 11;
					w1 = 11;
					w2 = 11;
					w3 = 11;
					w4 = 11;
					w5 = 11;
					w6 = 11;
					break;
				case 4:
					c1 = Time.Hour / 10;
					w6 = 9;
					break;
				case 3:
					c2 = Time.Hour % 10;
					w5 = pressure;	
					break;
				case 2:
					c3 = Time.Minute / 10;
					pressure = pressure % 100;
					w4 = pressure / 10;
					break;
				case 1:
					c4 = Time.Minute % 10;
					pressure = pressure % 1000;
					w3 = pressure / 100;
					break;
				case 0:
					c5 = Time.Second / 10;
					pressure = pressure % 10000;
					w2 = pressure / 1000;
					break;
				default:
					c1 = 11;
					c2 = 11;
					c3 = 11;
					c4 = 11;
					c5 = 11;
					c6 = 11;
					w1 = 11;
					w2 = 11;
					w3 = 11;
					w4 = 11;
					w5 = 11;
					w6 = 11;					
			}		
		}
		else 
		{			
			//middle
			int i = ((_ScrStepCounter-20)%(scrspeed*6))/scrspeed;
			switch (i){

				case 5:
					c6 = 11;
					w1 = 11;
					c1 = Randomise(c1);
					w6 = Randomise(w6);
					break;
				case 4:
					c1 = 11;
					w6 = 11;				
					c2 = Randomise(c2);
					w5 = Randomise(w5);
					break;
				case 3:
					c2 = 11;
					w5 = 11;
					c3 = Randomise(c3);
					w4 = Randomise(w4);
					break;
				case 2:
					c3 = 11;
					w4 = 11;
					c4 = Randomise(c4);
					w3 = Randomise(w3);
					break;
				case 1:
					c4 = 11;
					w3 = 11;
					c5 = Randomise(c5);
					w2 = Randomise(w2);
					break;
				case 0:
					c5 = 11;
					w2 = 11;
					c6 = Randomise(c6);
					w1 = Randomise(w1);
					break;					
			}					
		}		
	} 

	
	ShowClock_Dir(c1,c2,c3,c4,c5,c6);
	ShowWeather_Dir(w1,w2,w3,w4,w5,w6);	
}


void Nixie_Display::ScreenSaverStart(byte duration){
	_ScrSteps = (duration * 20)+1;
	_ScrStepCounter = _ScrSteps;
	ScreenSaverActive = 1;
	Leds_Off();
	  
	int pressure = Baro.Pressure;
	
	c1 = Time.Hour / 10;
	c2 = Time.Hour % 10;
	c3 = Time.Minute / 10;
	c4 = Time.Minute % 10;
	c5 = Time.Second / 10;
	c6 = Time.Second % 10;
	
	w1 = pressure / 10000;
	pressure = pressure % 10000;
	w2 = pressure / 1000;
	pressure = pressure % 1000;
	w3 = pressure / 100;
	pressure = pressure % 100;
	w4 = pressure / 10;
	pressure = pressure % 10;
	w5 = pressure;	
	w6 = 9;
}

byte Nixie_Display::Randomise(byte x){
	byte b = random(0, 9);
	if (b >= x){
		b++;
	}
	return b;
}





void Nixie_Display::Disp_Test(){
	byte bl_val = 0;
	unsigned long delay_start;
	
	
	HV_Off();
	ShowClock_Dir(11,11,11,11,11,11);
	ShowWeather_Dir(11,11,11,11,11,11);	
	HV_On();
	
	DimmerStart(255,7);
	
	while (DimActive > 0){
		DimmerPulse();
	}
	
	DimmerStart(0,7);
	DimmerPulse();
	
	for (byte x = 0; x < 10; x++) {
		ShowClock_Dir(x,x,x,x,x,x);
		ShowWeather_Dir(x,x,x,x,x,x);
		delay_start = millis();
		while (((millis() - delay_start) <= 500)|| (millis() < delay_start)){
			DimmerPulse();
		}
		if (DimStatus == 0){
			DimmerStart(DimIntensity,7);
		}
	}
}

