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

#ifndef _TEENSYCMAKE_COLORTHEME_H_
#define _TEENSYCMAKE_COLORTHEME_H_

#include <Arduino.h>

#define RGB565(r, g, b) ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

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

class ColorTheme {
 public:
  ColorTheme();

  uint16_t getColor(SystemColor color);

 private:
  uint16_t _colors[18];
};

#endif //_TEENSYCMAKE_COLORTHEME_H_
