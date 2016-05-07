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

//TODO: Ported from Little Helper needs work to be functional with Printrhub Layout System
class ProgressBar: public View
{
public:
	ProgressBar(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
	virtual void draw();

	void setMinValue(float minValue);
	void setMaxValue(float maxValue);
	void setLimits(float minValue, float maxValue);
	float getValue();
	void setValue(float value);

	virtual String getDescription() override;

private:
	float _minValue;
	float _maxValue;
	float _value;
};


#endif //TEENSYCMAKE_PROGRESSBAR_H
