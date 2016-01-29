//
// Created by Phillip Schuster on 30.01.16.
//

#include "LabelButton.h"

LabelButton::LabelButton(String text, Rect frame):
LabelView(text,frame)
{
}

LabelButton::LabelButton(String text, uint16_t x, uint16_t y, uint16_t width, uint16_t height):
LabelView(text,x,y,width,height)
{
}
