//
// Created by Phillip Schuster on 09.07.16.
//

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
	OverlayTextColor,
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
	uint16_t _colors[18];
};


#endif //_TEENSYCMAKE_COLORTHEME_H_
