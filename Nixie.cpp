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
	DimStatus = 0;
	NightMode = 0;
	
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
	
	digitalWrite(_HV_Pin, LOW);
	analogWrite(_BL_Pin,DimStatus);
	
	digitalWrite(_C_HighPin, HIGH);
	digitalWrite(_C_LowPin, HIGH);
	digitalWrite(_W_DotPin, HIGH);
	digitalWrite(_W_MinPin, HIGH);
	digitalWrite(_W_MaxPin, HIGH);
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
	
void Nixie_Display::ShowClock(byte hr, byte min, byte sec){
    _td = 0;
	
	byte byte1;
	byte byte2;
	byte byte3;
	
	byte1 = (hr / 10) << 4;
	byte1 = byte1 | (hr % 10);

	byte2 = (min / 10) << 4;
	byte2 = byte2 | (min % 10);
	
	byte3 = (sec / 10) << 4;
	byte3 = byte3 | (sec % 10);
	
	digitalWrite(_C_LatchPin, LOW);
	shiftOut(_C_DataPin, _C_ClockPin, MSBFIRST, byte1); 
	shiftOut(_C_DataPin, _C_ClockPin, MSBFIRST, byte2); 
	shiftOut(_C_DataPin, _C_ClockPin, MSBFIRST, byte3); 	
	digitalWrite(_C_LatchPin, HIGH);
	
	if (_Sec_Stat <= 1){
		Clock_Blink_On();
	} else {
		Clock_Blink_Off();
	}
}

void Nixie_Display::ShowDate(byte day, byte month, byte year){
    _td = 1;
	
	byte byte1;
	byte byte2;
	byte byte3;
	byte yr = (byte) (year - 30);
	
	byte1 = (day / 10) << 4;
	byte1 = byte1 | (day % 10);

	byte2 = (month / 10) << 4;
	byte2 = byte2 | (month % 10);
	
	byte3 = (yr / 10) << 4;
	byte3 = byte3 | (yr % 10);
	
	digitalWrite(_C_LatchPin, LOW);
	shiftOut(_C_DataPin, _C_ClockPin, MSBFIRST, byte1); 
	shiftOut(_C_DataPin, _C_ClockPin, MSBFIRST, byte2); 
	shiftOut(_C_DataPin, _C_ClockPin, MSBFIRST, byte3); 	
	digitalWrite(_C_LatchPin, HIGH);
	
	Date_Leds_On();
}

		
void Nixie_Display::ShowTemp(byte sensor, int temp, byte mmled){
	byte byte1;
	byte byte2;
	byte byte3;
	
	byte b1,b2,b3,s;

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
	
	SetMinMaxLed(mmled);
	
	digitalWrite(_W_DotPin, LOW); 
	
	digitalWrite(_W_LatchPin, LOW);
	shiftOut(_W_DataPin, _W_ClockPin, MSBFIRST, byte1); 
	shiftOut(_W_DataPin, _W_ClockPin, MSBFIRST, byte2); 
	shiftOut(_W_DataPin, _W_ClockPin, MSBFIRST, byte3); 	
	digitalWrite(_W_LatchPin, HIGH);
}

void Nixie_Display::ShowHum(byte sensor, byte hum, byte mmled){
	byte byte1;
	byte byte2;
	byte byte3;
	
	byte b1,b2;
	
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
	
	byte3 = 0 << 4;
	byte3 = byte3 | 2 ; // 2 = % sign
	
	SetMinMaxLed(mmled);
	
	digitalWrite(_W_DotPin, LOW); 
	
	digitalWrite(_W_LatchPin, LOW);
	shiftOut(_W_DataPin, _W_ClockPin, MSBFIRST, byte1); 
	shiftOut(_W_DataPin, _W_ClockPin, MSBFIRST, byte2); 
	shiftOut(_W_DataPin, _W_ClockPin, MSBFIRST, byte3); 	
	digitalWrite(_W_LatchPin, HIGH);
}
		
		
		

