//
// Created by Phillip Schuster on 17.01.16.
//

#ifndef TEENSYCMAKE_PHDISPLAY_H
#define TEENSYCMAKE_PHDISPLAY_H

#include "ILI9341_t3.h"
#include "StackArray.h"
#include "../layers/Layer.h"
#include "../layers/RectangleLayer.h"
#include "SD.h"
#include "../../framework/core/ImageBuffer.h"
#include "UIBitmap.h"
#include "../../UIBitmaps.h"

class PHDisplay: public ILI9341_t3
{
#pragma mark Constructor
public:
	virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;

	PHDisplay(uint8_t _CS, uint8_t _DC, uint8_t _RST = 255, uint8_t _MOSI=11, uint8_t _SCLK=13, uint8_t _MISO=12);

#pragma Layer Management
    virtual void addLayer(Layer* layer);
    virtual void clear();   //Automatically enables auto layout. Use disableAutoLayout to disable it
    void setupBuffers();

#pragma mark Draw Code and Display
    virtual void setNeedsLayout();
	virtual void setNeedsDisplay();
	virtual bool willRefresh();
    virtual void layoutIfNeeded();
    virtual void dispatch();
    virtual void drawBitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* bitmap, uint16_t xs, uint16_t ys, uint16_t ws, uint16_t hs);
		virtual void drawFileBitmapByColumn(uint16_t x, uint16_t y, uint16_t w, uint16_t h, File *file, uint16_t xs, uint16_t ys, uint16_t ws, uint16_t hs, uint32_t byteOffset = 0);
    virtual void drawMaskedBitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t* bitmap, uint16_t xs, uint16_t ys, uint16_t ws, uint16_t hs, uint16_t foregroundColor, uint16_t backgroundColor);
    virtual void setClippingRect(Rect* rect);
    virtual void resetClippingRect();
    virtual void setBackgroundColor(uint16_t backgroundColor) { _backgroundColor = backgroundColor; };
    virtual uint16_t getBackgroundColor() { return _backgroundColor; };
    void invalidateRect(Rect invalidationRect);
    void invalidateRect(Rect &invalidationRect, int scrollOffset, int deltaScrollOffset);
    virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;
    virtual void drawPixel(int16_t x, int16_t y, uint16_t color) override;
    virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
    virtual void debugLayer(Layer* layer,bool fill, uint16_t color, bool waitForTap=true);
	virtual void setFixedBackgroundLayer(Layer *layer);
	virtual void waitForTap();
	virtual Rect prepareRenderFrame(const Rect proposedRenderFrame, DisplayContext context);
	virtual void drawImageBuffer(ImageBuffer* imageBuffer, Rect renderFrame);
	virtual void disableAutoLayout();   //Use clear to enable auto layout again

#pragma mark Render To Buffer
	virtual void lockBuffer(ImageBuffer* imageBuffer);
	virtual void unlock();

protected:
    virtual void drawFontBits(uint32_t bits, uint32_t numbits, uint32_t x, uint32_t y, uint32_t repeat) override;

#pragma Display Brightness
public:
    virtual void fadeOut();
    virtual void fadeIn();

#pragma mark Scrolling
public:
    virtual void setScrollOffset(float scrollOffset, bool update);
    virtual float getScrollOffset() { return _scrollOffset; };
    virtual void setScrollInsets(uint16_t left, uint16_t right);
    Rect visibleRect();
    uint16_t getLayoutWidth();
    uint16_t getLayoutStart();
    void cropRectToScreen(Rect& rect);
	int mapScrollOffset(int scrollOffset);
	virtual float clampScrollTarget(float scrollTarget);

#pragma mark Member Variables
public:
    bool debug;

private:
    uint16_t _scrollInsetLeft;
    uint16_t _scrollInsetRight;
    RectangleLayer* _backgroundLayer;
    RectangleLayer* _foregroundLayer;
    StackArray<Layer*> _layers;
    StackArray<Layer*> _presentationLayers;
    bool _needsLayout;
	bool _needsDisplay;
    float _scrollOffset;
    Rect* _clipRect;
    uint16_t _backgroundColor;
	Layer* _fixedBackgroundLayer;
	ImageBuffer* _lockBuffer;
	bool _autoLayout;


};


#endif //TEENSYCMAKE_PHDISPLAY_H
