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

void RectangleLayer::draw(Rect& dirtyRect, Rect& invalidationRect)
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

    Rect renderFrame = Rect::Intersect(_frame,invalidationRect);
    LOG_VALUE("Frame:",_frame.toString());
    LOG_VALUE("renderFrame:",renderFrame.toString());

    //Transform to screen space
    renderFrame.x = renderFrame.x % 320;

    if (this->getStrokeWidth() > 0)
    {
        Display.drawRect(renderFrame.x,renderFrame.y,renderFrame.width,renderFrame.height,strokeColor);
        Display.fillRect(renderFrame.x+1,renderFrame.y+1,renderFrame.width-2,renderFrame.height-2,backgroundColor);
    }
    else
    {
        Display.fillRect(renderFrame.x,renderFrame.y,renderFrame.width,renderFrame.height,backgroundColor);
    }
}