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
#include <font_Arial.h>

LabelView::LabelView(String text, uint16_t x, uint16_t y, uint16_t width, uint16_t height):
		View(x,y,width,height)
{
	_textAlign = TEXTALIGN_LEFT;
	_text = text;

	_layer = new TextLayer(Rect(x,y,width,height));
	_layer->setText(&_text);
	addLayer(_layer);
}

LabelView::LabelView(String text, Rect frame):
	View(frame)
{
	_textAlign = TEXTALIGN_LEFT;
	_text = text;

	_layer = new TextLayer(frame);
	_layer->setText(&_text);
	addLayer(_layer);
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
	if (_layer->getForegroundColor() == color) return;
	_layer->setForegroundColor(color);
	setNeedsDisplay();
}

void LabelView::setFont(ILI9341_t3_font_t* font)
{
	_layer->setFont(font);
	setNeedsDisplay();
}
