//
// Created by Phillip Schuster on 09.07.16.
//

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
	void setBackgroundColor(uint16_t backgroundColor) { _backgroundColor = backgroundColor; _progressLayer->setBackgroundColor(_backgroundColor); }

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
