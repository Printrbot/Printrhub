//
// Created by Phillip Schuster on 16.01.16.
//

#include "RectangleLayer.h"
#include "Application.h"

RectangleLayer::RectangleLayer(Rect frame):
        Layer(frame)
{

}

void RectangleLayer::display()
{
    LOG("Drawing Rectangle Layer");
    uint16_t backgroundColor = this->getBackgroundColor();
    uint16_t strokeColor = this->getStrokeColor();

    Display.fillRect(_frame.x,_frame.y,_frame.width,_frame.height,backgroundColor);
    if (this->getStrokeWidth() > 0)
    {
        Display.drawRect(_frame.x,_frame.y,_frame.width,_frame.height,strokeColor);
    }
}