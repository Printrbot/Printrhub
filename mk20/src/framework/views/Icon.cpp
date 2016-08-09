//
// Created by Phillip Schuster on 09.07.16.
//

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
