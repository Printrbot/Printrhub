/*
 * BitmapLayer draws an image in RGB565 format from SRAM or program memory to the
 * display
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

#ifndef TEENSYCMAKE_BITMAPLAYER_H
#define TEENSYCMAKE_BITMAPLAYER_H

#include <stdint.h>
#include "Layer.h"
#include "../core/UIBitmap.h"

class BitmapLayer : public Layer {
#pragma mark Constrcutor
 public:
  BitmapLayer();
  BitmapLayer(Rect rect);
  virtual ~BitmapLayer();

#pragma mark Layer
  virtual void draw(Rect &invalidationRect) override;
  virtual void setBitmap(const uint8_t *bitmap, uint16_t width, uint16_t height);
  virtual void setBitmap(const uint16_t *bitmap, uint16_t width, uint16_t height);

#pragma mark Getter/Setter
  const uint16_t &getColor() const { return _color; }
  void setColor(const uint16_t &color) { _color = color; }

#pragma mark Member Variables
 private:
  const uint8_t *_bitmap;
  const uint16_t *_bitmapRGB;
  const UIBitmap *_uiBitmap;
  uint16_t _width;
  uint16_t _height;
  uint16_t _color;
};

#endif //TEENSYCMAKE_BITMAPLAYER_H
