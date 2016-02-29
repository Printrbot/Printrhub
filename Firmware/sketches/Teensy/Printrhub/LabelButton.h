//
// Created by Phillip Schuster on 30.01.16.
//

#ifndef TEENSYCMAKE_LABELBUTTON_H
#define TEENSYCMAKE_LABELBUTTON_H

#include "LabelView.h"
#include "BitmapButton.h"

enum ButtonType {
    Push = 0,
    Toggle = 1
};

enum ButtonState {
    Off = 0,
    On = 1
};

class LabelButton: public LabelView
{
public:
    LabelButton(String text, Rect frame);
    LabelButton(String text, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
    ~LabelButton() {};

    virtual void setDelegate(ButtonDelegate* delegate) { _delegate = delegate; };

    virtual void setAlternateBackgroundColor(uint16_t color) { _alternateBackgroundColor = color; };
    virtual uint16_t getAlternateBackgroundColor() { return _alternateBackgroundColor; };

    void setButtonType(ButtonType type) { _type = type; };
    ButtonType getButtonType() { return _type; };

    void setButtonState(ButtonState state);
    ButtonState getButtonState() { return _state; };

    void setAlternateFont(const ILI9341_t3_font_t* font) { _alternateFont = font; };
    const ILI9341_t3_font_t* getAlternateFont() { return _alternateFont; };

    void setAlternateTextColor(uint16_t color) { _alternateTextColor = color; };
    uint16_t getAlternateTextColor() { return _alternateTextColor; };

public:
    virtual bool touchDown(TS_Point &point) override;
    virtual bool touchUp(TS_Point &point) override;
    virtual void touchCancelled() override;

protected:
    ButtonDelegate* _delegate;
    uint16_t _alternateBackgroundColor;
    const ILI9341_t3_font_t* _alternateFont;
    uint16_t _alternateTextColor;
    ButtonType _type;
    ButtonState _state;

    void updateButton(ButtonState buttonState);

public:
    virtual void setFont(const ILI9341_t3_font_t *font) override;

    virtual void display() override;
};


#endif //TEENSYCMAKE_LABELBUTTON_H
