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

//Formulas taken from http://gizma.com/easing/
class AnimationCurve
{
public:
	static float linear(float t, float b, float c, float d) {
		return c*t/d + b;
	}
	static float easeInQuad(float t, float b, float c, float d) {
			t /= d;
			return c*t*t + b;
	};
	static float easeOutQuad(float t, float b, float c, float d) {
		t /= d;
		return -c * t*(t-2) + b;
	};
	static float easeInOutQuad(float t, float b, float c, float d) {
		t /= d/2;
		if (t < 1) return c/2*t*t + b;
		t--;
		return -c/2 * (t*(t-2) - 1) + b;
	}
	static float easeInCubic(float t, float b, float c, float d) {
		t /= d;
		return c*t*t*t + b;
	};
	static float easeOutCubic(float t, float b, float c, float d) {
		t /= d;
		t--;
		return c*(t*t*t + 1) + b;
	};
	static float easeInOutCubic(float t, float b, float c, float d) {
		t /= d/2;
		if (t < 1) return c/2*t*t*t + b;
		t -= 2;
		return c/2*(t*t*t + 2) + b;
	}
	static float easeInQuart(float t, float b, float c, float d) {
		t /= d;
		return c*t*t*t*t + b;
	};
	static float easeOutQuart(float t, float b, float c, float d) {
		t /= d;
		t--;
		return -c * (t*t*t*t - 1) + b;
	};
	static float easeInOutQuart(float t, float b, float c, float d) {
		t /= d/2;
		if (t < 1) return c/2*t*t*t*t + b;
		t -= 2;
		return -c/2 * (t*t*t*t - 2) + b;
	}
	static float easeInQuint(float t, float b, float c, float d) {
		t /= d;
		return c*t*t*t*t*t + b;
	};
	static float easeOutQuint(float t, float b, float c, float d) {
		t /= d;
		t--;
		return c*(t*t*t*t*t + 1) + b;
	};
	static float easeInOutQuint(float t, float b, float c, float d) {
		t /= d/2;
		if (t < 1) return c/2*t*t*t*t*t + b;
		t -= 2;
		return c/2*(t*t*t*t*t + 2) + b;
	}
	static float easeInSine(float t, float b, float c, float d) {
		return -c/2 * (cosf(M_PI*t/d) - 1) + b;
	};
	static float easeOutSine(float t, float b, float c, float d) {
		return c * sinf(t/d * (M_PI/2)) + b;
	};
	static float easeInOutSine(float t, float b, float c, float d) {
		return -c/2 * (cosf(M_PI*t/d) - 1) + b;
	}
	static float easeInExpo(float t, float b, float c, float d) {
		return c * powf( 2, 10 * (t/d - 1) ) + b;
	};
	static float easeOutExpo(float t, float b, float c, float d) {
		return c * ( -powf( 2, -10 * t/d ) + 1 ) + b;
	};
	static float easeInOutExpo(float t, float b, float c, float d) {
		t /= d/2;
		if (t < 1) return c/2 * powf( 2, 10 * (t - 1) ) + b;
		t--;
		return c/2 * ( -powf( 2, -10 * t) + 2 ) + b;
	}
	static float easeInCirc(float t, float b, float c, float d) {
		t /= d;
		return -c * (sqrtf(1 - t*t) - 1) + b;
	};
	static float easeOutCirc(float t, float b, float c, float d) {
		t /= d;
		t--;
		return c * sqrtf(1 - t*t) + b;
	};
	static float easeInOutCirc(float t, float b, float c, float d) {
		t /= d/2;
		if (t < 1) return -c/2 * (sqrtf(1 - t*t) - 1) + b;
		t -= 2;
		return c/2 * (sqrtf(1 - t*t) + 1) + b;
	}
};


#endif //_TEENSYCMAKE_ANIMATION_H_
