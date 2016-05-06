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
	_font = &PTSansNarrow_20;
	_textColor = Application.getTheme()->getSecondaryColor1();
	_backgroundColor = ILI9341_WHITE;
	_name = "LabelView";
	_layer = NULL;
}

LabelView::LabelView(String text, Rect frame):
	View(frame)
{
	_textAlign = TEXTALIGN_CENTERED;
	_verticalTextAlign = TEXTALIGN_CENTERED;
	_text = text;
	_font = &PTSansNarrow_20;
	_textColor = Application.getTheme()->getSecondaryColor1();
	_backgroundColor = ILI9341_WHITE;
	_name = "LabelView";
	_layer = NULL;
}

void LabelView::display()
{
	_layer = new TextLayer(_frame);
	_layer->setForegroundColor(_textColor);
	_layer->setBackgroundColor(_backgroundColor);
	_layer->setText(_text);
	_layer->setFont(_font);
	_layer->setTextAlign(_textAlign);
	_layer->setVerticalTextAlign(_verticalTextAlign);
	addLayer(_layer);

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

	if (_layer != NULL)
	{
		_layer->setTextAlign(textAlign);
	}

	setNeedsDisplay();
}

void LabelView::setTextColor(uint16_t color)
{
	if (_textColor == color) return;
	_textColor = color;

	if (_layer != NULL)
	{
		_layer->setForegroundColor(color);
	}

	setNeedsDisplay();
}

void LabelView::setVerticalTextAlign(uint8_t verticalTextAlign)
{
	if (_verticalTextAlign == verticalTextAlign) return;
	_verticalTextAlign = verticalTextAlign;

	if (_layer != NULL)
	{
		_layer->setVerticalTextAlign(verticalTextAlign);
	}

	setNeedsDisplay();
}

void LabelView::setFont(const ILI9341_t3_font_t* font)
{
	if (_font == font) return;
	_font = font;

	if (_layer != NULL)
	{
		_layer->setFont(font);
	}

	setNeedsDisplay();
}

const ILI9341_t3_font_t *LabelView::getFont()
{
	return _font;
}
