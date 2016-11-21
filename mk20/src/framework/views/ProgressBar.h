/*
 * ProgressBar implements a progress bar. Has some special handling that prevents
 * the view to update the screen for every single progress, instead the display is
 * only refreshed every second to keep draw overhead at a minimum even if we have
 * fast changes in the value (like downloading stuff)
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/printrbot-simple-2016-display-system-explained
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Phillip Schuster
 * https://github.com/Printrbot/Printrhub
 *
 * Developed in cooperation by Phillip Schuster (@appfruits) from appfruits.com
 * http://www.appfruits.com
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

#ifndef TEENSYCMAKE_PROGRESSBAR_H
#define TEENSYCMAKE_PROGRESSBAR_H

#include "View.h"
#include "../layers/RectangleLayer.h"

class ProgressBar : public View {
#pragma mark Constructor
 public:
  ProgressBar(Rect frame);
  ProgressBar(int x, int y, int width, int height);

#pragma mark Getter/Setter
  void setMinValue(float minValue);
  void setMaxValue(float maxValue);
  void setLimits(float minValue, float maxValue);
  float getValue();
  void setValue(float value);
  uint16_t getTrackColor() const { return _trackColor; }
  void setTrackColor(uint16_t trackColor) { _trackColor = trackColor; }
  void setBackgroundColor(uint16_t backgroundColor) {
	_backgroundColor = backgroundColor;
	_progressLayer->setBackgroundColor(_backgroundColor);
  }

#pragma mark View
  virtual String getDescription() override;
  virtual void setNeedsDisplay() override;

#pragma mark Member Functions
 private:
  Rect getLeftRect();
  Rect getRightRect();
  float getFraction();

#pragma mark Member Variables
 private:
  float _minValue;
  float _maxValue;
  float _value;
  RectangleLayer *_progressLayer;
  RectangleLayer *_trackLayer;
  uint16_t _trackColor;
  int _currentTrackWidth;
  unsigned long _lastUpdate = millis();

};

#endif //TEENSYCMAKE_PROGRESSBAR_H
