/*
 * TransparentTextLayer handles text drawing inside a rectangular area and permits different
 * alignment and color options but does not draw a background
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

#include "TransparentTextLayer.h"
#include "../../framework/core/Application.h"

TransparentTextLayer::TransparentTextLayer(Rect frame) :
	TextLayer(frame) {
}

void TransparentTextLayer::draw(Rect &invalidationRect) {
  Rect renderFrame = Rect::Intersect(_frame, invalidationRect);

  //Align the text layer
  Rect frame = _frame;
  if (_verticalTextAlign == TEXTALIGN_TOP) {
	frame.y += 1;
  } else if (_verticalTextAlign == TEXTALIGN_BOTTOM) {
	frame.y = frame.bottom() - _font->cap_height - 1;
  } else if (_verticalTextAlign == TEXTALIGN_CENTERED) {
	frame.y += (frame.height - _font->cap_height) / 2;
  }
  frame.height = _font->cap_height;

  //Assign padding
  frame.x += getPadding();
  frame.width -= getPadding() * 2;

  //TODO: Add code for horizontal alignment

  //TODO: Fix this bug. If the inner layers left and right ly on the outer layers boundaries
  //there is a strange render bug. Inseting by one pixels helps though and doesn't matter here
  frame.x += 1;
  frame.width -= 2;

  if (_textAlign == TEXTALIGN_CENTERED) {
	uint32_t width = Display.textWidth(_font, _text);
	frame.x += (frame.width - width) / 2;
	frame.width = width;
  } else if (_textAlign == TEXTALIGN_RIGHT) {
	uint32_t width = Display.textWidth(_font, _text);
	frame.x += (frame.width - width);
	frame.width = width;
  }

  //Map renderframes to screen space
  renderFrame = prepareRenderFrame(renderFrame);
  frame = prepareRenderFrame(frame);

  Display.setClippingRect(&renderFrame);

  Display.setFont(*_font);
  Display.setTextColor(_foregroundColor, _backgroundColor);
  Display.setTransparentText(true);
  Display.setCursor(frame.x, frame.y);
  Display.print(_text);

  Display.resetClippingRect();
}