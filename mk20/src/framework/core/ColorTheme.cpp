/*
 * Color theme for the printer. Holds colors for constants to color buttons, text
 * backgrounds and various other user interface elements
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Phillip Schuster
 * https://github.com/Printrbot/Printrhub
 *
 * Developed in cooperation by Phillip Schuster (@appfruits) from appfruits.com
 * http://www.appfruits.com/printrhub
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdint-gcc.h>
#include <stdint.h>
#include "ColorTheme.h"
#include "Application.h"

ColorTheme::ColorTheme() :
	_colors({RGB565(100, 100, 100),    //SpacerColor
			 RGB565(241, 241, 241),    //BackgroundColor
			 RGB565(181, 181, 181),    //HighlightBackgroundColor
			 RGB565(255, 255, 255),    //HighlightTextColor
			 RGB565(233, 154, 36),     //HighlightBackgroundColor
			 RGB565(190, 190, 190),    //HighlightTextColor
			 RGB565(241, 241, 241),    //ControlBackgroundColor
			 RGB565(220, 220, 220),    //ControlAlternateBackgroundColor
			 RGB565(40, 40, 40),       //ControlTextColor
			 RGB565(70, 70, 70),       //ControlAlternateTextColor
			 RGB565(71, 71, 71),       //TextColor
			 RGB565(255, 255, 255),    //OverlayTextColor
			 RGB565(2, 224, 53),       //SuccessColor
			 RGB565(255, 105, 1),      //WarningColor
			 RGB565(255, 4, 0),        //AlertColor
			 RGB565(200, 200, 200),    //SidebarColor
			 RGB565(96, 96, 96)    	   //SidebarTextColor
			}) {
}

uint16_t ColorTheme::getColor(SystemColor color) {
  return _colors[color];
}
