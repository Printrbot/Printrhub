//
// Created by Phillip Schuster on 06.05.16.
//

#ifndef TEENSY_BUTTON_H
#define TEENSY_BUTTON_H

#include "LabelView.h"

enum ButtonType {
    Push = 0,
    Toggle = 1
};

enum ButtonState {
    Off = 0,
    On = 1
};

class ButtonDelegate
{
public:
    virtual void buttonPressed(void* button) = 0;
};

class Button
{

public:
    void setButtonType(ButtonType type) { _type = type; }
    ButtonType getButtonType() { return _type; }

    ButtonState getButtonState() { return _state; }
    void setButtonState(ButtonState state);

    virtual void setDelegate(ButtonDelegate* delegate) { _delegate = delegate; };

protected:
    ButtonType _type;
    ButtonState _state;
    ButtonDelegate* _delegate;
};


#endif //TEENSY_BUTTON_H
