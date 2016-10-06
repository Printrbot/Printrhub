//
// Created by Phillip Schuster on 09.07.16.
//

#ifndef _TEENSYCMAKE_TIMER_H_
#define _TEENSYCMAKE_TIMER_H_

#include "Arduino.h"

class Timer
{
public:
	Timer(uint32_t timeOut);
	void reset();
	bool isTimedOut();

private:
	uint32_t _timeOut;
	uint32_t _startTime;
};


#endif //_TEENSYCMAKE_TIMER_H_
