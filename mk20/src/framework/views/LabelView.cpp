//
// Created by Phillip Schuster on 09.07.16.
//

#include "LabelView.h"
#include "../core/Application.h"
#include "ILI9341_t3.h"

LabelView::LabelView(String text, uint16_t x, uint16_t y, uint16_t width, uint16_t height):
		View(x,y,width,height)
{
	_textAlign = TEXTALIGN_CENTERED;
	_verticalTextAlign = TEXTALIGN_CENTERED;
	_text = text;
	_font = &PTSansNarrow_20;
	_textColor = Application.getTheme()->getColor(TextColor);
	_backgroundColor = ILI9341_WHITE;
	_name = "LabelView";
	_layer = NULL;
}

LabelView::LabelView(String text, Rect frame):
	View(frame)
{
	_textAlign = TEXTALIGN_CENTERED;
	_verticalTextAlign = TEXTALIGN_CENTERED;
	_text = text;
	_font = &PTSansNarrow_20;
	_textColor = Application.getTheme()->getColor(TextColor);
	_backgroundColor = ILI9341_WHITE;
	_name = "LabelView";
	_layer = NULL;
}

void LabelView::display()
{
	_layer = new TextLayer(_frame);
	_layer->setForegroundColor(_textColor);
	_layer->setBackgroundColor(_backgroundColor);
	_layer->setText(_text);
	_layer->setFont(_font);
	_layer->setTextAlign(_textAlign);
	_layer->setVerticalTextAlign(_verticalTextAlign);
	addLayer(_layer);

	View::display();
}

void LabelView::setText(String text)
{
	_text = text;
	setNeedsDisplay();
}

void LabelView::setTextAlign(uint8_t textAlign)
{
	if (_textAlign == textAlign) return;
	_textAlign = textAlign;

	if (_layer != NULL)
	{
		_layer->setTextAlign(textAlign);
	}

	setNeedsDisplay();
}

void LabelView::setTextColor(uint16_t color)
{
	if (_textColor == color) return;
	_textColor = color;

	if (_layer != NULL)
	{
		_layer->setForegroundColor(color);
	}

	setNeedsDisplay();
}

void LabelView::setVerticalTextAlign(uint8_t verticalTextAlign)
{
	if (_verticalTextAlign == verticalTextAlign) return;
	_verticalTextAlign = verticalTextAlign;

	if (_layer != NULL)
	{
		_layer->setVerticalTextAlign(verticalTextAlign);
	}

	setNeedsDisplay();
}

void LabelView::setFont(const ILI9341_t3_font_t* font)
{
	if (_font == font) return;
	_font = font;

	if (_layer != NULL)
	{
		_layer->setFont(font);
	}

	setNeedsDisplay();
}

const ILI9341_t3_font_t *LabelView::getFont()
{
	return _font;
}
