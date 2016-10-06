//
// Created by Phillip Schuster on 09.07.16.
//

#include "Timer.h"

Timer::Timer(uint32_t timeOut)
{
	_timeOut = timeOut;
}

void Timer::reset()
{
	_startTime = millis();
}

bool Timer::isTimedOut()
{
	if ((millis() - _startTime) > _timeOut)
	{
		return true;
	}

	return false;
}
