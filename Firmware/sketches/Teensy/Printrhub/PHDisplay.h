//
// Created by Phillip Schuster on 17.01.16.
//

#ifndef TEENSYCMAKE_PHDISPLAY_H
#define TEENSYCMAKE_PHDISPLAY_H

#include <ILI9341_t3.h>
#include "StackArray.h"
#include "Layer.h"
#include "RectangleLayer.h"

class PHDisplay: public ILI9341_t3
{
public:
    PHDisplay(uint8_t _CS, uint8_t _DC, uint8_t _RST = 255, uint8_t _MOSI=11, uint8_t _SCLK=13, uint8_t _MISO=12);

    virtual void addLayer(Layer* layer);
    virtual void clear();

    virtual void dispatch();

    virtual void setNeedsLayout();
    virtual void layoutIfNeeded();

    virtual void drawBitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* bitmap, uint16_t xs, uint16_t ys, uint16_t ws, uint16_t hs, float alpha);

    bool debug;

private:
    void cropRectToScreen(Rect& rect);

    RectangleLayer* _backgroundLayer;
    RectangleLayer* _foregroundLayer;
    StackArray<Layer*> _layers;
    StackArray<Layer*> _presentationLayers;
    bool _needsLayout;
};


#endif //TEENSYCMAKE_PHDISPLAY_H
