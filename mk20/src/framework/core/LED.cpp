/*
 * LED class that enables different kinds of actions animated over time. Has its
 * own loop function that is called from the main application loop.
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

#include "LED.h"

LED::LED(uint8_t pin) {
  _pin = pin;
  _mode = LED_Off;
}

LED::~LED() {
  end();
}

void LED::begin() {
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
}

void LED::end() {
  pinMode(_pin, INPUT);
}

void LED::on() {
  _mode = LED_On;
  update();
}

void LED::off() {
  _mode = LED_Off;
  update();
}

void LED::pulse(float seconds, bool repeat) {
  _mode = LED_Pulse;
  _pulseStart = 0;
  _pulseDuration = seconds;
  _pulseRepeat = repeat;
  update();
}

void LED::update() {
  float deltaTime = 0;
  unsigned long currentTime = millis();

  if (_mode == LED_On) {
	digitalWrite(_pin, HIGH);
  } else if (_mode == LED_Off) {
	digitalWrite(_pin, LOW);
  } else if (_mode == LED_Pulse) {
	if (_pulseStart == 0) {
	  digitalWrite(_pin, HIGH);
	  _pulseStart = currentTime;
	} else {
	  deltaTime = (float) (currentTime - _pulseStart) / 1000.0f;
	  if (deltaTime > _pulseDuration) {
		_pulseStart = currentTime;
		digitalWrite(_pin, !digitalRead(_pin));

		if (!_pulseRepeat) {
		  _mode = LED_Off;
		}
	  }
	}
  }
}

void LED::loop() {
  update();
}