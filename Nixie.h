/*
   Morse.h - Library for flashing Morse code.
   Created by David A. Mellis, November 2, 2007.
   Released into the public domain.
 */
#ifndef NixieLib_h
#define NixieLib_h

#include "Arduino.h"

class Nixie_Display
{
	public:
		Nixie_Display(byte c_latchp, byte c_clockp, byte c_datap, byte w_latchp, byte w_clockp, byte w_datap, byte c_highp, byte c_lowp, byte w_dotp, byte c_minp, byte c_maxp, byte bl_pin, byte hv_pin);
		void ShowClock(byte hr, byte min, byte sec);
		void ShowDate(byte day, byte month, byte year);
		void ShowPressure(int press, byte mmled);
		void ShowTemp(byte sensor, int temp, byte mmled);
		void ShowHum(byte sensor, byte hum, byte mmled);
	
		void SetupPage(byte _page, int _val);
		void SetupClock(byte hr, byte min, byte sec, byte _pos, byte changed);
		void SetupDate(byte day, byte mon, byte year, byte _pos, byte changed);
		
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
		
		void HV_On();
		void HV_Off();
		
		void ScreenSaverStart(byte duration, byte hr, byte min, byte sec, int pressure);
		
		void ScreenSaverPulse();
		void ScreenSaverSecPulse(byte hr, byte min, byte sec, int pressure);
		
		void Disp_Test();

		void C_Blink_On();
		void C_Blink_Off();

		void Clock_Blink_On();
		void Clock_Blink_Off();
		
		void Date_Leds_On();
		void Date_Leds_Off();
		
		void Leds_On();
		void Leds_Off();
		
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
		void Randomise();
		byte _scrnsvr_stepcounter;
		byte _scrnsvr_step;
		byte _td;
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
		
		byte _DimTarget;
		byte _DimSpeed;
		byte _DimStrt;
		byte _DimEnd;
		byte _ScreenSaverCounter;
		byte _ScreenSaverDuration;
		byte _ScreenSaverPulse;
		byte _ScrStep;
		
		byte _Sec_Stat;
		byte _Setup_NoBlink;
		unsigned long _LastDim;
		unsigned long _LastScr;
		unsigned long _LastScrSub;
		
		byte c1,c2,c3,c4,c5,c6;
		byte w1,w2,w3,w4,w5,w6;
		byte ct1,ct2,ct3,ct4,ct5,ct6;
		byte wt1,wt2,wt3,wt4,wt5;
		
};


#endif