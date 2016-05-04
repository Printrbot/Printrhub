//
// Created by Phillip Schuster on 04.05.16.
//

#ifndef TEENSY_SIDEBAR_H
#define TEENSY_SIDEBAR_H

#include "View.h"
#include "VerticalTextLayer.h"
#include "RectangleLayer.h"

class Sidebar : public View
{
public:
	Sidebar(const String &title, const String &icon, Rect frame);
	Sidebar(const String &title, const String &icon, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

private:
	String _title;
	String _icon;
	VerticalTextLayer* _textLayer;
	RectangleLayer* _bottomButtonLayer;
};


#endif //TEENSY_SIDEBAR_H
