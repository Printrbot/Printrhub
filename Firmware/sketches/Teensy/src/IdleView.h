//
// Created by Phillip Schuster on 16.01.16.
//

#ifndef TEENSYCMAKE_IDLEVIEW_H
#define TEENSYCMAKE_IDLEVIEW_H

#include "View.h"
#include "RectangleLayer.h"
#include "BitmapLayer.h"

class IdleView: public View
{
public:
    IdleView(const Rect& frame);

    RectangleLayer* getLayer();
    RectangleLayer* getLayer2();

protected:
    RectangleLayer* _layer;
    RectangleLayer* _layer2;
    BitmapLayer* _buttonLayer;
};


#endif //TEENSYCMAKE_IDLEVIEW_H
