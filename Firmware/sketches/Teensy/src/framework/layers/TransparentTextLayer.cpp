//
// Created by Phillip Schuster on 11.05.16.
//

#include "TransparentTextLayer.h"
#include "../../framework/core/Application.h"

TransparentTextLayer::TransparentTextLayer(Rect frame):
TextLayer(frame)
{
}

void TransparentTextLayer::draw(Rect &dirtyRect, Rect &invalidationRect)
{
    Rect renderFrame = Rect::Intersect(_frame, invalidationRect);

    //Align the text layer
    Rect frame = _frame;
    if (_verticalTextAlign == TEXTALIGN_TOP)
    {
        frame.y += 1;
    }
    else if (_verticalTextAlign == TEXTALIGN_BOTTOM)
    {
        frame.y = frame.bottom()-_font->cap_height - 1;
    }
    else if (_verticalTextAlign == TEXTALIGN_CENTERED)
    {
        frame.y += (frame.height - _font->cap_height) / 2;
    }
    frame.height = _font->cap_height;

    //Assign padding
    frame.x += getPadding();
    frame.width -= getPadding()*2;

    //TODO: Add code for horizontal alignment

    //TODO: Fix this bug. If the inner layers left and right ly on the outer layers boundaries
    //there is a strange render bug. Inseting by one pixels helps though and doesn't matter here
    frame.x += 1;
    frame.width -= 2;

    if (_textAlign == TEXTALIGN_CENTERED)
    {
        uint32_t width = Display.textWidth(_font, _text);
        frame.x += (frame.width - width)/2;
        frame.width = width;
    }
    else if (_textAlign == TEXTALIGN_RIGHT)
    {
        uint32_t width = Display.textWidth(_font, _text);
        frame.x += (frame.width - width);
        frame.width = width;
    }

    frame.x = frame.x % Display.getLayoutWidth();
    frame.x += Display.getLayoutStart();
    renderFrame.x = renderFrame.x % Display.getLayoutWidth();
    renderFrame.x += Display.getLayoutStart();

    Display.setClippingRect(&renderFrame);

    Display.setFont(*_font);
    Display.setTextColor(_foregroundColor,_backgroundColor);
    Display.setTransparentText(true);
    Display.setCursor(frame.x,frame.y);
    Display.print(_text);

    Display.resetClippingRect();
}