//
// Created by Phillip Schuster on 04.05.16.
//

#include "Sidebar.h"
#include "Application.h"

Sidebar::Sidebar(const String &title, const String &icon, Rect frame):
View(frame)
{
	_title = title;
	_icon = icon;

	Rect textLayerRect(frame.x,frame.y,frame.width,frame.height-50);

	_textLayer = new VerticalTextLayer(textLayerRect);
	_textLayer->setText(&_title);
	_textLayer->setForegroundColor(Application.getTheme()->getTextColor(ColorTheme::Shade::Light));
	_textLayer->setBackgroundColor(Application.getTheme()->getPrimaryColor(ColorTheme::Shade::Default));
	_textLayer->setVerticalTextAlign(TEXTALIGN_CENTERED);
	_textLayer->setTextAlign(TEXTALIGN_RIGHT);
	_textLayer->setFont(&PTSansNarrow_24);
	_textLayer->setPadding(3);
	addLayer(_textLayer);

	Rect bottomButtonRect(frame.x,frame.bottom()-50,frame.width,50);
	_bottomButtonLayer = new RectangleLayer(bottomButtonRect);
	_bottomButtonLayer->setBackgroundColor(Application.getTheme()->getPrimaryColor(ColorTheme::Shade::Darker));
	addLayer(_bottomButtonLayer);
}

Sidebar::Sidebar(const String &title, const String &icon, uint16_t x, uint16_t y, uint16_t width, uint16_t height):
View(x,y,width,height)
{
	_title = title;
	_icon = icon;
}

