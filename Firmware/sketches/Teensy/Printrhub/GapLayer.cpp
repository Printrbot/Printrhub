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
    this->setStrokeWidth(0);
}

GapLayer::~GapLayer()
{

}

void GapLayer::draw(Rect& renderFrame)
{
    this->setBackgroundColor(RGB565(globR,globR,globR));
    RectangleLayer::draw(renderFrame);

    globR++;
    if (globR > 255) globR = 0;

    Layer::draw(renderFrame);
}
