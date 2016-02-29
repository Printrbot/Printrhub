//
// Created by Phillip Schuster on 30.01.16.
//

#include "LabelButton.h"

LabelButton::LabelButton(String text, Rect frame):
LabelView(text,frame)
{
    _name = "LabelButton";
    _delegate = NULL;
    _state = ButtonState::Off;
    _type = ButtonType::Push;
    _alternateFont = getFont();
    _alternateTextColor = ILI9341_WHITE;
}

LabelButton::LabelButton(String text, uint16_t x, uint16_t y, uint16_t width, uint16_t height):
LabelView(text,x,y,width,height)
{
    _name = "LabelButton";
    _delegate = NULL;
    _state = ButtonState::Off;
    _type = ButtonType::Push;
    _alternateFont = getFont();
    _alternateTextColor = ILI9341_WHITE;
}

void LabelButton::updateButton(ButtonState buttonState)
{
    if (buttonState == ButtonState::Off)
    {
        getTextLayer()->setBackgroundColor(getBackgroundColor());
        setFont(getFont());
        getTextLayer()->setForegroundColor(getTextColor());
    }
    else if (buttonState == ButtonState::On)
    {
        getTextLayer()->setBackgroundColor(getAlternateBackgroundColor());
        getTextLayer()->setForegroundColor(getAlternateTextColor());
        if (_type != ButtonType::Toggle)
        {
            setFont(getAlternateFont());
        }
        else
        {
            //Don't scale text if Button is a toogle (looks strange)
            setFont(getFont());
        }
    }

    setNeedsDisplay();
}

bool LabelButton::touchDown(TS_Point &point)
{
    updateButton(ButtonState::On);

    return true;
}

bool LabelButton::touchUp(TS_Point &point)
{
    if (_type == ButtonType::Toggle)
    {
        _state = (ButtonState) !_state;
        updateButton(_state);
    }
    else
    {
        updateButton(ButtonState::Off);
    }

    if (_delegate != NULL)
    {
        _delegate->buttonPressed(this);
    }

    return true;
}

void LabelButton::touchCancelled()
{
    updateButton(ButtonState::Off);
}

void LabelButton::setFont(const ILI9341_t3_font_t *font)
{
    LabelView::setFont(font);

    if (_alternateFont == NULL)
    {
        _alternateFont = font;
    }
}

void LabelButton::setButtonState(ButtonState state)
{
    if (_state == state) return;
    _state = state;
}


void LabelButton::display()
{
    LabelView::display();
    updateButton(_state);
}
