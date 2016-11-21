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

#ifndef MATERIAL_VIEW_H
#define MATERIAL_VIEW_H

#include "../../framework/views/View.h"
#include "../../framework/layers/TextLayer.h"
#include "../../framework/layers/RectangleLayer.h"
#include "../../framework/layers/BitmapLayer.h"
#include "../../framework/layers/SDBitmapLayer.h"
#include "../../framework/views/LabelButton.h"

typedef struct Material {
  char name[32];
  char type[12];
  char brand[32];
  int16_t temperature;
  int16_t speed;
  uint8_t retraction;
} Material;

class MaterialView : public View {
 public:
  MaterialView(Material material, uint8_t offset = 0);

  uint16_t _width;
  virtual void display() override;

 private:
  Material _material;
  uint32_t _offset;
  TextLayer *_titleLayer;
  TextLayer *_typeLayer;
  TextLayer *_brandLayer;
  TextLayer *_temperatureLayer;
  TextLayer *_speedLayer;
};

#endif
