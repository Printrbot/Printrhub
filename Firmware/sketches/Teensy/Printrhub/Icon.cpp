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

#include "Icon.h"

Icon::Icon(const uint8_t* pixelData):
	_pixelData(pixelData)
{

}


uint8_t Icon::getWidth()
{
	return ICON_WIDTH;
}

uint8_t Icon::getHeight()
{
	return ICON_HEIGHT;
}


const uint8_t *Icon::getPixelData()
{
	return _pixelData;
}

const uint8_t pixelDataSlopeRaising[] = {
		0,0,0,0,0,1,0,0,
		0,0,0,0,1,1,1,0,
		0,0,0,1,0,1,0,1,
		0,0,0,0,0,1,0,0,
		0,0,0,0,0,1,0,0,
		0,0,0,0,0,1,0,0,
		1,1,1,1,1,1,0,0,
		0,0,0,0,0,0,0,0
};
Icon SlopeRaising(&pixelDataSlopeRaising[0]);

const uint8_t pixelDataSlopeFalling[] = {
		0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,0,0,
		0,0,0,0,0,1,0,0,
		0,0,0,0,0,1,0,0,
		0,0,0,0,0,1,0,0,
		0,0,0,1,0,1,0,1,
		0,0,0,0,1,1,1,0,
		0,0,0,0,0,1,0,0
		};
Icon SlopeFalling(&pixelDataSlopeFalling[0]);

const uint8_t pixelDataSlopeBoth[] = {
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		1,0,0,1,1,0,0,1,
		0,1,1,0,0,1,1,0,
		0,1,1,0,0,1,1,0,
		1,0,0,1,1,0,0,1,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0
};
Icon SlopeBoth(&pixelDataSlopeBoth[0]);

/*
const uint8_t[8][8] Dummy = {
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0}
};*/