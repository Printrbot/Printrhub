//
// Created by Phillip Schuster on 16.01.16.
//

#ifndef TEENSYCMAKE_RECTANGLELAYER_H
#define TEENSYCMAKE_RECTANGLELAYER_H

#include "Layer.h"

class RectangleLayer: public Layer
{
public:
    RectangleLayer(Rect frame);
    virtual ~RectangleLayer();

    virtual void draw();
};


#endif //TEENSYCMAKE_RECTANGLELAYER_H
