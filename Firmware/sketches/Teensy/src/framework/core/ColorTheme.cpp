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
		_colors({RGB565(100,100,100),	//SpacerColor
				 RGB565(231,236,238),	//BackgroundColor
				 RGB565(5,177,220),		//HighlightBackgroundColor
				 RGB565(255,255,255),	//HighlightTextColor
				 RGB565(3,100,120),		//HighlightBackgroundColor
				 RGB565(190,190,190),	//HighlightTextColor
				 RGB565(255,255,255),   //ControlBackgroundColor
				 RGB565(220,220,220),	//ControlAlternateBackgroundColor
				 RGB565(40,40,40),		//ControlTextColor
				 RGB565(70,70,70),		//ControlAlternateTextColor
				 RGB565(79,79,79),		//TextColor
				 RGB565(255,255,255),	//OverlayTextColor
				 RGB565(2,224,53),		//SuccessColor
				 RGB565(255,105,1),		//WarningColor
				 RGB565(255,4,0),		//AlertColor
				 RGB565(2,205,255),		//SidebarColor
				 RGB565(255,255,255)	//SidebarTextColor
		})
{
}


uint16_t ColorTheme::getColor(SystemColor color)
{
	return _colors[color];
}

