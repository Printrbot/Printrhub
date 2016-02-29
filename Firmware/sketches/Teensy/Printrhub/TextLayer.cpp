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
    _font = (ILI9341_t3_font_t *) &PTSansNarrow_20;
}

void TextLayer::setTextAlign(uint8_t textAlign)
{
    if (_textAlign == textAlign) return;
    _textAlign = textAlign;
    setNeedsDisplay();
}

void TextLayer::setVerticalTextAlign(uint8_t verticalTextAlign)
{
    if (_verticalTextAlign == verticalTextAlign) return;
    _verticalTextAlign = verticalTextAlign;
    setNeedsDisplay();
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

    //TODO: Add code for horizontal alignment

    //TODO: Fix this bug. If the inner layers left and right ly on the outer layers boundaries
    //there is a strange render bug. Inseting by one pixels helps though and doesn't matter here
    frame.x += 1;
    frame.width -= 2;

    if (_textAlign == TEXTALIGN_CENTERED)
    {
        uint32_t width = Display.textWidth(_font, *_text);
        frame.x += (frame.width - width)/2;
        frame.width = width;
    }
    else if (_textAlign == TEXTALIGN_RIGHT)
    {
        uint32_t width = Display.textWidth(_font, *_text);
        frame.x += (frame.width - width);
        frame.width = width;
    }

    frame.x = frame.x % 320;
    renderFrame.x = renderFrame.x % 320;

    //Fill left
    if (renderFrame.left() < frame.left())
    {
        Display.fillRect(renderFrame.x,renderFrame.y,frame.x - renderFrame.x,renderFrame.height,_backgroundColor);
    }
    //Fill right
    if (renderFrame.right() > frame.right())
    {
        Display.fillRect(frame.right(),renderFrame.y,renderFrame.right()-frame.right(),renderFrame.height,_backgroundColor);
    }
    //Fill above
    if (renderFrame.top() < frame.top())
    {
        Display.fillRect(renderFrame.x,renderFrame.y,renderFrame.width,frame.top()-renderFrame.top(),_backgroundColor);
    }
    //Fill below
    if (renderFrame.bottom() > frame.bottom())
    {
        Display.fillRect(renderFrame.x,frame.bottom(),renderFrame.width,renderFrame.bottom() - frame.bottom(),_backgroundColor);
    }

    Display.setClippingRect(&renderFrame);

    Display.setFont(*_font);
    Display.setTextColor(_foregroundColor,_backgroundColor);
    Display.setCursor(frame.x,frame.y);
    Display.print(*_text);

    //Fill the rest of the text layer
    Display.fillRect(Display.getCursorX(),frame.top(),frame.right()-Display.getCursorX(),frame.height,_backgroundColor);

    Display.resetClippingRect();
}