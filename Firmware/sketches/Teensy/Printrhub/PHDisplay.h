//
// Created by Phillip Schuster on 17.01.16.
//

#ifndef TEENSYCMAKE_PHDISPLAY_H
#define TEENSYCMAKE_PHDISPLAY_H

#include "ILI9341_t3.h"
#include "StackArray.h"
#include "Layer.h"
#include "RectangleLayer.h"
#include "SD.h"

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
    virtual void drawFileBitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, File *file, uint16_t xs, uint16_t ys, uint16_t ws, uint16_t hs, float alpha);

    virtual void setScrollOffset(float scrollOffset);
    virtual float getScrollOffset() { return _scrollOffset; };

    virtual void setClippingRect(Rect* rect);
    virtual void resetClippingRect();

    virtual void setBackgroundColor(uint16_t backgroundColor) { _backgroundColor = backgroundColor; };
    virtual uint16_t getBackgroundColor() { return _backgroundColor; };

    Rect visibleRect();

    bool debug;

private:
    void cropRectToScreen(Rect& rect);

    RectangleLayer* _backgroundLayer;
    RectangleLayer* _foregroundLayer;
    StackArray<Layer*> _layers;
    StackArray<Layer*> _presentationLayers;
    bool _needsLayout;
    float _scrollOffset;
    Rect* _clipRect;
    uint16_t _backgroundColor;

    void invalidateRect();

    void invalidateRect(Rect &dirtyRect, Rect &invalidationRect, uint16_t color);

protected:

public:
    virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;

    virtual void drawPixel(int16_t x, int16_t y, uint16_t color) override;

protected:
    virtual void drawFontBits(uint32_t bits, uint32_t numbits, uint32_t x, uint32_t y, uint32_t repeat) override;

public:
    virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;

    void setupBuffers();


};


#endif //TEENSYCMAKE_PHDISPLAY_H
