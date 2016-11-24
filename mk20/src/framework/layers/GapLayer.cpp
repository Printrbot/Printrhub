/*
 * GapLayer is a simple rectangle layer filled with a background color. This is
 * used in the automatic layout system and allows for quickly debugging layer outlines of
 * all those layers that get generated automatically in this process.
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

#include "GapLayer.h"
#include "../core/Application.h"

extern int globR;

GapLayer::GapLayer(Rect frame) : RectangleLayer(frame) {
  this->setBackgroundColor(ILI9341_WHITE);
  this->setStrokeColor(ILI9341_BLACK);
  this->setStrokeWidth(0);
}

GapLayer::~GapLayer() {

}
/*
void GapLayer::draw(Rect& renderFrame)
{
    //this->setBackgroundColor(RGB565(globR,globR,globR));
    RectangleLayer::draw(renderFrame);
    return;

    globR++;
    if (globR > 255) globR = 0;

    Layer::draw(renderFrame);
}
*/