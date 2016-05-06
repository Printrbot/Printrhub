//
// Created by Phillip Schuster on 06.05.16.
//

#include "LabelButton.h"
#include "Button.h"

void Button::setButtonState(ButtonState state)
{
    if (_state == state) return;
    _state = state;
}