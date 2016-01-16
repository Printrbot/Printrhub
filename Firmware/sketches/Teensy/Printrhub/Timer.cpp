/*
 * Little Helper -- your best friend in your lab
 * http://www.appfruits.com/littlehelper
 *
 * This software is part of the firmware running Little Helper. You may use, copy, 
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software 
 * but you must include this copyright notice and this permission in all copies or 
 * substantial portions of the Software.
 *
 * Appfruits invests time and resources to make this open source. Please support us
 * and the open-source hardware initiative.
 * 
 * Copyright 2014-2015 Phillip Schuster (@appfruits)
 * http://www.appfruits.com
 * 
 * MIT-License
 */

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