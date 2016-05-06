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

#ifndef _TEENSYCMAKE_COLORTHEME_H_
#define _TEENSYCMAKE_COLORTHEME_H_

#include <Arduino.h>

#define RGB565(r,g,b) ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

typedef enum SystemColor {
	SpacerColor = 0,
	BackgroundColor,
	HighlightBackgroundColor,
	HighlighTextColor,
	HighlightAlternateBackgroundColor,
	HighlightAlternateTextColor,
	ControlBackgroundColor,
	ControlAlternateBackgroundColor,
	ControlTextColor,
	ControlAlternateTextColor,
	TextColor,
	SuccessColor,
	WarningColor,
	AlertColor,
	SidebarBackgroundColor,
	SidebarTextColor,
};

class ColorTheme
{
public:
	ColorTheme();

	uint16_t getColor(SystemColor color);

private:
	uint16_t _colors[17];
};


#endif //_TEENSYCMAKE_COLORTHEME_H_
