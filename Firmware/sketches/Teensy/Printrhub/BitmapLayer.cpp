//
// Created by Phillip Schuster on 19.01.16.
//

#include "BitmapLayer.h"
#include "Application.h"

BitmapLayer::BitmapLayer(Rect frame): Layer(frame)
{
    _bitmap = NULL;
    _alpha = 1;
}

BitmapLayer::~BitmapLayer()
{

}

void BitmapLayer::setBitmap(const uint16_t *bitmap, uint16_t width, uint16_t height)
{
    _bitmap = bitmap;
    _width = width;
    _height = height;
    _needsDisplay = true;
}

void BitmapLayer::draw()
{
    if (_bitmap == NULL) return;

    int x = _frame.x;
    int y = _frame.y;
    int width = _width;
    int height = _height;
    int imageOffset = 0;
    int xs = 0;
    int ys = 0;

    if (_frame.y < 0)
    {
        y = 0;
        height = _height + _frame.y;
        ys = abs(_frame.y);
    }

    if (_frame.x < 0)
    {
        x = 0;
        width = _width + _frame.x;
        xs = abs(_frame.x);
    }

    if (_frame.y + _height > 239)
    {
        height = 239 - _frame.y;
    }

    if (_frame.x + _width > 319)
    {
        width = 319 - _frame.x;
    }

    if (height > 0 && width > 0)
    {
        Display.drawBitmap(x,y,width,height,_bitmap,xs,ys,_width,_height,1);
    }

    Layer::draw();
}

