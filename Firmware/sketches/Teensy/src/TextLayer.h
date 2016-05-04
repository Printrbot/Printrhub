//
// Created by Phillip Schuster on 26.01.16.
//

#ifndef TEENSYCMAKE_TEXTLAYER_H
#define TEENSYCMAKE_TEXTLAYER_H

#include "Layer.h"
#include "ILI9341_t3.h"

#define TEXTALIGN_LEFT 0
#define TEXTALIGN_CENTERED 1
#define TEXTALIGN_RIGHT 2
#define TEXTALIGN_TOP 0
#define TEXTALIGN_BOTTOM 2

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

    void setTextAlign(uint8_t textAlign);

    uint8_t getTextAlign() const
    {
        return _textAlign;
    }

    uint8_t getVerticalTextAlign() const
    {
        return _verticalTextAlign;
    }

    void setVerticalTextAlign(uint8_t verticalTextAlign);


    uint8_t getPadding() const
    {
        return _padding;
    }

    void setPadding(uint8_t _padding)
    {
        TextLayer::_padding = _padding;
    }

private:
    const ILI9341_t3_font_t* _font;
    uint16_t _foregroundColor;
    uint16_t _backgroundColor;
    String* _text;

    uint8_t _textAlign;
    uint8_t _verticalTextAlign;
    uint8_t _padding;
};


#endif //TEENSYCMAKE_TEXTLAYER_H
