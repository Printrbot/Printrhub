//
// Created by Phillip Schuster on 26.01.16.
//

#ifndef TEENSYCMAKE_TEXTLAYER_H
#define TEENSYCMAKE_TEXTLAYER_H

#include "Layer.h"
#include "ILI9341_t3.h"

class TextLayer: public Layer
{
public:
    TextLayer() {};
    TextLayer(Rect frame);
    virtual ~TextLayer() {};

    virtual void draw(Rect& dirtyRect, Rect& invalidationRect) override;

    const ILI9341_t3_font_t * getFont()
    {
        return _font;
    }

    void setFont(const ILI9341_t3_font_t *_font)
    {
        TextLayer::_font = _font;
    }

    uint16_t getForegroundColor() const
    {
        return _foregroundColor;
    }

    void setForegroundColor(uint16_t _foregroundColor)
    {
        TextLayer::_foregroundColor = _foregroundColor;
    }

    uint16_t getBackgroundColor() const
    {
        return _backgroundColor;
    }

    void setBackgroundColor(uint16_t _backgroundColor)
    {
        TextLayer::_backgroundColor = _backgroundColor;
    }


    const String* getText() const
    {
        return _text;
    }

    void setText(String *text)
    {
        _text = text;
    }

private:
    const ILI9341_t3_font_t* _font;
    uint16_t _foregroundColor;
    uint16_t _backgroundColor;
    String* _text;
};


#endif //TEENSYCMAKE_TEXTLAYER_H
