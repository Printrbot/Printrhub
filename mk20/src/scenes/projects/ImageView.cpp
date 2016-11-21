/*
 * A view the sets up a more complex user interface with a project/job scene
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/behind-the-scenes-printrbot-simple-2016/
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Mick Balaban
 * https://github.com/Printrbot/Printrhub
 *
 * Developed in cooperation with Phillip Schuster (@appfruits) from appfruits.com
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

#include "ImageView.h"
#include "framework/core/Application.h"
#include "framework/layers/SDBitmapLayer.h"
#include "framework/layers/TransparentTextLayer.h"
#include "font_LiberationSans.h"

ImageView::ImageView(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t offset) :
	View(x, y, width, height),
	_offset(offset) {
  _name = "ImageView";
}

ImageView::ImageView(Rect frame, uint32_t offset) :
	View(frame),
	_offset(offset) {

}

void ImageView::display() {
  SDBitmapLayer *imageLayer = new SDBitmapLayer(_frame);
  imageLayer->setBitmap(_indexFileName.c_str(), 270, 240, _offset);
  addLayer(imageLayer);

  _imageLayer = imageLayer;

  _imageTitleLayer = new TransparentTextLayer(Rect(_frame.x + 15, _frame.y + 10, Display.getLayoutWidth() - 30, 25));
  _imageTitleLayer->setTextAlign(TEXTALIGN_LEFT);
  _imageTitleLayer->setFont(&LiberationSans_14);
  _imageTitleLayer->setText(_imageTitle);
  _imageTitleLayer->setForegroundColor(ILI9341_BLACK);
  //_imageTitleLayer->setBackgroundColor(getBackgroundColor());
  addLayer(_imageTitleLayer);
  View::display();
}
