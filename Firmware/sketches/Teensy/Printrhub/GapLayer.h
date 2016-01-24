//
// Created by Phillip Schuster on 17.01.16.
//

#ifndef TEENSYCMAKE_GAPLAYER_H
#define TEENSYCMAKE_GAPLAYER_H

#include "RectangleLayer.h"

class GapLayer: public RectangleLayer
{
public:
    GapLayer(Rect frame);
    virtual ~GapLayer();

    //virtual void draw(Rect &renderFrame) override;
};


#endif //TEENSYCMAKE_GAPLAYER_H
