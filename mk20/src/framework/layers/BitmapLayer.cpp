//
// Created by Phillip Schuster on 19.01.16.
//

#include "BitmapLayer.h"
#include "../core/Application.h"

BitmapLayer::BitmapLayer(Rect frame): Layer(frame) {
  _bitmap = NULL;
  _color = ILI9341_PURPLE;
}

BitmapLayer::~BitmapLayer() {}

void BitmapLayer::setBitmap(const uint8_t *bitmap, uint16_t width, uint16_t height)
{
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


void BitmapLayer::draw(Rect &invalidationRect)
{
  if (_bitmap == NULL && _bitmapRGB == NULL && _uiBitmap == NULL) return;

  Rect renderFrame = Rect::Intersect(_frame,invalidationRect);

  int xs = renderFrame.left() - _frame.left();
  int ys = renderFrame.top() - _frame.top();
  int width = renderFrame.width;
  int height = renderFrame.height;

  //Map renderframe to screen space
  renderFrame = prepareRenderFrame(renderFrame);

  if (height > 0 && width > 0)
  {
    if (_bitmap != NULL)
      Display.drawMaskedBitmap(renderFrame.x,renderFrame.y,width,height,_bitmap,xs,ys,_width,_height,getBackgroundColor(),getColor());
    else if (_bitmapRGB != NULL)
      Display.drawBitmap(renderFrame.x, renderFrame.y, width, height, _bitmapRGB, xs, ys, _width, _height);
    else if (_uiBitmap != NULL) {
      //Display.drawFileBitmapByColumn(renderFrame.x, renderFrame.y, width, height, &file, xs, ys, _width, _height, _uiBitmap->offset);
    }
  }
}
