//
// Created by Phillip Schuster on 16.01.16.
//

#include "UIElement.h"

Rect& UIElement::getFrame()
{
    return _frame;
}

void UIElement::setFrame(Rect frame)
{
    _frame = frame;
}

void UIElement::setFrame(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    setFrame(Rect(x,y,width,height));
}
