//
// Created by Phillip Schuster on 17.01.16.
//

#include "PHDisplay.h"
#include "Application.h"

PHDisplay::PHDisplay(uint8_t _CS, uint8_t _DC, uint8_t _RST, uint8_t _MOSI, uint8_t _SCLK, uint8_t _MISO):
        ILI9341_t3(_CS,_DC,_RST,_MOSI,_SCLK,_MISO)
{
    _backgroundLayer = new RectangleLayer(Rect(0,0,240,320));
    _backgroundLayer->setBackgroundColor(ILI9341_BLACK);
    _backgroundLayer->setStrokeWidth(0);
}

void PHDisplay::addLayer(Layer *layer)
{
    LOG("Adding Layer");
    _backgroundLayer->splitWithRect(layer->getFrame());
    _layers.push(layer);
    LOG("Adding Layers done");
}

void PHDisplay::clear()
{
    _backgroundLayer->removeAllSublayers();
    _layers.clear(false);
}

void PHDisplay::dispatch()
{
    LOG("Sending background to display");
    _backgroundLayer->display();

    LOG("Sending layer to display");
    for (int i=0;i<_layers.count();i++)
    {
        Layer* layer = _layers.at(i);
        layer->display();
    }
}

void PHDisplay::drawBitmap(uint16_t x, uint16_t y, const uint16_t *bitmap, uint16_t w, uint16_t h)
{
    for (uint16_t yb=0;yb<h;yb++)
    {
        for (uint16_t xb=0;xb<w;xb++)
        {
            drawPixel(x+xb,y+yb,bitmap[yb*w+xb]);
        }
    }
}