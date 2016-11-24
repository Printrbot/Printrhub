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

#include "TextLayer.h"
#include "../core/Application.h"

TextLayer::TextLayer(Rect frame) :
	Layer(frame),
	_textAlign(TEXTALIGN_LEFT),
	_verticalTextAlign(TEXTALIGN_CENTERED),
	_padding(0) {
  _backgroundColor = Application.getTheme()->getColor(BackgroundColor);
  _foregroundColor = Application.getTheme()->getColor(TextColor);
  _font = (ILI9341_t3_font_t * ) & PTSansNarrow_20;
}

void TextLayer::setTextAlign(uint8_t textAlign) {
  if (_textAlign == textAlign) return;
  _textAlign = textAlign;
  setNeedsDisplay();
}

void TextLayer::setVerticalTextAlign(uint8_t verticalTextAlign) {
  if (_verticalTextAlign == verticalTextAlign) return;
  _verticalTextAlign = verticalTextAlign;
  setNeedsDisplay();
}

void TextLayer::draw(Rect &invalidationRect) {
  Rect renderFrame = Rect::Intersect(_frame, invalidationRect);

//    Display.fillRect(renderFrame.x,renderFrame.y,renderFrame.width,renderFrame.height,ILI9341_BLUE);
//    return;

/*    int xs = renderFrame.left() - _frame.left();
    int ys = renderFrame.top() - _frame.top();
    int width = renderFrame.width;
    int height = renderFrame.height;

    //Transform to screen space
    renderFrame.x = renderFrame.x % 320;

    if (height > 0 && width > 0)
    {

    }*/

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

  //Fill left
  if (renderFrame.left() < frame.left()) {
	Display.fillRect(renderFrame.x, renderFrame.y, frame.x - renderFrame.x, renderFrame.height, _backgroundColor);
  }
  //Fill right
  if (renderFrame.right() > frame.right()) {
	Display.fillRect(frame.right(), renderFrame.y,
					 renderFrame.right() - frame.right(), renderFrame.height, _backgroundColor);
  }
  //Fill above
  if (renderFrame.top() < frame.top()) {
	Display.fillRect(renderFrame.x, renderFrame.y, renderFrame.width,
					 frame.top() - renderFrame.top(), _backgroundColor);
  }
  //Fill below
  if (renderFrame.bottom() > frame.bottom()) {
	Display.fillRect(renderFrame.x, frame.bottom(), renderFrame.width,
					 renderFrame.bottom() - frame.bottom(), _backgroundColor);
  }

  Display.setClippingRect(&renderFrame);

  Display.setFont(*_font);
  Display.setTextColor(_foregroundColor, _backgroundColor);
  Display.setTransparentText(false);
  Display.setCursor(frame.x, frame.y);
  Display.print(_text);

  //Fill the rest of the text layer
  Display.fillRect(Display.getCursorX(), frame.top(),
				   frame.right() - Display.getCursorX(), frame.height, _backgroundColor);

  Display.resetClippingRect();
}

void TextLayer::setText(const String &text) {
  _text = text;
  setNeedsDisplay();
}