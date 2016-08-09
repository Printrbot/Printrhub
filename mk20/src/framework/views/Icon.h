//
// Created by Phillip Schuster on 09.07.16.
//

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
