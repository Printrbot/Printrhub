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

#ifndef TEENSYCMAKE_LABELVIEW_H
#define TEENSYCMAKE_LABELVIEW_H

#include "View.h"
#include "Application.h"

#define TEXTALIGN_LEFT 0
#define TEXTALIGN_CENTERED 1
#define TEXTALIGN_RIGHT 2

class LabelView: public View
{
public:
	LabelView(String text, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

	void setText(String text);
	void setTextAlign(uint8_t textAlign);
	void setTextColor(uint16_t color);
	void setFont(const ILI9341_t3_font_t &font);

private:
	String _text;
	uint8_t _textAlign;
	const ILI9341_t3_font_t *_font;
	uint16_t _textColor;
public:
	virtual void draw() override;
};


#endif //TEENSYCMAKE_LABELVIEW_H
