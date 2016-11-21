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

#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H

#include "../../framework/views/View.h"
#include "../../framework/layers/TextLayer.h"
#include "../../framework/layers/RectangleLayer.h"
#include "../../framework/layers/BitmapLayer.h"
#include "../../framework/layers/SDBitmapLayer.h"
#include "../../framework/views/LabelButton.h"

class ImageView : public View {
 public:
  ImageView(Rect frame, uint32_t offset = 0);
  ImageView(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t offset = 0);

  void setImageTitle(String imageTitle) { _imageTitle = imageTitle; };

  void setBitmap(const uint16_t *bitmap) { _bitmap = bitmap; };
  void setColor(const uint16_t color) { _color = color; };
  void setIndexFileName(String fileName) { _indexFileName = fileName; };
  String getIndexFileName() { return _indexFileName; };

  uint16_t _width;
  virtual void display() override;

 private:
  String _imageTitle;
  String _indexFileName;
  TextLayer *_imageTitleLayer;
  Layer *_imageLayer;
  const uint16_t *_bitmap;
  uint16_t _color;
  uint32_t _offset;
};

#endif
