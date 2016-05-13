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

#include "Animation.h"

Animation::Animation()
{
	reset();
}

Animation::Animation(String key)
{
	reset();
	setKey(key);
}

void Animation::reset()
{
	_initialValue = 0;
	_currentValue = 0;
	_targetValue = 0;
	_duration = 0.25;
	_delay = 0;
	_animationStart = 0;
	_key = "";
	_object = NULL;
}

Animation::Animation(String key, float initialValue) : Animation(key)
{
	setInitialValue(initialValue);
}

Animation::Animation(String key, float initialValue, float targetValue): Animation(key,initialValue)
{
	setTargetValue(targetValue);
}

Animation::Animation(String key, float initialValue, float targetValue, float duration): Animation(key,initialValue,targetValue)
{
	setDuration(duration);
}

void Animation::init(String key, float initialValue, float targetValue, float duration)
{
	setKey(key);
	setInitialValue(initialValue);
	setTargetValue(targetValue);
	setDuration(duration);
}

void Animation::init(String key, float initialValue, float targetValue, float duration, float delay)
{
	setKey(key);
	setInitialValue(initialValue);
	setTargetValue(targetValue);
	setDuration(duration);
	setDelay(delay);
}

void Animation::init(String key, float initialValue, float targetValue)
{
	setKey(key);
	setInitialValue(initialValue);
	setTargetValue(targetValue);
}

void Animation::setDuration(float duration)
{
	_duration = duration;
}

void Animation::setDelay(float delay)
{
	_delay = delay;
}

void Animation::setInitialValue(float initialValue)
{
	_initialValue = initialValue;
	_currentValue = _initialValue;
}

void Animation::setTargetValue(float targetValue)
{
	_targetValue = targetValue;
}

bool Animation::isActive()
{
	return (_animationStart != 0);
}

void Animation::setKey(String key)
{
	_key = key;
}

void Animation::setObject(AnimatableObject *object)
{
	_object = object;
}

void Animation::start()
{
	_animationStart = (float)(millis()/1000.0f) + _delay;
}


void Animation::stop()
{
	_animationStart = 0;
}

void Animation::update()
{
	if (_animationStart == 0) return;
	if (_targetValue == _initialValue) return;
	if (_object == NULL) return;

	uint32_t currentTime = millis();
	float difference = (float)(currentTime/1000.0f) - _animationStart;
	if (difference < 0)
	{
		//Animation did not start yet (waiting for delay)
		return;
	}

	float oldValue = _currentValue;
	if (difference >= _duration)
	{
		_currentValue = _targetValue;
		_animationStart = 0;
	}
	else
	{
		_currentValue = AnimationCurve::easeOutQuad(difference,_initialValue,_targetValue-_initialValue,_duration);
	}

	_object->animationUpdated(this, _currentValue, _currentValue - oldValue, (_duration - difference));

	//If the animation finished in this iteration
	if (_animationStart == 0)
	{
		_object->animationFinished(this);
	}
}

String Animation::getKey()
{
	return _key;
}