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
_delegate(NULL),
_originalFrame(frame),
_touchAnimation(NULL)

{
    LOG("Creating Bitmap Button");
    LOG_VALUE("Frame",frame.toString());
    LOG_VALUE("This-Frame",_frame.toString());
}

void BitmapButton::setBitmap(const uint16_t *bitmap, uint16_t width, uint16_t height)
{
    BitmapLayer* layer = new BitmapLayer(_frame);
    layer->setBitmap(bitmap,width,height);

    addLayer(layer);
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


bool BitmapButton::touchDown(TS_Point &point)
{
    if (_touchAnimation != NULL && _touchAnimation->isActive())
    {
        _touchAnimation->stop();
        _touchAnimation = NULL;
    }

    Animation* animation = Animator.getAnimationSlot();
    if (animation)
    {
        animation->init("pushIn",_frame.y,_frame.y+5,0.1,0);
        addAnimation(animation);
        _touchAnimation = animation;
    }

    return true;
}

bool BitmapButton::touchUp(TS_Point &point)
{
    if (_touchAnimation != NULL && _touchAnimation->isActive())
    {
        _touchAnimation->stop();
        _touchAnimation = NULL;
    }

    Animation* animation = Animator.getAnimationSlot();
    if (animation)
    {
        animation->init("pushOut",_frame.y,_originalFrame.y,0.2,0);
        addAnimation(animation);
        _touchAnimation = animation;

        //Trigger is fired when pushOut animation finishes, see ::animationFinished implementation
        //in this class
    }
    else
    {
        //No animation slot available, trigger delegate immediately
        if (_delegate != NULL)
        {
            _delegate->buttonPressed(this);
        }
    }

    return true;
}


void BitmapButton::touchCancelled()
{
    if (_touchAnimation != NULL && _touchAnimation->isActive())
    {
        _touchAnimation->stop();
        _touchAnimation = NULL;
    }

    Animation* animation = Animator.getAnimationSlot();
    if (animation)
    {
        animation->init("pushOutCancel",_frame.y,_originalFrame.y,0.2,0);
        addAnimation(animation);
        _touchAnimation = animation;
    }
}

void BitmapButton::animationUpdated(Animation *animation, float currentValue, float timeLeft)
{
    LOG_VALUE("Handling Animation",animation->getKey());
    if (animation->getKey() == "pushIn" || animation->getKey() == "pushOut" || animation->getKey() == "pushOutCancel")
    {
        _frame.y = currentValue;
        _layers.at(0)->setFrame(_frame);
        Display.setNeedsLayout();
    }
}


void BitmapButton::animationFinished(Animation *animation)
{
    if (animation->getKey() == "pushOut")
    {
        //Trigger delegates action message
        if (_delegate != NULL)
        {
            _delegate->buttonPressed(this);
        }
    }
}
