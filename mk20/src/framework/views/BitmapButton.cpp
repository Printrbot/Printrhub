/*
 * BitmapButton handles a button that uses a memory (SRAM and program memory) for
 * bitmap storage. Implements the ButtonDelegate interface
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

#include "BitmapButton.h"
#include "../layers/BitmapLayer.h"
#include "../layers/SDBitmapLayer.h"
#include "../core/Application.h"
#include "../animation/Animator.h"
#include "../animation/Animation.h"

BitmapButton::BitmapButton(Rect frame) :
	View(frame),
	_originalFrame(frame),
	_touchAnimation(NULL),
	_baseLayer(NULL),
	_bitmapLayer(NULL) {
  _delegate = NULL;
  _backgroundColor = Application.getTheme()->getColor(ControlBackgroundColor);
  _color = Application.getTheme()->getColor(ControlTextColor);
  _alternateBackgroundColor = Application.getTheme()->getColor(ControlAlternateBackgroundColor);
  _alternateTextColor = Application.getTheme()->getColor(ControlAlternateTextColor);

  LOG("Creating Bitmap Button");
  LOG_VALUE("Frame", frame.toString());
  LOG_VALUE("This-Frame", _frame.toString());
}

BitmapButton::~BitmapButton() {}

void BitmapButton::createBitmapFrame(Rect *frame, uint16_t width, uint16_t height) {
  //The bitmap is smaller than the frame, create a layer and place the bitmap inside it
  if (frame->width > width || frame->height > height) {
	_baseLayer = new RectangleLayer(*frame);
	_baseLayer->setBackgroundColor(getBackgroundColor());
	_baseLayer->setContext(getContext());

	frame->x += (frame->width - width) / 2;
	frame->y += (frame->height - height) / 2;
	frame->width = width;
	frame->height = height;

	//Stamp a hole in the base layer where we place the BitmapLayer
	_baseLayer->splitWithRect(*frame);
	addLayer(_baseLayer);
  }
}

void BitmapButton::setBitmap(const uint16_t *bitmapRGB, uint16_t width, uint16_t height) {
  Rect bitmapFrame = _frame;
  createBitmapFrame(&bitmapFrame, width, height);
  _bitmapLayer = new BitmapLayer(bitmapFrame);
  _bitmapLayer->setBitmap(bitmapRGB, width, height);
  _bitmapLayer->setContext(getContext());
  addLayer(_bitmapLayer);
}

void BitmapButton::setBitmap(const uint8_t *bitmap, uint16_t width, uint16_t height) {
  Rect bitmapFrame = _frame;
  createBitmapFrame(&bitmapFrame, width, height);

  _bitmapLayer = new BitmapLayer(bitmapFrame);
  _bitmapLayer->setBitmap(bitmap, width, height);
  _bitmapLayer->setBackgroundColor(getBackgroundColor());
  _bitmapLayer->setColor(getColor());
  _bitmapLayer->setContext(getContext());
  addLayer(_bitmapLayer);
}

void BitmapButton::setBitmap(UIBitmap *bitmap) {
  _sdbitmapLayer = new SDBitmapLayer(_frame);
  _sdbitmapLayer->setBackgroundColor(getBackgroundColor());
  _sdbitmapLayer->setBitmap("ui.min", bitmap->width, bitmap->height, bitmap->offset);
  _sdbitmapLayer->setContext(getContext());
  addLayer(_sdbitmapLayer);
}

void BitmapButton::setFrame(Rect frame, bool updateLayout) {
  if (_frame == frame) return;

  View::setFrame(frame, updateLayout);
  _originalFrame = frame;

  _layers.at(0)->setFrame(frame, updateLayout);
}

void BitmapButton::updateButton(ButtonState buttonState) {
  if (_sdbitmapLayer == NULL) return;

  if (buttonState == ButtonState::Off) {
	_sdbitmapLayer->setShadowed(false);
  } else if (buttonState == ButtonState::On) {
	_sdbitmapLayer->setShadowed(true);
  }

  setNeedsDisplay();
}

bool BitmapButton::touchDown(TS_Point &point) {
  if (!this->isVisible()) return false;
  updateButton(ButtonState::On);

  return true;
}

bool BitmapButton::touchUp(TS_Point &point) {
  if (!this->isVisible()) return false;

  if (_type == ButtonType::Toggle) {
	_state = (ButtonState) !_state;
	updateButton(_state);
  } else {
	updateButton(ButtonState::Off);
  }

  if (_delegate != NULL) {
	_delegate->buttonPressed(this);
  }

  return true;
}

void BitmapButton::touchCancelled() {
  updateButton(ButtonState::Off);
}
