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

LabelView::LabelView(String text, uint16_t x, uint16_t y, uint16_t width, uint16_t height): View(x,y,width,height)
{
	_font = &Arial_8;
	_textColor = RGB565(255,255,255);
	_textAlign = TEXTALIGN_LEFT;
	_text = text;
}

void LabelView::setText(String text)
{
	_text = text;
	setNeedsDisplay();
}

void LabelView::draw()
{
/*	display.fillRect(_frame,RGB565(0,0,0));

	display.setTextColor(_textColor);
	display.setFont(_font);

	int x = _frame.x;
	int y = _frame.y + ((_frame.height - display.calcTextHeight(_text,_font)) / 2);

	if (_textAlign == TEXTALIGN_CENTERED)
	{
		x = _frame.x + ((_frame.width - display.calcTextWidth(_text,_font)) / 2);
	}
	else if (_textAlign == TEXTALIGN_RIGHT)
	{
		x = _frame.right() - display.calcTextWidth(_text,_font);
	}

	display.setCursor(x,y);
	display.print(_text);*/
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

void LabelView::setFont(const ILI9341_t3_font_t& font)
{
	_font = &font;
	setNeedsDisplay();
}
