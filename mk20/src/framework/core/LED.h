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

#ifndef TEENSY_LED_H
#define TEENSY_LED_H

#include "Arduino.h"

enum LEDMode : uint8_t {
  LED_Off = 0,
  LED_On = 1,
  LED_Pulse = 2
};

class LED {
 public:
  LED(uint8_t pin);
  ~LED();

  void begin();
  void end();
  void setMode(LEDMode mode) { _mode = mode; };

  void on();
  void off();
  void blink() { pulse(0.5, true); };
  void pulse(float seconds, bool repeat);

  void loop();

 private:
  void update();

 private:
  uint8_t _pin;
  LEDMode _mode;
  unsigned long _pulseStart;
  float _pulseDuration;
  bool _pulseRepeat;
};

#endif //TEENSY_LED_H