void Nixie_Display::ShowPressure(int press, byte mmled){
	byte byte1;
	byte byte2;
	byte byte3;
	
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
		
	
	SetMinMaxLed(mmled);
	
	digitalWrite(_W_DotPin, LOW); 
	digitalWrite(_W_LatchPin, LOW);
	shiftOut(_W_DataPin, _W_ClockPin, MSBFIRST, byte1); 
	shiftOut(_W_DataPin, _W_ClockPin, MSBFIRST, byte2); 
	shiftOut(_W_DataPin, _W_ClockPin, MSBFIRST, byte3); 	
	digitalWrite(_W_LatchPin, HIGH);
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

void Nixie_Display::HV_On(){
	digitalWrite(_HV_Pin, HIGH);
}

void Nixie_Display::HV_Off(){
	digitalWrite(_HV_Pin, LOW);
}


void Nixie_Display::NightmodeStart(){
  NightMode = 1;
  HV_Off();
  DimmerStart(10,25);
  Leds_Off();
}

void Nixie_Display::NightmodeWake(){
}

void Nixie_Display::NightmodeEnd(){
  NightMode = 0;
  HV_On();
  DimmerStart(DimIntensity,25);
}

void Nixie_Display::C_Blink_On(){
	if (NightMode != 1){
		switch (_td){
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

void Nixie_Display::Date_Leds_On(){
	digitalWrite(_C_HighPin, HIGH);
	digitalWrite(_C_LowPin, LOW);
}
void Nixie_Display::Date_Leds_Off(){
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

void Nixie_Display::ScreenSaverStart(byte duration, byte hr, byte min, byte sec, int pressure){
	
	ct1 = hr / 10;
	ct2 = hr % 10;
	ct3 = min / 10;
	ct4 = min % 10;
	ct5 = sec / 10;
	ct6 = sec % 10;
	
	wt1 = pressure / 10000;
	pressure = pressure % 10000;
	wt2 = pressure / 1000;
	pressure = pressure % 1000;
	wt3 = pressure / 100;
	pressure = pressure % 100;
	wt4 = pressure / 10;
	pressure = pressure % 10;
	wt5 = pressure;	
	
	//ShowClock_Dir(11,11,11,11,11,11);
	//ShowWeather_Dir(11,11,11,11,11,11);	
	DimmerStart(0,8);
	_ScreenSaverDuration = duration;
	ScreenSaverActive = 1;
	_ScreenSaverCounter = 0;
	_LastScr = millis();
	_LastScrSub = millis();
	_ScrStep = 0;
	
	_td = 2;
}

void Nixie_Display::Randomise(){
	c1=random(0, 10);
	c2=random(0, 10);
	c3=random(0, 10);
	c4=random(0, 10);
	c5=random(0, 10);
	c6=random(0, 10);
	w1=random(0, 10);
	w2=random(0, 10);
	w3=random(0, 10);
	w4=random(0, 10);
	w5=random(0, 10);
	w6=random(0, 10);
}

void Nixie_Display::ScreenSaverPulse(){

	if (((millis() - _LastScr) >= 50)|| (millis() < _LastScr)){
		_LastScr += 50;
		Randomise();
		switch (ScreenSaverActive){
			case 1:
				_ScrStep++;
				break;
			case 2:
				_ScrStep = 12;
				break;
			case 3:
				if (_ScrStep > 0) {
					_ScrStep--;
				} else {
					_td = 0;
				}
				break;
		}
	}
	

	if (_ScreenSaverCounter < 4){
		//Begin of screensaver
		ScreenSaverActive = 1;
		_DimStrt=1;
		_DimEnd=0;
	} else {
		if ((_ScreenSaverDuration - _ScreenSaverCounter) < 3){
			//End of screensaver
			if (ScreenSaverActive == 2){
				DimmerStart(DimIntensity,8);
				ScreenSaverActive = 3;
				//_LastScr = millis();
				_ScrStep = 12;	
			}
		} else {
			if (((_ScreenSaverDuration - _ScreenSaverCounter) < 5)&&(_DimEnd==1)){
				DimmerStart(0,3);
				_DimEnd=0;
			} else {
				
				if ((DimStatus == 0)&&(_DimStrt==1)){
					DimmerStart(DimIntensity,3);
					_DimStrt=0;
					_DimEnd=1;
				}
				if ((DimStatus >= DimIntensity)&&(_DimStrt==0)){
					DimmerStart(0,3);
					_DimStrt=1;
				}
			}
			ScreenSaverActive = 2;
		}
	} 
	
	switch (_ScrStep){
	  case 0:
		c1=ct1;
		c2=ct2;
		c3=ct3;
		c4=ct4;
		c5=ct5;
		c6=ct6;
		w1=wt1;
		w2=wt2;
		w3=wt3;
		w4=wt4;
		w5=wt5;
		w6=9;
		break;
	  case 1:
		c1=ct1;
		c2=ct2;
		c3=ct3;
		c4=ct4;
		c5=ct5;
		c6=11;
		w1=11;
		w2=wt2;
		w3=wt3;
		w4=wt4;
		w5=wt5;
		w6=9;
		break; 
	  case 2:
		c1=ct1;
		c2=ct2;
		c3=ct3;
		c4=ct4;
		c5=11;
		c6=11;
		w1=11;
		w2=11;
		w3=wt3;
		w4=wt4;
		w5=wt5;
		w6=9;
		break;
	  case 3:
		c1=ct1;
		c2=ct2;
		c3=ct3;
		c4=11;
		c5=11;
		c6=11;
		w1=11;
		w2=11;
		w3=11;
		w4=wt4;
		w5=wt5;
		w6=9;
		digitalWrite(_W_DotPin, LOW);
		break; 
	  case 4:
		c1=ct1;
		c2=ct2;
		c3=11;
		c4=11;
		c5=11;
		c6=11;
		w1=11;
		w2=11;
		w3=11;
		w4=11;
		w5=wt5;
		w6=9;
		digitalWrite(_W_DotPin, HIGH);
		break;
	  case 5:
		c1=ct1;
		c2=11;
		c3=11;
		c4=11;
		c5=11;
		c6=11;
		w1=11;
		w2=11;
		w3=11;
		w4=11;
		w5=11;
		w6=9;
		break; 
	  case 6:
		c1=11;
		c2=11;
		c3=11;
		c4=11;
		c5=11;
		c6=11;
		w1=11;
		w2=11;
		w3=11;
		w4=11;
		w5=11;
		w6=11;
		break;
	  case 7:
		c1=11;
		c2=11;
		c3=11;
		c4=11;
		c5=11;
		w2=11;
		w3=11;
		w4=11;
		w5=11;
		w6=11;
		break;
	  case 8:
		c1=11;
		c2=11;
		c3=11;
		c4=11;
		w3=11;
		w4=11;
		w5=11;
		w6=11;
		break;
	  case 9:
		c1=11;
		c2=11;
		c3=11;
		w4=11;
		w5=11;
		w6=11;
		break;
	  case 10:
		c1=11;
		c2=11;
		w5=11;
		w6=11;
		break;
	  case 11:
		c1=11;
		w6=11;
		break;
	  default:
		break;
	} 
	ShowClock_Dir(c1,c2,c3,c4,c5,c6);
	ShowWeather_Dir(w1,w2,w3,w4,w5,w6);
	
	DimmerPulse();
	
	if (_ScreenSaverCounter == _ScreenSaverDuration + 1){
		ScreenSaverFinished = 1;
	}
}
void Nixie_Display::ScreenSaverSecPulse(byte hr, byte min, byte sec, int pressure){
	
	ct1 = hr / 10;
	ct2 = hr % 10;
	ct3 = min / 10;
	ct4 = min % 10;
	ct5 = sec / 10;
	ct6 = sec % 10;
	
	wt1 = pressure / 10000;
	pressure = pressure % 10000;
	wt2 = pressure / 1000;
	pressure = pressure % 1000;
	wt3 = pressure / 100;
	pressure = pressure % 100;
	wt4 = pressure / 10;
	pressure = pressure % 10;
	wt5 = pressure;	


	
	_ScreenSaverCounter++;
	_ScreenSaverPulse = 1;

}

void Nixie_Display::SetupPage(byte _page, int _val){
	ShowClock_Dir((_page / 10),(_page %10),11,11,11,11);
	ShowWeather_Dir(11,11,11,11,11,11);	
}

void Nixie_Display::SetupClock(byte hr, byte min, byte sec, byte _pos, byte _changed){
	byte byte1;
	byte byte2;
	byte byte3;

	if (_changed == 1) {
		_Setup_NoBlink = 5;
	}
	
	if ((_pos == 1) && ( _Sec_Stat <= 1) && (_Setup_NoBlink == 0)){
		byte1 = 0xBB;
	} else {
		byte1 = (hr / 10) << 4;
		byte1 = byte1 | (hr % 10);
	}
	if ((_pos == 2) && ( _Sec_Stat <= 1) && (_Setup_NoBlink == 0)){
		byte2 = 0xBB;
	} else {
		byte2 = (min / 10) << 4;
		byte2 = byte2 | (min % 10);
	}
	
	if ((_pos == 3) && ( _Sec_Stat <= 1) && (_Setup_NoBlink == 0)){
		byte3 = 0xBB;
	} else {
		byte3 = (sec / 10) << 4;
		byte3 = byte3 | (sec % 10);
	}
	
	digitalWrite(_C_LatchPin, LOW);
	shiftOut(_C_DataPin, _C_ClockPin, MSBFIRST, byte1); 
	shiftOut(_C_DataPin, _C_ClockPin, MSBFIRST, byte2); 
	shiftOut(_C_DataPin, _C_ClockPin, MSBFIRST, byte3); 	
	digitalWrite(_C_LatchPin, HIGH);	
	
	if (_Sec_Stat <= 1){
		Clock_Blink_On();
	} else {
		Clock_Blink_Off();
	}
}

void Nixie_Display::SetupDate(byte day, byte mon, byte year, byte _pos, byte _changed){
	byte byte1;
	byte byte2;
	byte byte3;
	byte yr = (byte) (year - 30);
	
	if (_changed == 1) {
		_Setup_NoBlink = 5;
	}
	
	if ((_pos == 1) && ( _Sec_Stat <= 1) && (_Setup_NoBlink == 0)){
		byte1 = 0xBB;
	} else {
		byte1 = (day / 10) << 4;
		byte1 = byte1 | (day % 10);
	}
	if ((_pos == 2) && ( _Sec_Stat <= 1) && (_Setup_NoBlink == 0)){
		byte2 = 0xBB;
	} else {
		byte2 = (mon / 10) << 4;
		byte2 = byte2 | (mon % 10);
	}
	
	if ((_pos == 3) && ( _Sec_Stat <= 1) && (_Setup_NoBlink == 0)){
		byte3 = 0xBB;
	} else {
		byte3 = (yr / 10) << 4;
		byte3 = byte3 | (yr % 10);
	}
	
	digitalWrite(_C_LatchPin, LOW);
	shiftOut(_C_DataPin, _C_ClockPin, MSBFIRST, byte1); 
	shiftOut(_C_DataPin, _C_ClockPin, MSBFIRST, byte2); 
	shiftOut(_C_DataPin, _C_ClockPin, MSBFIRST, byte3); 	
	digitalWrite(_C_LatchPin, HIGH);	
	
	if (_Sec_Stat <= 1){
		Date_Leds_On();
	} else {
		Date_Leds_Off();
	}
}


void Nixie_Display::Disp_Test(){
	byte bl_val = 0;
	unsigned long delay_start;
	
	ShowClock_Dir(11,11,11,11,11,11);
	ShowWeather_Dir(11,11,11,11,11,11);	

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

