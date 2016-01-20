//
// Created by Phillip Schuster on 20.01.16.
//

#ifndef TEENSYCMAKE_BITMAPBUTTON_H
#define TEENSYCMAKE_BITMAPBUTTON_H

#include "View.h"
#include "BitmapLayer.h"

class BitmapButton: public View
{
public:
    BitmapButton(Rect frame);
    virtual ~BitmapButton();

    virtual void setBitmap(const uint16_t* bitmap, uint16_t width, uint16_t height);
};


#endif //TEENSYCMAKE_BITMAPBUTTON_H
