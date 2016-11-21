/*
 * Animation class that makes it easy to interpolate a scalar value from source to target
 * over time using different easing functions. Used for scrolling animations.
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Phillip Schuster
 * https://github.com/Printrbot/Printrhub
 *
 * Developed in cooperation by Phillip Schuster (@appfruits) from appfruits.com
 * http://www.appfruits.com/printrhub
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "Animator.h"

AnimatorClass Animator;

AnimatorClass::AnimatorClass() {
  _lastUpdateTime = 0;
}

Animation *AnimatorClass::getAnimationSlot() {
  for (uint8_t i = 0; i < 10; i++) {
	if (!_animationSlots[i].isActive()) {
	  _animationSlots[i].reset();
	  return &_animationSlots[i];
	}
  }

  return NULL;
}

bool AnimatorClass::hasActiveAnimations() {
  for (uint8_t i = 0; i < 10; i++) {
	if (_animationSlots[i].isActive()) {
	  return true;
	}
  }

  return false;
}

void AnimatorClass::update() {
  if (_lastUpdateTime > 0) {
	unsigned long currentTime = micros();
	if (_lastUpdateTime < currentTime) {
	  if (currentTime - _lastUpdateTime < 5000) {
		return;
	  }
	} else {
	  //Just overflowed, don't check
	}
  }

  for (uint8_t i = 0; i < 10; i++) {
	if (_animationSlots[i].isActive()) {
	  _animationSlots[i].update();
	}
  }

  _lastUpdateTime = micros();
}