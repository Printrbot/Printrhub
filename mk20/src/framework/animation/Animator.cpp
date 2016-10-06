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

#include "Animator.h"

AnimatorClass Animator;

AnimatorClass::AnimatorClass()
{
	_lastUpdateTime = 0;
}

Animation* AnimatorClass::getAnimationSlot()
{
	for (uint8_t i=0;i<10;i++)
	{
		if (!_animationSlots[i].isActive())
		{
			_animationSlots[i].reset();
			return &_animationSlots[i];
		}
	}

	return NULL;
}


bool AnimatorClass::hasActiveAnimations()
{
	for (uint8_t i = 0; i < 10; i++)
	{
		if (_animationSlots[i].isActive())
		{
			return true;
		}
	}

	return false;
}

void AnimatorClass::update()
{
	if (_lastUpdateTime > 0)
	{
		unsigned long currentTime = micros();
		if (_lastUpdateTime < currentTime)
		{
			if (currentTime - _lastUpdateTime < 5000)
			{
				return;
			}
		}
		else
		{
			//Just overflowed, don't check
		}
	}

	for (uint8_t i=0;i<10;i++)
	{
		if (_animationSlots[i].isActive())
		{
			_animationSlots[i].update();
		}
	}

	_lastUpdateTime = micros();
}