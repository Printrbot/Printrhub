//
// Created by Phillip Schuster on 16.01.16.
//

#ifndef TEENSYCMAKE_LAYER_H
#define TEENSYCMAKE_LAYER_H

#include "UIElement.h"
#include "StackArray.h"

class Layer: public UIElement
{
public:
    Layer();
    Layer(Rect frame);
    virtual ~Layer();

    virtual void draw() = 0;

    void display(Layer* backgroundLayer=NULL);

    void setBackgroundColor(const uint16_t& color);
    uint16_t getBackgroundColor() const;

    void setStrokeColor(const uint16_t& strokeColor);
    uint16_t getStrokeColor() const;

    void setStrokeWidth(const uint8_t strokeWidth);
    uint8_t getStrokeWidth() const;

    bool isLeaf();

    void splitWithRect(Rect& rect);

    void addSublayer(Layer* layer);
    StackArray<Layer*>* getSublayers();

    void removeAllSublayers();

    virtual Layer* subLayerWithRect(Rect frame);

    int uniqueId;

private:
    void splitVertically(int x, Layer** left, Layer** right);
    void splitHorizontally(int y, Layer**top, Layer**bottom);

private:
    uint16_t _backgroundColor;
    uint16_t _strokeColor;
    uint8_t _strokeWidth;

    StackArray<Layer*>* _sublayers;
public:
    virtual void setFrame(Rect frame) override;
};


#endif //TEENSYCMAKE_LAYER_H
