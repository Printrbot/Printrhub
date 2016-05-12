//
// Created by Phillip Schuster on 12.05.16.
//

#ifndef TEENSY_IMAGEBUFFER_H
#define TEENSY_IMAGEBUFFER_H

#include "Arduino.h"
#include "../../drivers/SD/SD.h"

class ImageBuffer
{
#pragma mark Constructor
public:
	ImageBuffer(uint16_t* buffer, uint16_t width, uint16_t height);
	ImageBuffer(uint16_t width, uint16_t height);
	virtual ~ImageBuffer();

#pragma mark Draw functions
	virtual void drawPixel(int16_t x, int16_t y, uint16_t color);
	virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
	virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
	virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
	virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
	virtual void drawBitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* bitmap, uint16_t xs, uint16_t ys, uint16_t ws, uint16_t hs);
	virtual void drawFileBitmapByColumn(uint16_t x, uint16_t y, uint16_t w, uint16_t h, File *file, uint16_t xs, uint16_t ys, uint16_t ws, uint16_t hs);
	virtual void drawMaskedBitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t* bitmap, uint16_t xs, uint16_t ys, uint16_t ws, uint16_t hs, uint16_t foregroundColor, uint16_t backgroundColor);
	virtual void setTranslation(int16_t tx, int16_t ty);

#pragma mark Getter/Setter
	virtual uint16_t* getData() const { return _data; };
	virtual uint16_t getWidth() const { return _width; };
	virtual uint16_t getHeight() const { return _height; };

#pragma mark Member Variables
private:
	uint16_t* _data;
	uint16_t _width;
	uint16_t _height;
	bool _manageBuffer;
	uint16_t _tx;
	uint16_t _ty;
};


#endif //TEENSY_IMAGEBUFFER_H
