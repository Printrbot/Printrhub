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

//    Display.fillRect(renderFrame.x,renderFrame.y,renderFrame.width,renderFrame.height,ILI9341_BLUE);
//    return;

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
    Display.setTextColor(_foregroundColor,_backgroundColor);
    Display.setCursor(font.x,font.y);
    Display.print(*_text);

    //Fill the rest of the text layer
    Display.fillRect(Display.getCursorX(),font.y,font.right()-Display.getCursorX(),font.height,_backgroundColor);

    Display.resetClippingRect();
}