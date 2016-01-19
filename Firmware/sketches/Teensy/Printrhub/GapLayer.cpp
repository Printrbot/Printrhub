//
// Created by Phillip Schuster on 17.01.16.
//

#include "GapLayer.h"
#include "Application.h"

int globR;

GapLayer::GapLayer(Rect frame): RectangleLayer(frame)
{
    this->setBackgroundColor(ILI9341_WHITE);
    this->setStrokeColor(ILI9341_BLACK);
    this->setStrokeWidth(0);
    globR += 20;
}

GapLayer::~GapLayer()
{

}