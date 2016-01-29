/*
 * Little Helper -- your best friend in your lab
 * http://www.appfruits.com/littlehelper
 *
 * This software is part of the firmware running Little Helper. You may use, copy, 
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software 
 * but you must include this copyright notice and this permission in all copies or 
 * substantial portions of the Software.
 *
 * Appfruits invests time and resources to make this open source. Please support us
 * and the open-source hardware initiative.
 * 
 * Copyright 2014-2015 Phillip Schuster (@appfruits)
 * http://www.appfruits.com
 * 
 * MIT-License
 */

#include "LabelView.h"
#include "Application.h"
#include "ILI9341_t3.h"

LabelView::LabelView(String text, uint16_t x, uint16_t y, uint16_t width, uint16_t height):
		View(x,y,width,height)
{
	_textAlign = TEXTALIGN_CENTERED;
	_verticalTextAlign = TEXTALIGN_CENTERED;
	_text = text;
	_font = &Arial_20;
	_textColor = ILI9341_BLACK;
	_backgroundColor = ILI9341_WHITE;
}

LabelView::LabelView(String text, Rect frame):
	View(frame)
{
	_textAlign = TEXTALIGN_CENTERED;
	_verticalTextAlign = TEXTALIGN_CENTERED;
	_text = text;
	_font = &Arial_20;
	_textColor = ILI9341_BLACK;
	_backgroundColor = ILI9341_WHITE;
}

void LabelView::display()
{
	_backgroundLayer = new RectangleLayer(_frame);
	_backgroundLayer->setBackgroundColor(_backgroundColor);
	addLayer(_backgroundLayer);

	//Align the text layer
	Rect frame = _frame;
	if (_verticalTextAlign == TEXTALIGN_TOP)
	{
		frame.y += 1;
	}
	else if (_verticalTextAlign == TEXTALIGN_BOTTOM)
	{
		frame.y = frame.bottom()-_font->cap_height - 1;
	}
	else if (_verticalTextAlign == TEXTALIGN_CENTERED)
	{
		frame.y += (frame.height - _font->cap_height) / 2;
	}
	frame.height = _font->cap_height;

	//TODO: Add code for horizontal alignment

	//TODO: Fix this bug. If the inner layers left and right ly on the outer layers boundaries
	//there is a strange render bug. Inseting by one pixels helps though and doesn't matter here
	frame.x += 1;
	frame.width -= 2;

	if (_textAlign == TEXTALIGN_CENTERED)
	{
		uint32_t width = Display.textWidth(_font, _text);
		frame.x += (frame.width - width)/2;
		frame.width = width;
	}
	else if (_textAlign == TEXTALIGN_RIGHT)
	{
		uint32_t width = Display.textWidth(_font, _text);
		frame.x += (frame.width - width);
		frame.width = width;
	}

	_layer = new TextLayer(frame);
	_layer->setForegroundColor(_textColor);
	_layer->setBackgroundColor(_backgroundColor);
	_layer->setText(&_text);
	addLayer(_layer);

	_backgroundLayer->splitWithRect(_layer->getFrame());

	View::display();
}

void LabelView::setText(String text)
{
	_text = text;
	setNeedsDisplay();
}

void LabelView::setTextAlign(uint8_t textAlign)
{
	if (_textAlign == textAlign) return;
	_textAlign = textAlign;
	setNeedsDisplay();
}

void LabelView::setTextColor(uint16_t color)
{
	if (_textColor == color) return;
	_textColor = color;
	setNeedsDisplay();
}

void LabelView::setVerticalTextAlign(uint8_t verticalTextAlign)
{
	if (_verticalTextAlign == verticalTextAlign) return;
	_verticalTextAlign = verticalTextAlign;
	setNeedsDisplay();
}

void LabelView::setFont(const ILI9341_t3_font_t* font)
{
	if (_font == font) return;
	_font = font;
	setNeedsDisplay();
}
