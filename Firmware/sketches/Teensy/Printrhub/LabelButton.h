//
// Created by Phillip Schuster on 30.01.16.
//

#ifndef TEENSYCMAKE_LABELBUTTON_H
#define TEENSYCMAKE_LABELBUTTON_H

#include "LabelView.h"
#include "BitmapButton.h"

class LabelButton: public LabelView
{
public:
    LabelButton(String text, Rect frame);
    LabelButton(String text, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
    ~LabelButton() {};

    virtual void setDelegate(ButtonDelegate* delegate) { _delegate = delegate; };

protected:
    ButtonDelegate* _delegate;
};


#endif //TEENSYCMAKE_LABELBUTTON_H
