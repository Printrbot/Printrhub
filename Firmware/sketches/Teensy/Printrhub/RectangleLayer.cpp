//
// Created by Phillip Schuster on 16.01.16.
//

#include "RectangleLayer.h"
#include "Application.h"

RectangleLayer::RectangleLayer(Rect frame):
        Layer(frame)
{

}

RectangleLayer::~RectangleLayer()
{

}

void RectangleLayer::draw()
{
    uint16_t backgroundColor = this->getBackgroundColor();
    uint16_t strokeColor = this->getStrokeColor();

/*    Serial.print("Fill-Rect: ");
    Serial.print(_frame.x);
    Serial.print(",");
    Serial.print(_frame.y);
    Serial.print(",");
    Serial.print(_frame.width);
    Serial.print(",");
    Serial.println(_frame.height);*/

    if (this->getStrokeWidth() > 0)
    {
        Display.drawRect(_frame.x,_frame.y,_frame.width,_frame.height,strokeColor);
        Display.fillRect(_frame.x+1,_frame.y+1,_frame.width-2,_frame.height-2,backgroundColor);
    }
    else
    {
        Display.fillRect(_frame.x,_frame.y,_frame.width,_frame.height,backgroundColor);
    }
}