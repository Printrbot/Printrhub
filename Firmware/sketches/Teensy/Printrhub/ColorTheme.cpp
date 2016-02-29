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

#include <stdint-gcc.h>
#include <stdint.h>
#include "ColorTheme.h"
#include "Application.h"

ColorTheme::ColorTheme():
		_colors({{RGB565(25,194,255),RGB565(255,182,131),RGB565(255,161, 94),RGB565(255,106,0),RGB565(255,106,0)},
				{RGB565(131,149,149),RGB565(146,143,132),RGB565(167,159,131),RGB565(237,214,132),RGB565(254,218, 86)},
				{RGB565(79,79,79),RGB565( 80, 65,186),RGB565(132,123,192),RGB565(114,112,133),RGB565( 83, 81,102)},
				{RGB565(59,138,127),RGB565(135,164,160),RGB565(107,158,151),RGB565( 28,131,117),RGB565(  6,115,100)}
		}),
		_textColors({RGB565(255,255,255),RGB565(255,255,255),RGB565(240,240,240),RGB565(40,40,40),RGB565(0,0,0)}),
		_backgroundColor(RGB565(231,236,238))
{
}

uint16_t ColorTheme::getPrimaryColor(ColorTheme::Shade shade)
{
	return _colors[0][shade];
}

uint16_t ColorTheme::getSecondaryColor1(ColorTheme::Shade shade)
{
	return _colors[1][shade];
}

uint16_t ColorTheme::getSecondaryColor2(ColorTheme::Shade shade)
{
	return _colors[2][shade];
}

uint16_t ColorTheme::getComplementColor(ColorTheme::Shade shade)
{
	return _colors[3][shade];
}

uint16_t ColorTheme::getBackgroundColor(ColorTheme::Shade shade)
{
	return _backgroundColor;
}

uint16_t ColorTheme::getTextColor(ColorTheme::Shade shade)
{
	return _textColors[shade];
}
