//
// Created by Phillip Schuster on 20.01.16.
//

#include "BitmapButton.h"
#include "BitmapLayer.h"
#include "Application.h"
#include "Animator.h"
#include "Animation.h"

BitmapButton::BitmapButton(Rect frame):
View(frame),
_originalFrame(frame),
_touchAnimation(NULL),
_baseLayer(NULL),
_bitmapLayer(NULL)
{
    _delegate = NULL;
    _backgroundColor = Application.getTheme()->getColor(ControlBackgroundColor);
    _color = Application.getTheme()->getColor(ControlTextColor);
    _alternateBackgroundColor = Application.getTheme()->getColor(ControlAlternateBackgroundColor);
    _alternateTextColor = Application.getTheme()->getColor(ControlAlternateTextColor);

    LOG("Creating Bitmap Button");
    LOG_VALUE("Frame",frame.toString());
    LOG_VALUE("This-Frame",_frame.toString());
}

void BitmapButton::setBitmap(const uint8_t *bitmap, uint16_t width, uint16_t height)
{
    Rect bitmapFrame = _frame;

    //The bitmap is smaller than the frame, create a layer and place the bitmap inside it
    if (_frame.width > width || _frame.height > height)
    {
        _baseLayer = new RectangleLayer(_frame);
        _baseLayer->setBackgroundColor(getBackgroundColor());

        bitmapFrame.x += (_frame.width - width)/2;
        bitmapFrame.y += (_frame.height - height)/2;
        bitmapFrame.width = width;
        bitmapFrame.height = height;

        //Stamp a hole in the base layer where we place the BitmapLayer
        _baseLayer->splitWithRect(bitmapFrame);
        addLayer(_baseLayer);
    }

    _bitmapLayer = new BitmapLayer(bitmapFrame);
    _bitmapLayer->setBitmap(bitmap,width,height);
    _bitmapLayer->setBackgroundColor(getBackgroundColor());
    _bitmapLayer->setColor(getColor());

    addLayer(_bitmapLayer);
}

BitmapButton::~BitmapButton()
{

}



void BitmapButton::setFrame(Rect frame)
{
    if (_frame == frame) return;

    View::setFrame(frame);
    _originalFrame = frame;

    _layers.at(0)->setFrame(frame);
}

void BitmapButton::updateButton(ButtonState buttonState)
{
    if (buttonState == ButtonState::Off)
    {
        if (getBaseLayer() != NULL)
        {
            getBaseLayer()->setBackgroundColor(getBackgroundColor());
        }

        getBitmapLayer()->setBackgroundColor(getBackgroundColor());
        getBitmapLayer()->setColor(getColor());
    }
    else if (buttonState == ButtonState::On)
    {
        if (getBaseLayer() != NULL)
        {
            getBaseLayer()->setBackgroundColor(getAlternateBackgroundColor());
        }

        getBitmapLayer()->setBackgroundColor(getAlternateBackgroundColor());
        getBitmapLayer()->setColor(getAlternateTextColor());
    }

    setNeedsDisplay();
}

bool BitmapButton::touchDown(TS_Point &point)
{
    updateButton(ButtonState::On);

    return true;
}

bool BitmapButton::touchUp(TS_Point &point)
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

void BitmapButton::touchCancelled()
{
    updateButton(ButtonState::Off);
}
