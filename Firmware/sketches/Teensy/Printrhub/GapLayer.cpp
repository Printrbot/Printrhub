//
// Created by Phillip Schuster on 17.01.16.
//

#include "GapLayer.h"
#include "Application.h"

extern int globR;

GapLayer::GapLayer(Rect frame): RectangleLayer(frame)
{
    this->setBackgroundColor(ILI9341_WHITE);
    this->setStrokeColor(ILI9341_BLACK);
    this->setStrokeWidth(1);
}

GapLayer::~GapLayer()
{

}

void GapLayer::draw()
{
    this->setBackgroundColor(RGB565(globR,globR,globR));
    RectangleLayer::draw();

    globR++;
    if (globR > 255) globR = 0;
}
