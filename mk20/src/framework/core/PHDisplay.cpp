//
// Created by Phillip Schuster on 17.01.16.
//

#include "PHDisplay.h"
#include "Application.h"
#include <SPI.h>
#include <Wiring.h>
#include <Arduino.h>
#include "SD.h"

#define SPICLOCK 30000000

PHDisplay::PHDisplay(uint8_t _CS, uint8_t _DC, uint8_t _RST, uint8_t _MOSI, uint8_t _SCLK, uint8_t _MISO):
        ILI9341_t3(_CS,_DC,_RST,_MOSI,_SCLK,_MISO),
        _foregroundLayer(NULL),
        _backgroundLayer(NULL)
{
    setupBuffers();

    _needsLayout = false;
    _needsDisplay = false;
    _fixedBackgroundLayer = NULL;

    debug = false;
    _transparentText = false;
    
    _scrollOffset = 0;
    _scrollInsetLeft = 0;
    _scrollInsetRight = 0;
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

void PHDisplay::setFixedBackgroundLayer(Layer *layer)
{
    layer->setNeedsDisplay();
    _fixedBackgroundLayer = layer;
}

void PHDisplay::setupBuffers()
{
    if (_foregroundLayer != NULL)
    {
        delete _foregroundLayer;
        _foregroundLayer = NULL;
    }
    if (_backgroundLayer != NULL)
    {
        delete _backgroundLayer;
        _backgroundLayer = NULL;
    }

    _foregroundLayer = new RectangleLayer(Rect(0,0,650,240));
    _foregroundLayer->setBackgroundColor(ILI9341_WHITE);
    _foregroundLayer->setStrokeWidth(0);

    _backgroundLayer = new RectangleLayer(Rect(0,0,650,240));
    _backgroundLayer->setBackgroundColor(ILI9341_WHITE);
    _backgroundLayer->setStrokeWidth(0);
}

void PHDisplay::clear()
{
    setupBuffers();
    _layers.clear(false);
    _needsLayout = true;
    _autoLayout = true;
    _fixedBackgroundLayer = NULL;
    _scrollOffset = 0;
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
    if (_fixedBackgroundLayer != NULL) return;

    //Now delete the old background layer and swap foreground to background
    delete _backgroundLayer;
    _backgroundLayer = _foregroundLayer;

    //SceneController* currentScene = Application.currentScene();

    //Get Max Layer Width
    Rect bounds = Rect(0,0,getLayoutWidth(),240);
    for (int i=0;i<_layers.count();i++)
    {
        Layer *layer = _layers.at(i);
        if (layer->getContext() == DisplayContext::Fixed) continue;

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

    //We have calculated the width for scrolling, if we don't use auto layout stop work now
    _needsLayout = false;
    if (!_autoLayout) return;

    for (int i=0;i<_layers.count();i++)
    {
        Layer *layer = _layers.at(i);
        if (layer->getContext() == DisplayContext::Fixed) continue;

        Rect layerFrame = layer->getFrame();
        _foregroundLayer->splitWithRect(layerFrame);
    }

    _needsLayout = false;
}

void PHDisplay::dispatch()
{
    if (!_needsDisplay) return;

    //LOG("Sending background to display");
    if (_fixedBackgroundLayer != NULL)
    {
        _fixedBackgroundLayer->display();
    }
    else
    {
        if (_autoLayout)
        {
            _foregroundLayer->display(_backgroundLayer);
        }
    }

    //LOG("Sending layer to display");
    for (int i=0;i<_layers.count();i++)
    {
        Layer* layer = _layers.at(i);
        layer->display();
    }

    _needsDisplay = false;
}

void PHDisplay::drawBitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *bitmap, uint16_t xs, uint16_t ys, uint16_t ws, uint16_t hs)
{
/*    SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
    setAddr(x, y, x+w-1, y+h-1);
    writecommand_cont(ILI9341_RAMWR);*/

    if (_lockBuffer != NULL)
    {
        _lockBuffer->drawBitmap(x,y,w,h,bitmap,xs,ys,ws,hs);
        return;
    }

    for (uint16_t xb=0;xb<w;xb++)
    {
        SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
        setAddr(x+xb, y, x+xb, y+h-1);
        writecommand_cont(ILI9341_RAMWR);

        for (uint16_t yb=0;yb<h;yb++)
        {
            if (yb == h-1)
            {
                //Last pixel
                writedata16_last(bitmap[(xb+xs)*hs+(yb+ys)]);
            }
            else
            {
                //All other pixels
                writedata16_cont(bitmap[(xb+xs)*hs+(yb+ys)]);
            }
        }
        SPI.endTransaction();
    }
}


void PHDisplay::drawMaskedBitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t *bitmap, uint16_t xs,
                                 uint16_t ys, uint16_t ws, uint16_t hs, uint16_t foregroundColor,
                                 uint16_t backgroundColor)
{
    if (_lockBuffer != NULL)
    {
        _lockBuffer->drawMaskedBitmap(x,y,w,h,bitmap,xs,ys,ws,hs,foregroundColor,backgroundColor);
        return;
    }

    //TODO: This code will fail if ys > 0 and hs < h as it's not implemented correctly. As it's not needed by the current firmware I leave this comment and resolve it later
    for (uint16_t xb=0;xb<w;xb++)
    {
        SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
        setAddr(x+xb, y, x+xb, y+h-1);
        writecommand_cont(ILI9341_RAMWR);

        for (uint16_t yb=0;yb<h;yb++)
        {
            //uint8_t byte = bitmap[((yb+ys)*ws+(xb+xs))/8];
            uint8_t byte = bitmap[((xb+xs)*hs+(yb+ys))/8];
            uint8_t slot = ((xb+xs)*hs+(yb+ys)) % 8;

            bool bit = (byte >> slot) & 1;

            uint16_t color = foregroundColor;
            if (bit)
            {
                color = backgroundColor;
            }

            if (yb == h-1)
            {
                //Last pixel
                writedata16_last(color);
            }
            else
            {
                //All other pixels
                writedata16_cont(color);
            }
        }

        SPI.endTransaction();
    }
}


