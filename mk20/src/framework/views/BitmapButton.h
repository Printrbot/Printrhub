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

#ifndef TEENSYCMAKE_BITMAPBUTTON_H
#define TEENSYCMAKE_BITMAPBUTTON_H

#include "View.h"
#include "../layers/BitmapLayer.h"
#include "../layers/SDBitmapLayer.h"
#include "Button.h"
#include "../core/UIBitmap.h"

class BitmapButton;

class BitmapButton : public View, public Button {
#pragma mark Constructor
 public:
  BitmapButton(Rect frame);
  virtual ~BitmapButton();

#pragma mark Getter/Setter
  virtual void setBitmap(const uint8_t *bitmap, uint16_t width, uint16_t height);
  virtual void setBitmap(const uint16_t *bitmap, uint16_t width, uint16_t height);
  virtual void setBitmap(UIBitmap *bitmap);
  uint16_t getColor() const { return _color; }
  void setColor(uint16_t color) { _color = color; }
  virtual void setAlternateBackgroundColor(uint16_t color) { _alternateBackgroundColor = color; };
  virtual uint16_t getAlternateBackgroundColor() { return _alternateBackgroundColor; };;;;
  void setAlternateTextColor(uint16_t color) { _alternateTextColor = color; };
  uint16_t getAlternateTextColor() { return _alternateTextColor; };
  RectangleLayer *getBaseLayer() const { return _baseLayer; }
  void setBaseLayer(RectangleLayer *baseLayer) { _baseLayer = baseLayer; }
  BitmapLayer *getBitmapLayer() const { return _bitmapLayer; }
  void setBitmapLayer(BitmapLayer *bitmapLayer) { _bitmapLayer = bitmapLayer; }

#pragma mark View
  virtual void setFrame(Rect frame, bool updateLayout = true) override;
 private:
  void createBitmapFrame(Rect *frame, uint16_t width, uint16_t height);
#pragma mark Touch Handling
 private:
  virtual bool touchUp(TS_Point &point) override;
  virtual bool touchDown(TS_Point &point) override;
  virtual void touchCancelled() override;

#pragma mark Member Functions
  void updateButton(ButtonState buttonState);

#pragma mark Member Variables
 protected:
  Rect _originalFrame;
  Animation *_touchAnimation;
  uint16_t _color;
  uint16_t _alternateBackgroundColor;
  uint16_t _alternateTextColor;
  RectangleLayer *_baseLayer;
  BitmapLayer *_bitmapLayer;
  SDBitmapLayer *_sdbitmapLayer;
};

#endif //TEENSYCMAKE_BITMAPBUTTON_H
