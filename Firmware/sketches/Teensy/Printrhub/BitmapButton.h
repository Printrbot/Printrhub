//
// Created by Phillip Schuster on 20.01.16.
//

#ifndef TEENSYCMAKE_BITMAPBUTTON_H
#define TEENSYCMAKE_BITMAPBUTTON_H

#include "View.h"
#include "BitmapLayer.h"

class BitmapButton;

class ButtonDelegate
{
public:
    virtual void buttonPressed(void* button) = 0;
};

class BitmapButton: public View
{
public:
    BitmapButton(Rect frame);
    virtual ~BitmapButton();

    virtual void setBitmap(const uint16_t* bitmap, uint16_t width, uint16_t height);

    virtual void setFrame(Rect frame) override;

    virtual void setDelegate(ButtonDelegate* delegate) { _delegate = delegate; };

protected:
    ButtonDelegate* _delegate;
    Rect _originalFrame;
    Animation* _touchAnimation;

public:
    virtual bool touchUp(TS_Point &point) override;

protected:
    virtual void animationUpdated(Animation *animation, float currentValue, float timeLeft) override;

public:
    virtual void animationFinished(Animation *animation) override;

    virtual bool touchDown(TS_Point &point) override;

    virtual void touchCancelled() override;
};


#endif //TEENSYCMAKE_BITMAPBUTTON_H
