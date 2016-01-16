//
// Created by Phillip Schuster on 16.01.16.
//

#ifndef TEENSYCMAKE_LAYER_H
#define TEENSYCMAKE_LAYER_H

#include "UIElement.h"

class Layer: public UIElement
{
public:
    Layer();
    Layer(const Rect& frame);

    virtual void display() = 0;

    void setBackgroundColor(const uint16_t& color);
    uint16_t getBackgroundColor() const;

    void setStrokeColor(const uint16_t& strokeColor);
    uint16_t getStrokeColor() const;

    void setStrokeWidth(const uint8_t strokeWidth);
    uint8_t getStrokeWidth() const;

private:
    uint16_t _backgroundColor;
    uint16_t _strokeColor;
    uint8_t _strokeWidth;
};


#endif //TEENSYCMAKE_LAYER_H
