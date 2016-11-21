/*
 * View that wraps the TextLayer
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

#ifndef TEENSYCMAKE_LABELVIEW_H
#define TEENSYCMAKE_LABELVIEW_H

#include "View.h"
#include "../core/Application.h"
#include "../layers/TextLayer.h"

#define TEXTALIGN_LEFT 0
#define TEXTALIGN_CENTERED 1
#define TEXTALIGN_RIGHT 2
#define TEXTALIGN_TOP 0
#define TEXTALIGN_BOTTOM 2

class LabelView : public View {
#pragma mark Constructor:
 public:
  LabelView(String text, Rect frame);
  LabelView(String text, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

#pragma mark Getter/Setter
  void setText(String text);
  String getText() const { return _text; };
  void setTextAlign(uint8_t textAlign);
  uint8_t getTextAlign() const { return _textAlign; };
  void setVerticalTextAlign(uint8_t verticalTextAlign);
  uint8_t getVerticalTextAlign() const { return _verticalTextAlign; };
  void setTextColor(uint16_t color);
  uint16_t getTextColor() { return _textColor; };
  virtual void setFont(const ILI9341_t3_font_t *font);
  const ILI9341_t3_font_t *getFont();
  TextLayer *getTextLayer() { return _layer; };

#pragma mark View
  virtual bool touchDown(TS_Point &point) { return View::touchDown(point); };
  virtual bool touchUp(TS_Point &point) { return View::touchUp(point); };
  virtual void touchCancelled() { View::touchCancelled(); }
  virtual void display() override;

#pragma mark Member Variables
 protected:
  TextLayer *_layer;
 private:
  String _text;
  uint8_t _textAlign;
  uint8_t _verticalTextAlign;
  const ILI9341_t3_font_t *_font;
  uint16_t _textColor;
};

#endif //TEENSYCMAKE_LABELVIEW_H
