//
// Created by Phillip Schuster on 30.01.16.
//

#ifndef TEENSYCMAKE_LABELBUTTON_H
#define TEENSYCMAKE_LABELBUTTON_H

#include "LabelView.h"
#include "Button.h"
#include "../layers/BitmapLayer.h"

class LabelButton: public LabelView, public Button
{
#pragma mark Constructor
public:
	virtual void setFrame(Rect frame) override;

	LabelButton(String text, Rect frame);
    LabelButton(String text, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
    ~LabelButton() {};

#pragma mark Getter/Setter
    virtual void setAlternateBackgroundColor(uint16_t color) { _alternateBackgroundColor = color; };
    virtual uint16_t getAlternateBackgroundColor() { return _alternateBackgroundColor; };;;;

    void setAlternateFont(const ILI9341_t3_font_t* font) { _alternateFont = font; };
    const ILI9341_t3_font_t* getAlternateFont() { return _alternateFont; };

    void setAlternateTextColor(uint16_t color) { _alternateTextColor = color; };
    uint16_t getAlternateTextColor() { return _alternateTextColor; };

    virtual void setIcon(const uint8_t* bitmap, uint16_t color, uint16_t width, uint16_t height);

#pragma mark Member Functions
private:
    void updateButton(ButtonState buttonState);

#pragma mark View
public:
    virtual bool touchDown(TS_Point &point) override;
    virtual bool touchUp(TS_Point &point) override;
    virtual void touchCancelled() override;
    virtual void setFont(const ILI9341_t3_font_t *font) override;
    virtual void display() override;

#pragma mark Member Variables
protected:
    uint16_t _alternateBackgroundColor;
    const ILI9341_t3_font_t* _alternateFont;
    uint16_t _alternateTextColor;
    const uint8_t* _iconBitmap;
    BitmapLayer* _iconLayer;
    uint8_t _iconWidth;
    uint8_t _iconHeight;
    uint16_t _iconColor;
    RectangleLayer* _gapLayer;
};


#endif //TEENSYCMAKE_LABELBUTTON_H
