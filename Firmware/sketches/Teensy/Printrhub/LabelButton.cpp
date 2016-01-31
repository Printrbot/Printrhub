//
// Created by Phillip Schuster on 30.01.16.
//

#include "LabelButton.h"

LabelButton::LabelButton(String text, Rect frame):
LabelView(text,frame)
{
    _name = "LabelButton";
    _delegate = NULL;
}

LabelButton::LabelButton(String text, uint16_t x, uint16_t y, uint16_t width, uint16_t height):
LabelView(text,x,y,width,height)
{
    _name = "LabelButton";
    _delegate = NULL;
}

bool LabelButton::touchDown(TS_Point &point)
{
    LOG("LABEL BUTTON TOUCH DOWN");
    getTextLayer()->setBackgroundColor(getAlternateBackgroundColor());
    setTextColor(ILI9341_WHITE);
    setFont(&PTSansNarrow_28);
    getBackgroundLayer()->setBackgroundColor(getAlternateBackgroundColor());
    setNeedsDisplay();

    return true;
}

bool LabelButton::touchUp(TS_Point &point)
{
    getTextLayer()->setBackgroundColor(getBackgroundColor());
    setFont(&PTSansNarrow_20);
    setTextColor(Application.getTheme()->getSecondaryColor2());
    getBackgroundLayer()->setBackgroundColor(getBackgroundColor());
    setNeedsDisplay();

    if (_delegate != NULL)
    {
        _delegate->buttonPressed(this);
    }

    return true;
}

void LabelButton::touchCancelled()
{
    getTextLayer()->setBackgroundColor(getBackgroundColor());
    setFont(&PTSansNarrow_20);
    setTextColor(Application.getTheme()->getSecondaryColor2());
    getBackgroundLayer()->setBackgroundColor(getBackgroundColor());
    setNeedsDisplay();
}
