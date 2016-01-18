//
// Created by Phillip Schuster on 16.01.16.
//

#ifndef TEENSYCMAKE_IDLEVIEW_H
#define TEENSYCMAKE_IDLEVIEW_H

#include "View.h"
#include "RectangleLayer.h"

class IdleView: public View
{
public:
    IdleView(const Rect& frame);

    RectangleLayer* getLayer();
    RectangleLayer* getLayer2();

protected:
    RectangleLayer* _layer;
    RectangleLayer* _layer2;
};


#endif //TEENSYCMAKE_IDLEVIEW_H
