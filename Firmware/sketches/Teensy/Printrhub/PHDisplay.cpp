//
// Created by Phillip Schuster on 17.01.16.
//

#include "PHDisplay.h"
#include "Application.h"
#include <SPI.h>
#include <Wiring.h>
#include <Arduino.h>

#define SPICLOCK 30000000

PHDisplay::PHDisplay(uint8_t _CS, uint8_t _DC, uint8_t _RST, uint8_t _MOSI, uint8_t _SCLK, uint8_t _MISO):
        ILI9341_t3(_CS,_DC,_RST,_MOSI,_SCLK,_MISO)
{
    _foregroundLayer = new RectangleLayer(Rect(0,0,750,240));
    _foregroundLayer->setBackgroundColor(ILI9341_WHITE);
    _foregroundLayer->setStrokeWidth(0);

    _backgroundLayer = new RectangleLayer(Rect(0,0,750,240));
    _backgroundLayer->setBackgroundColor(ILI9341_WHITE);
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

    //SceneController* currentScene = Application.currentScene();

    //Get Max Layer Width
    Rect bounds = Rect(0,0,320,240);
    for (int i=0;i<_layers.count();i++)
    {
        Layer *layer = _layers.at(i);

        if (layer->getFrame().left() < bounds.left())
        {
            bounds.x = layer->getFrame().x;
        }
        if (layer->getFrame().right() > bounds.right())
        {
            bounds.width = layer->getFrame().right() - bounds.left();
        }
        if (layer->getFrame().top() < bounds.top())
        {
            bounds.y = layer->getFrame().y;
        }
        if (layer->getFrame().bottom() > bounds.bottom())
        {
            bounds.height = layer->getFrame().bottom() - bounds.top();
        }
    }

    bounds.width += 1;

    uint16_t backgroundColor = Application.currentScene()->getBackgroundColor();

    //Create new Layer (forming the new foreground layer)
    _foregroundLayer = new RectangleLayer(bounds);
    _foregroundLayer->setBackgroundColor(backgroundColor);
    _foregroundLayer->setStrokeWidth(0);

    for (int i=0;i<_layers.count();i++)
    {
        Layer *layer = _layers.at(i);

        Rect layerFrame = layer->getFrame();
        Rect screenRect = Rect(0,0,319,239);
        //if (screenRect.intersectsRect(layerFrame))
        {
            //Crop Rectangle to screen
            //cropRectToScreen(layerFrame);

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

void PHDisplay::invalidateRect(Rect&dirtyRect, Rect& invalidationRect, uint16_t color)
{
    //InvalidationRect is in screen shape, but we need view space

    //LOG_VALUE("View-Space:",invalidationRect.toString());
    //LOG_VALUE("Screen-Spcae:",dirtyRect.toString());

    _foregroundLayer->invalidateRect(dirtyRect, invalidationRect);

    //LOG("Sending layer to display");
    for (int i=0;i<_layers.count();i++)
    {
        Layer* layer = _layers.at(i);
        layer->invalidateRect(dirtyRect, invalidationRect);
    }

    //fillRect(dirtyRect.x,0,dirtyRect.width,dirtyRect.height,color);
}

void PHDisplay::setScrollOffset(float scrollOffset)
{
    if (scrollOffset < -(_foregroundLayer->getFrame().width-320))
    {
        scrollOffset = -(_foregroundLayer->getFrame().width-320);
    }
    if (scrollOffset > 0)
    {
        scrollOffset = 0;
    }

    float diffScrollOffset = ceilf(_scrollOffset - scrollOffset);
    if (diffScrollOffset == 0) return;

    //Save scroll offset
    _scrollOffset = scrollOffset;

    //LOG_VALUE("Rect-Width: ",diffScrollOffset);
    //LOG_VALUE("Scroll Offset: ",scrollOffset);

    if (diffScrollOffset > 0)
    {
        //Left
        //Invalidate Layers intersection the right rectangle

        int sx = -scrollOffset - fabsf(diffScrollOffset);
        int sw = fabsf(diffScrollOffset);

        int vx = -scrollOffset + 320 - fabsf(diffScrollOffset);
        int vw = sw;

        Rect invalidationRect(vx,0,vw,240);
        Rect dirtyRect(sx,0,sw,240);
        invalidateRect(dirtyRect,invalidationRect,ILI9341_GREEN);
    }
    else if (diffScrollOffset < 0)
    {
        //Right
        //Invalidate Layers intersecting with the right rectangle

        int sx = -scrollOffset;
        int sw = fabs(diffScrollOffset)+1;

        int vx = -scrollOffset;
        int vw = sw;

        //LOG_VALUE("SX:",sx);
        //LOG_VALUE("SW:",sw);

        Rect invalidationRect(vx,0,vw,240);
        Rect dirtyRect(sx,0,sw,240);
        invalidateRect(dirtyRect,invalidationRect,ILI9341_CYAN);
        //fillRect(sx,0,sw,240,ILI9341_GREEN);
    }
    else
    {
        //Do nothing as nothing has changed
        return;
    }

    //Shift display to the specific frame (this is hardware scrolling)
    if (scrollOffset < 0)
    {
        int numScreens = -scrollOffset/320;
        numScreens += 1;
        scrollOffset += numScreens * 320;
    }
    if (scrollOffset > 319)
    {
        scrollOffset -= 320;
    }

    //LOG_VALUE("Scroll Offset:",scrollOffset);

    Display.setScroll((int)scrollOffset);

}

Rect PHDisplay::visibleRect()
{
    return Rect(-_scrollOffset,0,320,240);
}


void PHDisplay::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    if (_clipRect == NULL)
    {
        ILI9341_t3::drawPixel(x, y, color);
    }
    else
    {
        if (x >= _clipRect->left() && x <= _clipRect->right())
        {
            if (y >= _clipRect->top() && y <= _clipRect->bottom())
            {
                ILI9341_t3::drawPixel(x, y, color);
            }
        }
    }
}

void PHDisplay::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    if (_clipRect == NULL)
    {
        ILI9341_t3::fillRect(x, y, w, h, color);
    }
    else
    {
/*        if (x < _clipRect->left()) x = _clipRect->left();
        if (x+w > _clipRect->right()) w = x - _clipRect->right();
        if (y < _clipRect->top()) y = _clipRect->top();
        if (y+h > _clipRect->bottom()) h = y - _clipRect->bottom();*/

        Rect frame = Rect(x,y,w,h);
        frame = Rect::Intersect(frame,*_clipRect);

        ILI9341_t3::fillRect(frame.x, frame.y, frame.width, frame.height, color);
    }
}

void PHDisplay::drawFontBits(uint32_t bits, uint32_t numbits, uint32_t x, uint32_t y, uint32_t repeat)
{
    // TODO: replace this *slow* code with something fast...
    //Serial.printf("      %d bits at %d,%d: %X\n", numbits, x, y, bits);
    if (bits == 0) return;
    do {
        uint32_t x1 = x;
        uint32_t n = numbits;
#if 1
        do {
            n--;
            drawPixel(x1,y, textbgcolor);
            if (bits & (1 << n)) {
                drawPixel(x1, y, textcolor);
                //Serial.printf("        pixel at %d,%d\n", x1, y);
            }
            x1++;
        } while (n > 0);
#endif
#if 0
        int w = 0;
		do {
			n--;
			if (bits & (1 << n)) {
				w++;
			}
			else if (w > 0) {
				drawFastHLine(x1 - w, y, w, textcolor);
				w = 0;
			}

			x1++;
		} while (n > 0);
		if (w > 0) drawFastHLine(x1 - w, y, w, textcolor);
#endif
        y++;
        repeat--;
    } while (repeat);
}

void PHDisplay::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
    //This function is used by a lot of draw functions
    if (_clipRect == NULL)
    {
        ILI9341_t3::drawFastHLine(x, y, w, color);
    }
    else
    {
        Rect frame = Rect(x,y,w,1);
        frame = Rect::Intersect(frame,*_clipRect);

        ILI9341_t3::drawFastHLine(frame.x,frame.y,frame.width,color);
    }
}

void PHDisplay::setClippingRect(Rect *rect)
{
    _clipRect = rect;
}

void PHDisplay::resetClippingRect()
{
    _clipRect = NULL;
}