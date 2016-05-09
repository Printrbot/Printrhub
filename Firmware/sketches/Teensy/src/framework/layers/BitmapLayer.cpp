//
// Created by Phillip Schuster on 19.01.16.
//

#include "BitmapLayer.h"
#include "../core/Application.h"

BitmapLayer::BitmapLayer(Rect frame): Layer(frame)
{
    _bitmap = NULL;
    _color = ILI9341_PURPLE;
}

BitmapLayer::~BitmapLayer()
{

}

void BitmapLayer::setBitmap(const uint8_t *bitmap, uint16_t width, uint16_t height)
{
    _bitmap = bitmap;
    _width = width;
    _height = height;
    _needsDisplay = true;
}

void BitmapLayer::draw(Rect& dirtyRect, Rect& invalidationRect)
{
    if (_bitmap == NULL) return;

    Rect renderFrame = Rect::Intersect(_frame,invalidationRect);

    int xs = renderFrame.left() - _frame.left();
    int ys = renderFrame.top() - _frame.top();
    int width = renderFrame.width;
    int height = renderFrame.height;

    //Transform to screen space
    renderFrame.x = renderFrame.x % Display.getLayoutWidth();
    renderFrame.x += Display.getLayoutStart();

    if (height > 0 && width > 0)
    {
        Display.drawMaskedBitmap(renderFrame.x,renderFrame.y,width,height,_bitmap,xs,ys,_width,_height,getBackgroundColor(),getColor());
    }

//    Display.fillRect(renderFrame.x,renderFrame.y,renderFrame.width,renderFrame.height,ILI9341_PINK);
    return;
/*
    int x = renderFrame.x;
    int y = renderFrame.y;
    int width = _width;
    int height = _height;
    int imageOffset = 0;
    int xs = 0;
    int ys = 0;

    if (renderFrame.y < 0)
    {
        y = 0;
        height = _height + renderFrame.y;
        ys = abs(renderFrame.y);
    }

    if (renderFrame.x < 0)
    {
        x = 0;
        width = _width + renderFrame.x;
        xs = abs(renderFrame.x);
    }

    if (renderFrame.y + _height > 239)
    {
        height = 239 - renderFrame.y;
    }

    if (renderFrame.x + _width > 319)
    {
        width = 319 - renderFrame.x;
    }

    if (height > 0 && width > 0)
    {
        Display.drawBitmap(x,y,width,height,_bitmap,xs,ys,_width,_height,1);
    }

    Layer::draw(renderFrame);*/
}

