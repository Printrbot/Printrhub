/*
 * Rectangle layer filled with color
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

#include "RectangleLayer.h"
#include "../core/Application.h"

RectangleLayer::RectangleLayer(Rect frame) :
	Layer(frame) {

}

RectangleLayer::~RectangleLayer() {

}

void RectangleLayer::draw(Rect &invalidationRect) {
  uint16_t backgroundColor = this->getBackgroundColor();
  uint16_t strokeColor = this->getStrokeColor();
  Rect renderFrame = Rect::Intersect(_frame, invalidationRect);

  //Map renderframe to screen space
  renderFrame = prepareRenderFrame(renderFrame);

  if (this->getStrokeWidth() > 0) {
	Display.drawRect(renderFrame.x, renderFrame.y, renderFrame.width, renderFrame.height, strokeColor);
	Display.fillRect(
		renderFrame.x + 1, renderFrame.y + 1, renderFrame.width - 2, renderFrame.height - 2, backgroundColor);
  } else {
	Display.fillRect(renderFrame.x, renderFrame.y, renderFrame.width, renderFrame.height, backgroundColor);
  }
}
