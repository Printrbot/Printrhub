/*
 * LabelButton draws a button with text and color and handles button presses and
 * also draws some user feedback when button is pressed
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

#include "LabelButton.h"
#include "../../framework/layers/BitmapLayer.h"

LabelButton::LabelButton(String text, Rect frame) :
	LabelView(text, frame) {
  _name = "LabelButton";
  _delegate = NULL;
  _state = ButtonState::Off;
  _type = ButtonType::Push;
  _alternateFont = NULL;
  _borderWidth = 0;
  _iconBitmap = NULL;
  _gapLayer = NULL;
  _iconLayer = NULL;

  _backgroundColor = Application.getTheme()->getColor(ControlBackgroundColor);
  _alternateBackgroundColor = Application.getTheme()->getColor(ControlAlternateBackgroundColor);
  _alternateTextColor = Application.getTheme()->getColor(ControlAlternateTextColor);
  setTextColor(Application.getTheme()->getColor(ControlTextColor));
}

LabelButton::LabelButton(String text, uint16_t x, uint16_t y, uint16_t width, uint16_t height) :
	LabelView(text, x, y, width, height) {
  _name = "LabelButton";
  _delegate = NULL;
  _state = ButtonState::Off;
  _type = ButtonType::Push;
  _alternateFont = NULL;
  _borderWidth = 0;
  _iconBitmap = NULL;
  _gapLayer = NULL;
  _iconLayer = NULL;

  _backgroundColor = Application.getTheme()->getColor(ControlBackgroundColor);
  _alternateBackgroundColor = Application.getTheme()->getColor(ControlAlternateBackgroundColor);
  _alternateTextColor = Application.getTheme()->getColor(ControlAlternateTextColor);
  setTextColor(Application.getTheme()->getColor(ControlTextColor));
}

void LabelButton::updateButton(ButtonState buttonState) {
  if (buttonState == ButtonState::Off) {
	getTextLayer()->setBackgroundColor(getBackgroundColor());
	setFont(getFont());
	getTextLayer()->setForegroundColor(getTextColor());

	if (_gapLayer) {
	  _gapLayer->setBackgroundColor(getBackgroundColor());
	  _iconLayer->setBackgroundColor(getBackgroundColor());
	}
  } else if (buttonState == ButtonState::On) {
	getTextLayer()->setBackgroundColor(getAlternateBackgroundColor());
	getTextLayer()->setForegroundColor(getAlternateTextColor());
	if (_type != ButtonType::Toggle) {
	  if (getAlternateFont() != NULL) {
		getTextLayer()->setFont(getAlternateFont());
	  }
	} else {
	  //Don't scale text if Button is a toogle (looks strange)
	  getTextLayer()->setFont(getFont());
	}

	if (_gapLayer) {
	  _gapLayer->setBackgroundColor(getAlternateBackgroundColor());
	  _iconLayer->setBackgroundColor(getAlternateBackgroundColor());
	}
  }

  setNeedsDisplay();
}

bool LabelButton::touchDown(TS_Point &point) {
  updateButton(ButtonState::On);

  return true;
}

bool LabelButton::touchUp(TS_Point &point) {
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

void LabelButton::touchCancelled() {
  updateButton(ButtonState::Off);
}

void LabelButton::setFont(const ILI9341_t3_font_t *font) {
  LabelView::setFont(font);

  if (_alternateFont == NULL) {
	_alternateFont = font;
  }
}

void LabelButton::display() {
  if (_iconBitmap == NULL) {
	_layer = new TextLayer(_frame);
	_layer->setForegroundColor(getTextColor());
	_layer->setBackgroundColor(getBackgroundColor());
	_layer->setText(getText());
	_layer->setFont(getFont());
	_layer->setTextAlign(getTextAlign());
	_layer->setVerticalTextAlign(getVerticalTextAlign());
	addLayer(_layer);
  } else {
	uint16_t inset = 10;    //Gap from left and right border
	uint32_t textWidth = Display.textWidth(getFont(), getText());

	uint32_t innerWidth = textWidth + _iconWidth + 10;  //10 Gap between icon and text
	uint32_t paddingHeight = (_frame.height - Display.fontHeight(getFont())) / 2;

	//TEXTALIGN_CENTERED default
	uint32_t paddingLeft = inset;  //Inset from the left border
	uint32_t paddingRight = _frame.width - paddingLeft - innerWidth - inset;
	if (getTextAlign() == TEXTALIGN_CENTERED) {
	  paddingLeft = (_frame.width - innerWidth) / 2;
	  paddingRight = paddingLeft;
	}

	_gapLayer = new RectangleLayer(Rect(_frame.x, _frame.y, paddingLeft + _iconWidth + 10, _frame.height));
	_gapLayer->setBackgroundColor(getBackgroundColor());

	_iconLayer = new BitmapLayer(Rect(_frame.x + paddingLeft, _frame.y + paddingHeight, _iconWidth, _iconHeight));
	_gapLayer->splitWithRect(_iconLayer->getFrame());
	_iconLayer->setBitmap(_iconBitmap, _iconWidth, _iconHeight);
	_iconLayer->setBackgroundColor(getBackgroundColor());
	_iconLayer->setColor(_iconColor);

	_layer = new TextLayer(Rect(
		_frame.x + paddingLeft + _iconWidth + 10, _frame.y, textWidth + paddingRight, _frame.height));
	_layer->setForegroundColor(getTextColor());
	_layer->setBackgroundColor(getBackgroundColor());
	_layer->setText(getText());
	_layer->setFont(getFont());
	_layer->setTextAlign(TEXTALIGN_LEFT);
	_layer->setVerticalTextAlign(getVerticalTextAlign());

	addLayer(_layer);
	addLayer(_gapLayer);
	addLayer(_iconLayer);
  }

  View::display();

  updateButton(_state);
}

void LabelButton::setFrame(Rect frame, bool updateLayout) {
  View::setFrame(frame, updateLayout);

  if (_iconBitmap == NULL) {
	_layer->setFrame(frame, updateLayout);
  } else {
	uint16_t inset = 10;    //Gap from left and right border
	uint32_t textWidth = Display.textWidth(getFont(), getText());

	uint32_t innerWidth = textWidth + _iconWidth + 10;  //10 Gap between icon and text
	uint32_t paddingHeight = (_frame.height - Display.fontHeight(getFont())) / 2;

	//TEXTALIGN_CENTERED default
	uint32_t paddingLeft = inset;  //Inset from the left border
	uint32_t paddingRight = _frame.width - paddingLeft - innerWidth - inset;
	if (getTextAlign() == TEXTALIGN_CENTERED) {
	  paddingLeft = (_frame.width - innerWidth) / 2;
	  paddingRight = paddingLeft;
	}

	_gapLayer->setFrame(Rect(_frame.x, _frame.y, paddingLeft + _iconWidth + 10, _frame.height), updateLayout);
	_iconLayer->setFrame(Rect(_frame.x + paddingLeft, _frame.y + paddingHeight, _iconWidth, _iconHeight), updateLayout);
	_layer->setFrame(Rect(
		_frame.x + paddingLeft + _iconWidth + 10, _frame.y, textWidth + paddingRight, _frame.height), updateLayout);
  }
}

void LabelButton::setIcon(const uint8_t *bitmap, uint16_t color, uint16_t width, uint16_t height) {
  _iconBitmap = bitmap;
  _iconWidth = width;
  _iconHeight = height;
  _iconColor = color;
}

