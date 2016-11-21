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

#include "BitmapLayer.h"
#include "../core/Application.h"

BitmapLayer::BitmapLayer(Rect frame) : Layer(frame) {
  _bitmap = NULL;
  _color = ILI9341_PURPLE;
}

BitmapLayer::~BitmapLayer() {}

void BitmapLayer::setBitmap(const uint8_t *bitmap, uint16_t width, uint16_t height) {
  _bitmap = bitmap;
  _width = width;
  _height = height;
  _bitmapRGB = NULL;
  _needsDisplay = true;
}

void BitmapLayer::setBitmap(const uint16_t *bitmapRGB, uint16_t width, uint16_t height) {
  _bitmapRGB = bitmapRGB;
  _bitmap = NULL;
  _width = width;
  _height = height;
  _needsDisplay = true;
}

void BitmapLayer::draw(Rect &invalidationRect) {
  if (_bitmap == NULL && _bitmapRGB == NULL && _uiBitmap == NULL) return;

  Rect renderFrame = Rect::Intersect(_frame, invalidationRect);

  int xs = renderFrame.left() - _frame.left();
  int ys = renderFrame.top() - _frame.top();
  int width = renderFrame.width;
  int height = renderFrame.height;

  //Map renderframe to screen space
  renderFrame = prepareRenderFrame(renderFrame);

  if (height > 0 && width > 0) {
	if (_bitmap != NULL)
	  Display.drawMaskedBitmap(renderFrame.x, renderFrame.y, width, height, _bitmap, xs, ys, _width, _height, getBackgroundColor(), getColor());
	else if (_bitmapRGB != NULL)
	  Display.drawBitmap(renderFrame.x, renderFrame.y, width, height, _bitmapRGB, xs, ys, _width, _height);
	else if (_uiBitmap != NULL) {
	  //Display.drawFileBitmapByColumn(renderFrame.x, renderFrame.y, width, height, &file, xs, ys, _width, _height, _uiBitmap->offset);
	}
  }
}
