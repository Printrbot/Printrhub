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

#ifndef TEENSYCMAKE_ICONS_H
#define TEENSYCMAKE_ICONS_H

#include <initializer_list>
#include "Arduino.h"

#define ICON_WIDTH 8
#define ICON_HEIGHT 8

class Icon
{
public:
	Icon(const uint8_t* pixelData);

	uint8_t getWidth();
	uint8_t getHeight();
	const uint8_t* getPixelData();

private:
	const uint8_t* _pixelData;
};

extern Icon SlopeRaising;
extern Icon SlopeFalling;
extern Icon SlopeBoth;


#endif //TEENSYCMAKE_ICONS_H
