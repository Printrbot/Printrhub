//
// Created by Phillip Schuster on 26.01.16.
//

#include "TextLayer.h"
#include "Application.h"

TextLayer::TextLayer(Rect frame):
Layer(frame),
_foregroundColor(RGB565(0,0,0)),
_backgroundColor(RGB565(128,255,255))
{
    _font = (ILI9341_t3_font_t *) &Arial_20;
}

void TextLayer::draw(Rect &dirtyRect, Rect &invalidationRect)
{
    Rect renderFrame = Rect::Intersect(_frame,invalidationRect);

/*    int xs = renderFrame.left() - _frame.left();
    int ys = renderFrame.top() - _frame.top();
    int width = renderFrame.width;
    int height = renderFrame.height;

    //Transform to screen space
    renderFrame.x = renderFrame.x % 320;

    if (height > 0 && width > 0)
    {

    }*/

    Rect font = _frame;

    font.x = font.x % 320;
    renderFrame.x = renderFrame.x % 320;

    Display.setClippingRect(&renderFrame);

    Display.setFont(Arial_20);
    Display.setTextColor(_foregroundColor);
    Display.setBackgroundColor(_backgroundColor);
    Display.setCursor(font.x,font.y);
    Display.print(*_text);

    Display.resetClippingRect();
}