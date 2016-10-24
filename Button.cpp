#include "Arduino.h"
#include "Button.h"

Button::Button(byte pin, byte pullup, int debounce){
	_pin = pin;
	_debounce = debounce;
	_laststate = 0;
	_first_backoff = 0;
	//_stat = 0;
	
	if (pullup == 1){
		pinMode(pin, INPUT_PULLUP);
	} else {
		pinMode(pin, INPUT);
	}
}

bool Button::hold(int _holdtime){
	if ((_pressed == 1) && (!backoff_act)){
		if (millis() - _press_start >= _holdtime){
			return true;	
		} else {
			return false;	
		}
	} else {
		return false;	
	}
}


bool Button::status(){
	if ((_pressed == 1) && (!backoff_act)){
		return true;	
	} else {
		return false;	
	}
}

bool Button::status(int _time){
	if ((_pressed == 1) && (!backoff_act)){
		_backoff_start = millis();
		_backoff_time = _time;
		backoff_act = true;
		return true;	
	} else {
		return false;	
	}
}

bool Button::status(int _time, int _time2){
	if ((_pressed == 1) && (!backoff_act)){
		
		_backoff_start = millis();
		if (_first_backoff == 0){
			_backoff_time = _time;
		} else {
			_backoff_time = _time2;
		}		
		_first_backoff = 1;
		backoff_act = true;
		
		return true;	
	} else {
		return false;	
	}
}

void Button::block(){
	_pressed = false;
	_blocked = 1;
}

void Button::check(){
	unsigned long _t;
		
	if (digitalRead(_pin) == LOW){
		//button is pressed
		_t = millis();
		
		if (_laststate == 0){
			//press starts
			_press_start = _t;
			_laststate = 1;
			_blocked = 0;
			backoff_act = false;
			_first_backoff = 0;
		} else {
			//press is ongoing			
			if (_t < _press_start){
				// rollover of millis
				_press_start = _t;
				_backoff_start = _t;
			}
			if (backoff_act){
				if (_t - _backoff_start >= _backoff_time){
					backoff_act = false;
				}
			}
			if ((_t - _press_start) >= _debounce){
				//debounce has passed
				//press accepted
				if (_blocked == 0){
					_pressed = 1;	
				}				
			}			
		}
	} else {
		//button is not pressed
		_pressed = 0;

		if (_laststate == 1){
			//bounce ignore
			_laststate = 0;
		}	
	}
}





