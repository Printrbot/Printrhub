//
// Created by Phillip Schuster on 09.07.16.
//

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

	_progressLayer->setFrame(getLeftRect(),false);
	_trackLayer->setFrame(getRightRect(),false);
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
