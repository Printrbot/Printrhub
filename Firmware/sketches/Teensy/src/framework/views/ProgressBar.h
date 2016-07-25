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

#ifndef TEENSYCMAKE_PROGRESSBAR_H
#define TEENSYCMAKE_PROGRESSBAR_H

#include "View.h"
#include "../layers/RectangleLayer.h"

class ProgressBar: public View
{
#pragma mark Constructor
public:
	ProgressBar(Rect frame);
	ProgressBar(int x, int y, int width, int height);

#pragma mark Getter/Setter
	void setMinValue(float minValue);
	void setMaxValue(float maxValue);
	void setLimits(float minValue, float maxValue);
	float getValue();
	void setValue(float value);
	uint16_t getTrackColor() const { return _trackColor; }
	void setTrackColor(uint16_t trackColor) { _trackColor = trackColor; }

#pragma mark View
	virtual String getDescription() override;
	virtual void setNeedsDisplay() override;

#pragma mark Member Functions
private:
	Rect getLeftRect();
	Rect getRightRect();
	float getFraction();

#pragma mark Member Variables
private:
	float _minValue;
	float _maxValue;
	float _value;
	RectangleLayer* _progressLayer;
	RectangleLayer* _trackLayer;
	uint16_t _trackColor;
	int _currentTrackWidth;
};


#endif //TEENSYCMAKE_PROGRESSBAR_H
