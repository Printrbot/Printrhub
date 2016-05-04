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

#include "ProgressBar.h"
#include "Application.h"

ProgressBar::ProgressBar(uint16_t x, uint16_t y, uint16_t width, uint16_t height):
	View(x,y,width,height)
{
	setMinValue(0);
	setMaxValue(1);
	_value = 0;
}

void ProgressBar::draw()
{
//	display.drawRoundRect(_frame.x,_frame.y,_frame.width,_frame.height,3,Application.getTheme()->getPrimaryColor());

	float width = (_frame.width * this->_value)-1;
	if (width > _frame.width) width = _frame.width;
	if (width < 6) width = 6;
	LOG_VALUE("Progress-Bar Width",width);
	//display.fillRoundRect(_frame.x+1,_frame.y,(uint8_t)width,_frame.height,3,Application.getTheme()->getPrimaryColor());
	//display.fillRect(x+1,_frame.y+1,4,_frame.height-2,ST7735_CYAN);
}

String ProgressBar::getDescription()
{
	return "Progressbar";
}

void ProgressBar::setMinValue(float minValue)
{
	_minValue = minValue;
}

void ProgressBar::setMaxValue(float maxValue)
{
	_maxValue = maxValue;
}

float ProgressBar::getValue()
{
	return _value;
}

void ProgressBar::setValue(float value)
{
	if (this->_value == value) return;
	if (value > _maxValue) value = _maxValue;
	if (value < _minValue) value = _minValue;

	this->_value = value;

	setNeedsDisplay();
}

void ProgressBar::setLimits(float minValue, float maxValue)
{
	setMinValue(minValue);
	setMaxValue(maxValue);
}
