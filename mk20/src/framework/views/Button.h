/*
 * Base class for all buttons implementing a standard interface for button handling
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/printrbot-simple-2016-display-system-explained
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Phillip Schuster
 * https://github.com/Printrbot/Printrhub
 *
 * Developed in cooperation by Phillip Schuster (@appfruits) from appfruits.com
 * http://www.appfruits.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

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

class ButtonDelegate {
 public:
  virtual void buttonPressed(void *button) = 0;
};

class Button {

 public:
  void setButtonType(ButtonType type) { _type = type; }
  ButtonType getButtonType() { return _type; }

  ButtonState getButtonState() { return _state; }
  void setButtonState(ButtonState state);

  virtual void setDelegate(ButtonDelegate *delegate) { _delegate = delegate; };

 protected:
  ButtonType _type;
  ButtonState _state;
  ButtonDelegate *_delegate;
};

#endif //TEENSY_BUTTON_H
