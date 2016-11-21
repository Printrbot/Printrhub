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

#ifndef TEENSYCMAKE_LABELBUTTON_H
#define TEENSYCMAKE_LABELBUTTON_H

#include "LabelView.h"
#include "Button.h"
#include "../layers/BitmapLayer.h"

class LabelButton : public LabelView, public Button {
#pragma mark Constructor
 public:
  virtual void setFrame(Rect frame, bool updateLayout = true) override;

  LabelButton(String text, Rect frame);
  LabelButton(String text, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
  ~LabelButton() {};

#pragma mark Getter/Setter
  virtual void setAlternateBackgroundColor(uint16_t color) { _alternateBackgroundColor = color; };
  virtual uint16_t getAlternateBackgroundColor() { return _alternateBackgroundColor; };;;;

  void setAlternateFont(const ILI9341_t3_font_t *font) { _alternateFont = font; };
  const ILI9341_t3_font_t *getAlternateFont() { return _alternateFont; };

  void setAlternateTextColor(uint16_t color) { _alternateTextColor = color; };
  uint16_t getAlternateTextColor() { return _alternateTextColor; };

  virtual void setIcon(const uint8_t *bitmap, uint16_t color, uint16_t width, uint16_t height);

#pragma mark Member Functions
 private:
  void updateButton(ButtonState buttonState);

#pragma mark View
 public:
  virtual bool touchDown(TS_Point &point) override;
  virtual bool touchUp(TS_Point &point) override;
  virtual void touchCancelled() override;
  virtual void setFont(const ILI9341_t3_font_t *font) override;
  virtual void display() override;

#pragma mark Member Variables
 protected:
  uint16_t _alternateBackgroundColor;
  const ILI9341_t3_font_t *_alternateFont;
  uint16_t _alternateTextColor;
  const uint8_t *_iconBitmap;
  BitmapLayer *_iconLayer;
  uint8_t _iconWidth;
  uint8_t _iconHeight;
  uint16_t _iconColor;
  RectangleLayer *_gapLayer;
};

#endif //TEENSYCMAKE_LABELBUTTON_H
