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

#ifndef _TEENSYCMAKE_ANIMATION_H_
#define _TEENSYCMAKE_ANIMATION_H_

#include "Arduino.h"

class AnimatableObject;

class Animation
{
public:
	Animation();
	Animation(String key);
	Animation(String key, float initialValue);
	Animation(String key, float initialValue, float targetValue);
	Animation(String key, float initialValue, float targetValue, float duration);

	void init(String key, float initialValue, float targetValue);
	void init(String key, float initialValue, float targetValue, float duration);
	void init(String key, float initialValue, float targetValue, float duration, float delay);

	void setDelay(float delay);
	void setDuration(float duration);
	void setInitialValue(float initialValue);
	void setTargetValue(float targetValue);
	void setKey(String key);
	void setObject(AnimatableObject* object);

	String getKey();

	bool isActive();
	void reset();
	void update();
	void start();
	void stop();

private:
	float _initialValue;
	float _currentValue;
	float _targetValue;
	float _duration;
	float _animationStart;
	float _delay;
	String _key;
	AnimatableObject* _object;
};

class AnimatableObject
{
public:
	virtual void animationUpdated(Animation *animation, float currentValue, float deltaValue, float timeLeft)
	{

	};

	virtual void animationFinished(Animation* animation)
	{

	};

	void addAnimation(Animation* animation)
	{
		animation->setObject(this);
		animation->start();
	};
};


#endif //_TEENSYCMAKE_ANIMATION_H_
