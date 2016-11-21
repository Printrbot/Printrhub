/*
 * Shows details about a material
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

#include "MaterialView.h"
#include "framework/core/Application.h"
#include "framework/layers/SDBitmapLayer.h"
#include "framework/layers/TransparentTextLayer.h"
#include "font_LiberationSans.h"

extern UIBitmaps uiBitmaps;

MaterialView::MaterialView(Material material, uint8_t offset) :
	View(270 * offset, 0, 270, 240),
	_offset(offset),
	_material(material) {
}

void MaterialView::display() {
  SDBitmapLayer *imageLayer = new SDBitmapLayer(_frame);
  imageLayer->setBitmap("ui.min", uiBitmaps.materials_scene.width, uiBitmaps.materials_scene.height, uiBitmaps.materials_scene.offset);
  addLayer(imageLayer);

  _titleLayer = new TransparentTextLayer(Rect(_frame.x + 5, 100, Display.getLayoutWidth() - 5, 14));
  _titleLayer->setTextAlign(TEXTALIGN_CENTERED);
  _titleLayer->setFont(&LiberationSans_14);
  _titleLayer->setText(_material.name);
  _titleLayer->setForegroundColor(ILI9341_BLACK);
  addLayer(_titleLayer);

  _typeLayer = new TransparentTextLayer(Rect(_frame.x + 5, 125, Display.getLayoutWidth() - 5, 14));
  _typeLayer->setTextAlign(TEXTALIGN_CENTERED);
  _typeLayer->setFont(&LiberationSans_12);
  String _l = "Type: ";
  _l = _l + _material.type;
  _typeLayer->setText(_l.c_str());
  _typeLayer->setForegroundColor(ILI9341_BLACK);
  addLayer(_typeLayer);

  /*
  _brandLayer = new TransparentTextLayer(Rect(_frame.x+5,_frame.y+40,Display.getLayoutWidth()-15,230));
  _brandLayer->setTextAlign(TEXTALIGN_CENTERED);
  _brandLayer->setFont(&LiberationSans_12);
  _l = "Brand: ";
  _l = _l + _material.brand;
  _brandLayer->setText(_l.c_str());
  _brandLayer->setForegroundColor(ILI9341_BLACK);
  addLayer(_brandLayer);
*/
  _temperatureLayer = new TransparentTextLayer(Rect(_frame.x + 5, 150, Display.getLayoutWidth() - 5, 14));
  _temperatureLayer->setTextAlign(TEXTALIGN_CENTERED);
  _temperatureLayer->setFont(&LiberationSans_12);
  _l = "Print Temp: ";
  _l = _l + (int) _material.temperature;
  _temperatureLayer->setText(_l.c_str());
  _temperatureLayer->setForegroundColor(ILI9341_BLACK);
  addLayer(_temperatureLayer);

  View::display();
}
