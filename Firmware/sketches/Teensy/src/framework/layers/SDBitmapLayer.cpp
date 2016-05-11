//
// Created by Phillip Schuster on 22.02.16.
//

#include "SDBitmapLayer.h"
#include "../core/Application.h"

SDBitmapLayer::SDBitmapLayer(Rect frame): Layer(frame)
{

}

SDBitmapLayer::~SDBitmapLayer()
{

}

void SDBitmapLayer::setBitmap(const char *filePath, uint16_t width, uint16_t height)
{
    _filePath = filePath;
    _width = width;
    _height = height;
    _needsDisplay = true;
}

void SDBitmapLayer::draw(Rect &invalidationRect)
{
    Rect renderFrame = Rect::Intersect(_frame,invalidationRect);

    int xs = renderFrame.left() - _frame.left();
    int ys = renderFrame.top() - _frame.top();
    int width = renderFrame.width;
    int height = renderFrame.height;

    //Map renderframe to screen space
    renderFrame = prepareRenderFrame(renderFrame);

    if (height > 0 && width > 0)
    {
        _file = SD.open(_filePath,FILE_READ);
        Display.drawFileBitmapByColumn(renderFrame.x,renderFrame.y,width,height,&_file,xs,ys,_width,_height);
    }
}

