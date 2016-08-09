//
// Created by Phillip Schuster on 09.07.16.
//

#include <stdint-gcc.h>
#include <stdint.h>
#include "ColorTheme.h"
#include "Application.h"

ColorTheme::ColorTheme():
		_colors({RGB565(100,100,100),	//SpacerColor
				 RGB565(231,236,238),	//BackgroundColor
				 RGB565(85,85,85),		//HighlightBackgroundColor
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
				 RGB565(113,113,113),		//SidebarColor
				 RGB565(255,255,255)	//SidebarTextColor
		})
{
}


uint16_t ColorTheme::getColor(SystemColor color)
{
	return _colors[color];
}