void PHDisplay::drawFileBitmapByColumn(uint16_t x, uint16_t y, uint16_t w, uint16_t h, File *file, uint16_t xs,
                                       uint16_t ys, uint16_t ws, uint16_t hs, uint32_t byteOffset)
{
    if (_lockBuffer != NULL)
    {
        _lockBuffer->drawFileBitmapByColumn(x,y,w,h,file,xs,ys,ws,hs, byteOffset);
        return;
    }

    //TODO: This code will fail if ys > 0 and hs < h as it's not implemented correctly. As it's not needed by the current firmware I leave this comment and resolve it later
    uint16_t buffer[320];
    for (uint16_t xb=0;xb<w;xb++)
    {
        file->seek((((xb+xs)*hs)*sizeof(uint16_t))+byteOffset);

        file->read(buffer,sizeof(uint16_t)*hs);

        SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
        setAddr(x+xb, y, x+xb, y+h-1);
        writecommand_cont(ILI9341_RAMWR);

        for (uint16_t yb=0;yb<h;yb++)
        {
            if (yb == h-1)
            {
                //Last pixel
                writedata16_last(buffer[yb+ys]);
            }
            else
            {
                //All other pixels
                writedata16_cont(buffer[yb+ys]);
            }
            //drawPixel(x+xb,y+yb,buffer[xb+xs]);
        }
        SPI.endTransaction();
    }
}

void PHDisplay::setNeedsLayout()
{
    _needsLayout = true;
}

void PHDisplay::setNeedsDisplay()
{
    _needsDisplay = true;
}

bool PHDisplay::willRefresh()
{
    return (_needsDisplay);
}

void PHDisplay::invalidateRect(Rect &invalidationRect, int scrollOffset, int deltaScrollOffset)
{
    //LOG_VALUE("View-Space:",invalidationRect.toString());
    //LOG_VALUE("Screen-Spcae:",dirtyRect.toString());

    if (deltaScrollOffset > 0)
    {
        int so = mapScrollOffset(scrollOffset-invalidationRect.width-1);
        Display.setScroll(so);

        if (_autoLayout)
        {
            _foregroundLayer->invalidateRect(invalidationRect);
        }

        //LOG("Sending layer to display");
        for (int i=0;i<_layers.count();i++)
        {
            Layer* layer = _layers.at(i);
            if (layer->getContext() == DisplayContext::Fixed) continue;
            layer->invalidateRect(invalidationRect);
        }
    }
    else
    {
        int so = mapScrollOffset(scrollOffset);
        Display.setScroll(so);

        if (_autoLayout)
        {
            _foregroundLayer->invalidateRect(invalidationRect);
        }

        //LOG("Sending layer to display");
        for (int i=0;i<_layers.count();i++)
        {
            Layer* layer = _layers.at(i);
            if (layer->getContext() == DisplayContext::Fixed) continue;
            layer->invalidateRect(invalidationRect);
        }
    }

    //fillRect(dirtyRect.x,0,dirtyRect.width,dirtyRect.height,color);
}

