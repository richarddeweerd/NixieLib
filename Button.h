#ifndef Button_h
#define Button_h

#include "Arduino.h"

class Button
{
  public:
    Button(byte pin, byte pullup, int debounce);
    void check();
	void block();
	bool hold(int _holdtime);
	bool status();
	bool status(int _time);
	bool status(int _time, int _time2);
	bool backoff_act;
	
  private:
    byte _pin;
	byte _debounce;
	byte _laststate;
	
	byte _pressed;
	byte _blocked;
	
	unsigned long _press_start;

	unsigned long _backoff_start;
	int _backoff_time;
	byte _first_backoff;
	

};

#endif