//
// Created by Phillip Schuster on 12.05.16.
//

#ifndef TEENSY_GRAPHICSCONTEXT_H
#define TEENSY_GRAPHICSCONTEXT_H

#include "Arduino.h"
#include "../../drivers/SD/SD.h"

class GraphicsContext
{
public:
	virtual void drawBitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* bitmap, uint16_t xs, uint16_t ys, uint16_t ws, uint16_t hs) = 0;
	virtual void drawFileBitmapByColumn(uint16_t x, uint16_t y, uint16_t w, uint16_t h, File *file, uint16_t xs, uint16_t ys, uint16_t ws, uint16_t hs) = 0;
	virtual void drawMaskedBitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t* bitmap, uint16_t xs, uint16_t ys, uint16_t ws, uint16_t hs, uint16_t foregroundColor, uint16_t backgroundColor) = 0;
	virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) = 0;
	virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) = 0;
};


#endif //TEENSY_GRAPHICSCONTEXT_H