void PHDisplay::setScrollInsets(uint16_t left, uint16_t right)
{
    _scrollInsetLeft = left;
    _scrollInsetRight = right;

    if (getRotation() == ILI9341_ORIENTATION_LANDSCAPE_LEFT)
    {
        setScrollArea(right,ILI9341_TFTHEIGHT-right-left,left);
    }
    else
    {
        setScrollArea(left,ILI9341_TFTHEIGHT-right-left,right);
    }
}

uint16_t PHDisplay::getLayoutWidth()
{
    return ILI9341_TFTHEIGHT - (_scrollInsetLeft + _scrollInsetRight);
}


uint16_t PHDisplay::getLayoutStart()
{
    if (getRotation() == ILI9341_ORIENTATION_LANDSCAPE_LEFT)
    {
        return _scrollInsetLeft;
    }
    else
    {
        return _scrollInsetRight;
    }
}

int PHDisplay::mapScrollOffset(int so)
{
    //Shift display to the specific frame (this is hardware scrolling)
    if (so < 0)
    {
        int numScreens = -so/(getLayoutWidth());
        numScreens += 1;
        so += numScreens * getLayoutWidth();
    }
    if (so >= getLayoutWidth())
    {
        so -= getLayoutWidth();
    }

    return so;
}


float PHDisplay::clampScrollTarget(float scrollTarget)
{
    if (scrollTarget < -((_foregroundLayer->getFrame().width-1)-getLayoutWidth()))
    {
        scrollTarget = -((_foregroundLayer->getFrame().width-1)-getLayoutWidth());
    }
    if (scrollTarget > 0)
    {
        scrollTarget = 0;
    }

    return scrollTarget;
}

void PHDisplay::setScrollOffset(float scrollOffset, bool update)
{
   // LOG_VALUE("Layout-Width: ",(_foregroundLayer->getFrame().width-1));
    if (scrollOffset < -((_foregroundLayer->getFrame().width-1)-getLayoutWidth()))
    {
        scrollOffset = -((_foregroundLayer->getFrame().width-1)-getLayoutWidth());
    }
    if (scrollOffset > 0)
    {
        scrollOffset = 0;
    }

    int oldScrollOffset = (int)_scrollOffset;
    int newScrollOffset = (int)scrollOffset;
    int diffScrollOffset = oldScrollOffset - newScrollOffset;
    //if (diffScrollOffset == 0) return;

    //Save scroll offset
    _scrollOffset = scrollOffset;

    //LOG_VALUE("Rect-Width: ",diffScrollOffset);
    //LOG_VALUE("Scroll Offset: ",scrollOffset);

    //LOG_VALUE("Scroll Offset:",scrollOffset);

    if (diffScrollOffset > 0)
    {
        //Left
        //Invalidate Layers intersection the right rectangle

        int sx = -newScrollOffset - abs(diffScrollOffset);
        int sw = diffScrollOffset;

        int vx = -newScrollOffset + getLayoutWidth() - diffScrollOffset;
        int vw = sw;

        if (vw > 0)
        {
            if (update)
            {
                Rect invalidationRect(vx,0,vw,240);
                invalidateRect(invalidationRect, newScrollOffset, diffScrollOffset);
            }
        }
    }
    else if (diffScrollOffset < 0)
    {
        //Right
        //Invalidate Layers intersecting with the right rectangle

        int sx = -newScrollOffset;
        int sw = abs(diffScrollOffset)+1;

        int vx = -newScrollOffset;
        int vw = sw;

        //LOG_VALUE("SX:",sx);
        //LOG_VALUE("SW:",sw);

        if (vw > 0)
        {
            if (update)
            {
                Rect invalidationRect(vx,0,vw,240);
                invalidateRect(invalidationRect, newScrollOffset, diffScrollOffset);
            }
        }
        //fillRect(sx,0,sw,240,ILI9341_GREEN);
    }
    else
    {
        //Do nothing as nothing has changed
        return;
    }
}

Rect PHDisplay::visibleRect()
{
    return Rect(-_scrollOffset,0,getLayoutWidth(),240);
}


