/*
   Nixie.h - Library for controlling nixie display.
   Created by R. de Weerd, October, 2016.
 */
#ifndef Nixie_h
#define Nixie_h

#include "Arduino.h"
#include "Nixie_Sensor.h"
#include "TimeLib.h"

#define PulsesPerSec 20        				// total pulses send to the display every seccond, used for blinking and screensaver
#define PulseInterval 1000/PulsesPerSec

class Nixie_Display
{
	public:
		Nixie_Display(byte c_latchp, byte c_clockp, byte c_datap, byte w_latchp, byte w_clockp, byte w_datap, byte c_highp, byte c_lowp, byte w_dotp, byte c_minp, byte c_maxp, byte bl_pin, byte hv_pin);

		void HV_On();
		void HV_Off();
		
		void Date_Leds_On();
		void Date_Leds_Off();
		
		void Leds_On();
		void Leds_Off();
		
		void ShowTime();
		void ShowDate();
		void ShowPressure();
		void ShowTemp(byte sensor);
		void ShowHum(byte sensor);
		
		void ShowCodeVersion(long ver);
		
		void SetupClock(byte _pos,  byte chng);
		void SetupPage(byte _page, int _val);
		void SetupPage(byte _page, byte _val);
		void ExitSetup();
			
		void ScreenSaverStart(byte duration);
		
		RF_Sensor RFSensor[3];
		Baro_Sensor Baro;
		TimeElements Time;

		byte BackLightDay;
		byte BackLightNight;
		
		//======================================================== ^^^^^ Verified ^^^^^ ========================================================
		
		
		
		void Pulse();
		
		void SetMinMaxLed(byte MinMaxL);
		
		void ShowWeather(byte b1, byte b2, byte b3, byte b4, byte b5, byte s);
		
		void ShowClock_Dir(byte d1, byte d2, byte d3, byte d4, byte d5, byte d6);
		void ShowWeather_Dir(byte d1, byte d2, byte d3, byte d4, byte d5, byte d6);
		
		void DimmerStart(byte target, byte speed);		
		void DimmerPulse();
		
		void SecPulse(byte _pulse);
		
		void NightmodeStart();
		void NightmodeWake();
		void NightmodeEnd();
		

		
		
		
		//void ScreenSaverSecPulse(byte hr, byte min, byte sec, int pressure);
		
		void Disp_Test();

		void C_Blink_On();
		void C_Blink_Off();

		void Clock_Blink_On();
		void Clock_Blink_Off();
		

		
		byte PulseCount;
		
		byte NightMode;
		
		byte day;
		byte month;
		int	year;
		
		byte DimStatus;
		byte DimActive;
		byte ScreenSaverActive;
		byte ScreenSaverFinished;
		byte DimIntensity;
		

		
	private:

		void ScreenSaverPulse();
		byte Randomise(byte x);

		
		byte _C_LatchPin;
		byte _C_ClockPin;
		byte _C_DataPin;
		byte _W_LatchPin;
		byte _W_ClockPin;
		byte _W_DataPin;
		byte _C_HighPin;
		byte _C_LowPin;
		byte _W_DotPin;
		byte _W_MinPin;
		byte _W_MaxPin;
		byte _BL_Pin;
		byte _HV_Pin;
	
		byte _TimeDisplay;
		byte _BlinkPos;
		byte _ValChanged;
		
		byte _lastPulse;

		int _ScrSteps;
		int _ScrStepCounter;

		byte c1,c2,c3,c4,c5,c6;
		byte w1,w2,w3,w4,w5,w6;


		
		//======================================================== ^^^^^ Verified ^^^^^ ========================================================

		
		byte _DimTarget;
		byte _DimSpeed;
		byte _DimStrt;
		byte _DimEnd;
		byte _ScreenSaverCounter;
		byte _ScreenSaverDuration;
		byte _ScreenSaverPulse;
		
		
		byte _Sec_Stat;
		byte _Setup_NoBlink;
		unsigned long _LastDim;
		unsigned long _LastScr;
		unsigned long _LastScrSub;
		
		
};


#endif