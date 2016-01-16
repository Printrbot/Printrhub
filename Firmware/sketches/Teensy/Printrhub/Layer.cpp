//
// Created by Phillip Schuster on 16.01.16.
//

#include "Layer.h"
#include "Application.h"

Layer::Layer():
_strokeColor(ILI9341_BLACK),
_backgroundColor(ILI9341_BLACK),
_strokeWidth(0)
{

}

Layer::Layer(const Rect &frame):
Layer()
{
    _frame = frame;
}

void Layer::setBackgroundColor(const uint16_t &color)
{
    _backgroundColor = color;
}

uint16_t Layer::getBackgroundColor() const
{
    return _backgroundColor;
}

void Layer::setStrokeColor(const uint16_t &strokeColor)
{
    _strokeColor = strokeColor;
}

uint16_t Layer::getStrokeColor() const
{
    return _strokeColor;
}

void Layer::setStrokeWidth(const uint8_t strokeWidth)
{
    _strokeWidth = strokeWidth;
}

uint8_t Layer::getStrokeWidth() const
{
    return _strokeWidth;
}