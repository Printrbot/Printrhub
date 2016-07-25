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
#include "../core/Application.h"

ProgressBar::ProgressBar(int x, int y, int width, int height):
	View(x,y,width,height)
{
	setMinValue(0);
	setMaxValue(1);
	_value = 0;
	_currentTrackWidth = 0;

	_backgroundColor = Application.getTheme()->getColor(ControlBackgroundColor);
	_trackColor = Application.getTheme()->getColor(SpacerColor);

	_progressLayer = new RectangleLayer(getLeftRect());
	_progressLayer->setBackgroundColor(_backgroundColor);
	addLayer(_progressLayer);

	_trackLayer = new RectangleLayer(getRightRect());
	_trackLayer->setBackgroundColor(_trackColor);
	addLayer(_trackLayer);
}

ProgressBar::ProgressBar(Rect frame):
	ProgressBar(frame.x,frame.y,frame.width,frame.height)
{

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

float ProgressBar::getFraction()
{
	return (float)(_value - _minValue)/(float)(_maxValue-_minValue);
}

Rect ProgressBar::getLeftRect()
{
	return Rect(_frame.x,_frame.y,_frame.width*getFraction(),_frame.height);
}

Rect ProgressBar::getRightRect()
{
	Rect leftRect = getLeftRect();
	return Rect(leftRect.right()+1,_frame.y,_frame.right()-leftRect.right()-1,_frame.height);
}

float ProgressBar::getValue()
{
	return _value;
}

void ProgressBar::setNeedsDisplay()
{
	if (_progressLayer == NULL || _trackLayer == NULL) return;

	_progressLayer->setFrame(getLeftRect());
	_trackLayer->setFrame(getRightRect());

	View::setNeedsDisplay();
}

void ProgressBar::setValue(float value)
{
	if (this->_value == value) return;
	if (value > _maxValue) value = _maxValue;
	if (value < _minValue) value = _minValue;

	this->_value = value;

	//Only refresh if there is something to draw
	int trackWidth = (int)((float)_frame.width * value);
	if (trackWidth != _currentTrackWidth)
	{
		setNeedsDisplay();
	}
	_currentTrackWidth = trackWidth;
}

void ProgressBar::setLimits(float minValue, float maxValue)
{
	setMinValue(minValue);
	setMaxValue(maxValue);
}