void PHDisplay::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    //Locked with image buffer, draw into the buffer
    if (_lockBuffer != NULL)
    {
        _lockBuffer->drawPixel(x,y,color);
        return;
    }

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
    //Locked with image buffer, draw into the buffer
    if (_lockBuffer != NULL)
    {
        LOG("fillRect on buffer");
        _lockBuffer->fillRect(x,y,w,h,color);
        return;
    }

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


void PHDisplay::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    //Locked with image buffer, draw into the buffer
    if (_lockBuffer != NULL)
    {
        LOG("drawRect on buffer");
        _lockBuffer->drawRect(x,y,w,h,color);
        return;
    }

    if (_clipRect == NULL)
    {
        ILI9341_t3::drawRect(x, y, w, h, color);
    }
    else
    {
        Rect frame = Rect(x,y,w,h);
        frame = Rect::Intersect(frame,*_clipRect);

        ILI9341_t3::drawRect(frame.x, frame.y, frame.width, frame.height, color);
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
            if (bits & (1 << n)) {
                drawPixel(x1, y, textcolor);
                //Serial.printf("        pixel at %d,%d\n", x1, y);
            }
            else
            {
                if (!_transparentText)
                {
                    drawPixel(x1,y, textbgcolor);
                }
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
    //Locked with image buffer, draw into the buffer
    if (_lockBuffer != NULL)
    {
        _lockBuffer->drawFastHLine(x,y,w,color);
        return;
    }

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

void PHDisplay::fadeOut()
{
    if (debug)
    {
        fadeIn();
        return;
    }

    for (int i=128;i>=0;i--)
    {
        analogWrite(TFT_BACKLIGHT_PWM,i);
        delay(1);
    }
}

void PHDisplay::fadeIn()
{
    for (int i=0;i<=128;i++)
    {
        analogWrite(TFT_BACKLIGHT_PWM,i);
        delay(1);
    }
}


void PHDisplay::debugLayer(Layer *layer, bool fill, uint16_t color, bool waitForTap)
{
    if (fill)
    {
        fillRect(layer->getFrame().x+layer->getOriginX(),layer->getFrame().y,layer->getFrame().width,layer->getFrame().height,color);
    }
    else
    {
        drawRect(layer->getFrame().x+layer->getOriginX(),layer->getFrame().y,layer->getFrame().width,layer->getFrame().height,color);
    }

    if (!waitForTap) return;
    this->waitForTap();
}


void PHDisplay::waitForTap()
{
    while (Touch.touched())
    {
        delay(10);
    };

    while(!Touch.touched())
    {
        delay(10);
    }
    while (Touch.touched())
    {
        delay(10);
    };
}


void PHDisplay::lockBuffer(ImageBuffer *imageBuffer)
{
    LOG("Locking Buffer");
    _lockBuffer = imageBuffer;
}

void PHDisplay::unlock()
{
    LOG("Releasing Buffer");
    _lockBuffer = NULL;
}


Rect PHDisplay::prepareRenderFrame(const Rect proposedRenderFrame, DisplayContext context)
{
    if (_lockBuffer != NULL) return proposedRenderFrame;

    Rect frame = proposedRenderFrame;
    //Map layer frame to display space
    if (context == DisplayContext::Scrolling)
    {
        //Transform to screen space
        if (frame.x >= Display.getLayoutWidth())
        {
            frame.x = frame.x % Display.getLayoutWidth();
            frame.x += Display.getLayoutStart();
        }
        else
        {
            frame.x += Display.getLayoutStart();
        }
    }

    return frame;
}


void PHDisplay::invalidateRect(Rect invalidationRect)
{
    LOG_VALUE("Invalidating Rect",invalidationRect.toString());

    if (_autoLayout)
    {
        _foregroundLayer->invalidateRect(invalidationRect);
    }

    //LOG("Sending layer to display");
    for (int i=0;i<_layers.count();i++)
    {
        Layer* layer = _layers.at(i);
        if (layer->getContext() == DisplayContext::Fixed) continue;
        layer->invalidateRect(invalidationRect);
    }
}


void PHDisplay::drawImageBuffer(ImageBuffer *imageBuffer, Rect renderFrame)
{
    drawBitmap(renderFrame.x,renderFrame.y,renderFrame.width,renderFrame.height,imageBuffer->getData(),0,0,imageBuffer->getWidth(),imageBuffer->getHeight());
    //fillRect(renderFrame.x,renderFrame.y,renderFrame.width,renderFrame.height,ILI9341_PINK);
}


void PHDisplay::disableAutoLayout()
{
    _autoLayout = false;
}
