//
// Created by Phillip Schuster on 12.05.16.
//

#include "ImageBuffer.h"
#include "Application.h"

ImageBuffer::ImageBuffer(uint16_t* buffer, uint16_t width, uint16_t height)
{
	_manageBuffer = false;
	_data = buffer;
	_width = width;
	_height = height;
	_tx = 0;
	_ty = 0;
}


ImageBuffer::ImageBuffer(uint16_t width, uint16_t height)
{
	_data = (uint16_t*)malloc(sizeof(uint16_t)*width*height);
	_width = width;
	_height = height;
	_manageBuffer = true;
	_tx = 0;
	_ty = 0;
}


ImageBuffer::~ImageBuffer()
{
	if (_manageBuffer)
	{
		free(_data);
	}
}


void ImageBuffer::drawPixel(int16_t x, int16_t y, uint16_t color)
{
	x += _tx;
	y += _ty;

	if (x >= _width) return;
	if (x < 0) return;
	if (y >= _height) return;
	if (y < 0) return;

	_data[x*_height+y] = color;
}


void ImageBuffer::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	for (int x1=x;x1<x+w;x1++)
	{
		for (int y1=y;y1<y+h;y1++)
		{
			drawPixel(x1,y1,color);
		}
	}
}


void ImageBuffer::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	for (int x1=x;x1<x+w;x1++)
	{
		drawPixel(x1,y,color);
		drawPixel(x1,y+h-1,color);
	}

	for (int y1=y;y1<y+h;y1++)
	{
		drawPixel(x,y1,color);
		drawPixel(x+w-1,y1,color);
	}
}


void ImageBuffer::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
	for (int x1=x;x1<x+w;x1++)
	{
		drawPixel(x1,y,color);
	}
}

void ImageBuffer::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
	for (int y1=y;y1<y+h;y1++)
	{
		drawPixel(x,y1,color);
	}
}


void ImageBuffer::drawBitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *bitmap, uint16_t xs,
                             uint16_t ys, uint16_t ws, uint16_t hs)
{
	for (uint16_t xb=0;xb<w;xb++)
	{
		for (uint16_t yb=0;yb<h;yb++)
		{
			uint16_t color = bitmap[(xb+xs)*hs+(yb+ys)];
			drawPixel(xb,yb,color);
		}
	}
}

void ImageBuffer::drawFileBitmapByColumn(uint16_t x, uint16_t y, uint16_t w, uint16_t h, File *file, uint16_t xs,
                                         uint16_t ys, uint16_t ws, uint16_t hs)
{
	uint16_t buffer[320];
	for (uint16_t xb=0;xb<w;xb++)
	{
		file->seek(((xb+xs)*hs)*sizeof(uint16_t));
		file->read(buffer,sizeof(uint16_t)*hs);

		for (uint16_t yb=0;yb<h;yb++)
		{
			drawPixel(x+xb,y+yb,buffer[xb+xs]);
		}
	}
}

void ImageBuffer::drawMaskedBitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t *bitmap, uint16_t xs,
                                   uint16_t ys, uint16_t ws, uint16_t hs, uint16_t foregroundColor,
                                   uint16_t backgroundColor)
{
	for (uint16_t xb=0;xb<w;xb++)
	{
		for (uint16_t yb=0;yb<h;yb++)
		{
			uint8_t byte = bitmap[((xb+xs)*hs+(yb+ys))/8];
			uint8_t slot = ((xb+xs)*hs+(yb+ys)) % 8;

			bool bit = (byte >> slot) & 1;

			uint16_t color = foregroundColor;
			if (bit)
			{
				color = backgroundColor;
			}

			drawPixel(xb,yb,color);
		}
	}
}


void ImageBuffer::setTranslation(int16_t tx, int16_t ty)
{
	_tx = tx;
	_ty = ty;
}


