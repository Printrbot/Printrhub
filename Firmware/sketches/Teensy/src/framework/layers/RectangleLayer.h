//
// Created by Phillip Schuster on 16.01.16.
//

#ifndef TEENSYCMAKE_RECTANGLELAYER_H
#define TEENSYCMAKE_RECTANGLELAYER_H

#include "Layer.h"

class RectangleLayer: public Layer
{
#pragma mark Constructor
public:
    RectangleLayer(Rect frame);
    virtual ~RectangleLayer();

#pragma mark Layer
    virtual void draw(Rect &invalidationRect) override;
};


#endif //TEENSYCMAKE_RECTANGLELAYER_H
