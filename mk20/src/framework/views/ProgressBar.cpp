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

#include "ProgressBar.h"
#include "../core/Application.h"

ProgressBar::ProgressBar(int x, int y, int width, int height) :
	View(x, y, width, height) {
  setMinValue(0);
  setMaxValue(1);
  _value = 0;
  _currentTrackWidth = 0;

  _backgroundColor = Application.getTheme()->getColor(ControlBackgroundColor);
  _trackColor = Application.getTheme()->getColor(HighlightBackgroundColor);

  _progressLayer = new RectangleLayer(getLeftRect());
  _progressLayer->setBackgroundColor(_backgroundColor);
  addLayer(_progressLayer);

  _trackLayer = new RectangleLayer(getRightRect());
  _trackLayer->setBackgroundColor(_trackColor);
  addLayer(_trackLayer);
}

ProgressBar::ProgressBar(Rect frame) :
	ProgressBar(frame.x, frame.y, frame.width, frame.height) {

}

String ProgressBar::getDescription() {
  return "Progressbar";
}

void ProgressBar::setMinValue(float minValue) {
  _minValue = minValue;
}

void ProgressBar::setMaxValue(float maxValue) {
  _maxValue = maxValue;
}

float ProgressBar::getFraction() {
  return (float) (_value - _minValue) / (float) (_maxValue - _minValue);
}

Rect ProgressBar::getLeftRect() {
  return Rect(_frame.x, _frame.y, _frame.width * getFraction(), _frame.height);
}

Rect ProgressBar::getRightRect() {
  Rect leftRect = getLeftRect();
  return Rect(leftRect.right() + 1, _frame.y, _frame.right() - leftRect.right() - 1, _frame.height);
}

float ProgressBar::getValue() {
  return _value;
}

void ProgressBar::setNeedsDisplay() {
  if (_progressLayer == NULL || _trackLayer == NULL) return;
  if ((millis() - _lastUpdate) < 1000) return;

  FLOW_SPAM("Drawing Progress Bar: %d", (int) (_value * 100));

  _progressLayer->setFrame(getLeftRect(), false);
  _trackLayer->setFrame(getRightRect(), false);

  _lastUpdate = millis();
}

void ProgressBar::setValue(float value) {
  if (isnan(value)) value = 0.0f;
  if (this->_value == value) return;
  if (value > _maxValue) value = _maxValue;
  if (value < _minValue) value = _minValue;

  this->_value = value;

  FLOW_SPAM("Setting Progress Bar Value: %d", (int) (_value * 100));

  //Only refresh if there is something to draw
  int trackWidth = (int) ((float) _frame.width * value);
  if (trackWidth != _currentTrackWidth && trackWidth > 0) {
	setNeedsDisplay();
  }
  _currentTrackWidth = trackWidth;
}

void ProgressBar::setLimits(float minValue, float maxValue) {
  setMinValue(minValue);
  setMaxValue(maxValue);
}
