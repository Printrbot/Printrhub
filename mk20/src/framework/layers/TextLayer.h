/*
 * TextLayer handles text drawing inside a rectangular area and permits different
 * alignment and color options
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

#ifndef TEENSYCMAKE_TEXTLAYER_H
#define TEENSYCMAKE_TEXTLAYER_H

#include "Layer.h"
#include "ILI9341_t3.h"

#define TEXTALIGN_LEFT 0
#define TEXTALIGN_CENTERED 1
#define TEXTALIGN_RIGHT 2
#define TEXTALIGN_TOP 0
#define TEXTALIGN_BOTTOM 2

class TextLayer : public Layer {
#pragma mark Constructor
 public:
  TextLayer() {};
  TextLayer(Rect frame);
  virtual ~TextLayer() {};

#pragma mark Layer
  virtual void draw(Rect &invalidationRect) override;

#pragma mark Getter/Setter
  const ILI9341_t3_font_t *getFont() { return _font; }
  void setFont(const ILI9341_t3_font_t *_font) { TextLayer::_font = _font; }
  uint16_t getForegroundColor() const { return _foregroundColor; }
  void setForegroundColor(uint16_t _foregroundColor) { TextLayer::_foregroundColor = _foregroundColor; }
  uint16_t getBackgroundColor() const { return _backgroundColor; }
  void setBackgroundColor(uint16_t _backgroundColor) { TextLayer::_backgroundColor = _backgroundColor; }
  const String getText() const { return _text; }
  void setText(const String &text);
  uint8_t getTextAlign() const { return _textAlign; }
  uint8_t getVerticalTextAlign() const { return _verticalTextAlign; }
  void setVerticalTextAlign(uint8_t verticalTextAlign);
  void setTextAlign(uint8_t textAlign);
  uint8_t getPadding() const { return _padding; }
  void setPadding(uint8_t _padding) { TextLayer::_padding = _padding; }

#pragma mark Member Variables
 private:
  uint8_t _padding;
 protected:
  uint16_t _backgroundColor;
  const ILI9341_t3_font_t *_font;
  uint16_t _foregroundColor;
  String _text;
  uint8_t _textAlign;
  uint8_t _verticalTextAlign;
};

#endif //TEENSYCMAKE_TEXTLAYER_H
