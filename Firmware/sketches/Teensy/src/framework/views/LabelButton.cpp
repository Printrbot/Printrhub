//
// Created by Phillip Schuster on 30.01.16.
//

#include "LabelButton.h"
#include "../../framework/layers/BitmapLayer.h"

LabelButton::LabelButton(String text, Rect frame):
LabelView(text,frame)
{
    _name = "LabelButton";
    _delegate = NULL;
    _state = ButtonState::Off;
    _type = ButtonType::Push;
    _alternateFont = getFont();
    _borderWidth = 0;
    _iconBitmap = NULL;
    _gapLayer = NULL;
    _iconLayer = NULL;

    _backgroundColor = Application.getTheme()->getColor(ControlBackgroundColor);
    _alternateBackgroundColor = Application.getTheme()->getColor(ControlAlternateBackgroundColor);
    _alternateTextColor = Application.getTheme()->getColor(ControlAlternateTextColor);
    setTextColor(Application.getTheme()->getColor(ControlTextColor));
}

LabelButton::LabelButton(String text, uint16_t x, uint16_t y, uint16_t width, uint16_t height):
LabelView(text,x,y,width,height)
{
    _name = "LabelButton";
    _delegate = NULL;
    _state = ButtonState::Off;
    _type = ButtonType::Push;
    _alternateFont = getFont();
    _borderWidth = 0;
    _iconBitmap = NULL;
    _gapLayer = NULL;
    _iconLayer = NULL;

    _backgroundColor = Application.getTheme()->getColor(ControlBackgroundColor);
    _alternateBackgroundColor = Application.getTheme()->getColor(ControlAlternateBackgroundColor);
    _alternateTextColor = Application.getTheme()->getColor(ControlAlternateTextColor);
    setTextColor(Application.getTheme()->getColor(ControlTextColor));
}

void LabelButton::updateButton(ButtonState buttonState)
{
    if (buttonState == ButtonState::Off)
    {
        getTextLayer()->setBackgroundColor(getBackgroundColor());
        setFont(getFont());
        getTextLayer()->setForegroundColor(getTextColor());

        if (_gapLayer)
        {
            _gapLayer->setBackgroundColor(getBackgroundColor());
            _iconLayer->setBackgroundColor(getBackgroundColor());
        }
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

        if (_gapLayer)
        {
            _gapLayer->setBackgroundColor(getAlternateBackgroundColor());
            _iconLayer->setBackgroundColor(getAlternateBackgroundColor());
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


void LabelButton::display()
{
    if (_iconBitmap == NULL)
    {
        _layer = new TextLayer(_frame);
        _layer->setForegroundColor(getTextColor());
        _layer->setBackgroundColor(getBackgroundColor());
        _layer->setText(getText());
        _layer->setFont(getFont());
        _layer->setTextAlign(getTextAlign());
        _layer->setVerticalTextAlign(getVerticalTextAlign());
        addLayer(_layer);
    }
    else
    {
        uint32_t textWidth = Display.textWidth(getFont(),getText());

        uint32_t innerWidth = textWidth + _iconWidth + 10;  //10 Gap between icon and text
        uint32_t paddingWidth = (_frame.width - innerWidth)/2;
        uint32_t paddingHeight = (_frame.height - Display.fontHeight(getFont()))/2;

        _gapLayer = new RectangleLayer(Rect(_frame.x,_frame.y,paddingWidth + _iconWidth + 10,_frame.height));
        _gapLayer->setBackgroundColor(getBackgroundColor());

        _iconLayer = new BitmapLayer(Rect(_frame.x + paddingWidth,_frame.y + paddingHeight, _iconWidth,_iconHeight));
        _gapLayer->splitWithRect(_iconLayer->getFrame());
        _iconLayer->setBitmap(_iconBitmap,_iconWidth,_iconHeight);
        _iconLayer->setBackgroundColor(getBackgroundColor());
        _iconLayer->setColor(_iconColor);

        _layer = new TextLayer(Rect(_frame.x+paddingWidth+_iconWidth+10,_frame.y,textWidth + paddingWidth,_frame.height));
        _layer->setForegroundColor(getTextColor());
        _layer->setBackgroundColor(getBackgroundColor());
        _layer->setText(getText());
        _layer->setFont(getFont());
        _layer->setTextAlign(TEXTALIGN_LEFT);
        _layer->setVerticalTextAlign(getVerticalTextAlign());

        addLayer(_layer);
        addLayer(_gapLayer);
        addLayer(_iconLayer);
    }

    View::display();

    updateButton(_state);
}


void LabelButton::setIcon(const uint8_t *bitmap, uint16_t color, uint16_t width, uint16_t height)
{
    _iconBitmap = bitmap;
    _iconWidth = width;
    _iconHeight = height;
    _iconColor = color;
}

