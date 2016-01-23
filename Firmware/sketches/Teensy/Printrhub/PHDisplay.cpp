//
// Created by Phillip Schuster on 17.01.16.
//

#include "PHDisplay.h"
#include "Application.h"
#include <SPI.h>
#include <Wiring.h>

#define SPICLOCK 30000000

PHDisplay::PHDisplay(uint8_t _CS, uint8_t _DC, uint8_t _RST, uint8_t _MOSI, uint8_t _SCLK, uint8_t _MISO):
        ILI9341_t3(_CS,_DC,_RST,_MOSI,_SCLK,_MISO)
{
    _foregroundLayer = new RectangleLayer(Rect(0,0,320,240));
    _foregroundLayer->setBackgroundColor(ILI9341_BLACK);
    _foregroundLayer->setStrokeWidth(0);

    _backgroundLayer = new RectangleLayer(Rect(0,0,320,240));
    _backgroundLayer->setBackgroundColor(ILI9341_BLACK);
    _backgroundLayer->setStrokeWidth(0);

    _needsLayout = true;

    debug = false;
}

void PHDisplay::addLayer(Layer *layer)
{
    layer->setNeedsDisplay();
    if (debug) LOG("Adding Layer");
//    _backgroundLayer->splitWithRect(layer->getFrame());
    _layers.push(layer);
    if (debug) LOG("Adding Layers done");

    _needsLayout = true;
}

void PHDisplay::clear()
{
    _backgroundLayer->removeAllSublayers();
    _foregroundLayer->removeAllSublayers();
    _layers.clear(false);
}


void PHDisplay::cropRectToScreen(Rect &rect)
{
    if (rect.left() < 0)
    {
        rect.setLeft(0);
    }
    if (rect.top() < 0)
    {
        rect.setTop(0);
    }
    if (rect.right() > 319)
    {
        rect.setRight(319);
    }
    if (rect.bottom() > 239)
    {
        rect.setBottom(239);
    }
}

void PHDisplay::layoutIfNeeded()
{
    if (!_needsLayout) return;

    //Now delete the old background layer and swap foreground to background
    delete _backgroundLayer;
    _backgroundLayer = _foregroundLayer;

    //Create new Layer (forming the new foreground layer)
    _foregroundLayer = new RectangleLayer(Rect(0,0,320,240));
    _foregroundLayer->setBackgroundColor(ILI9341_BLACK);
    _foregroundLayer->setStrokeWidth(0);

    for (int i=0;i<_layers.count();i++)
    {
        Layer *layer = _layers.at(i);

        Rect layerFrame = layer->getFrame();
        Rect screenRect = Rect(0,0,319,239);
        if (screenRect.intersectsRect(layerFrame))
        {
            //Crop Rectangle to screen
            cropRectToScreen(layerFrame);

            _foregroundLayer->splitWithRect(layerFrame);
        }
    }

    _needsLayout = false;
}

void PHDisplay::dispatch()
{
    //LOG("Sending background to display");

    _foregroundLayer->display(_backgroundLayer);

    //LOG("Sending layer to display");
    for (int i=0;i<_layers.count();i++)
    {
        Layer* layer = _layers.at(i);
        layer->display();
    }
}

void PHDisplay::drawBitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *bitmap, uint16_t xs, uint16_t ys, uint16_t ws, uint16_t hs, float alpha)
{
    SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
    setAddr(x, y, x+w-1, y+h-1);
    writecommand_cont(ILI9341_RAMWR);
    for (uint16_t yb=0;yb<h;yb++)
    {
        for (uint16_t xb=0;xb<w;xb++)
        {
            if (xb == w-1 && yb == h-1)
            {
                //Last pixel
                writedata16_last(bitmap[(yb+ys)*ws+(xb+xs)]);
            }
            else
            {
                //All other pixels
                writedata16_cont(bitmap[(yb+ys)*ws+(xb+xs)]);
            }
        }
    }
    SPI.endTransaction();
}

void PHDisplay::setNeedsLayout()
{
    _needsLayout = true;
}
