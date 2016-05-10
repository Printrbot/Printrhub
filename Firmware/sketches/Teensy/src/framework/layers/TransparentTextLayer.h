//
// Created by Phillip Schuster on 11.05.16.
//

#ifndef TEENSY_TRANSPARENTTEXTLAYER_H
#define TEENSY_TRANSPARENTTEXTLAYER_H


#include "TextLayer.h"

class TransparentTextLayer : public TextLayer
{
#pragma mark Constructor
public:
    TransparentTextLayer() {};
    TransparentTextLayer(Rect frame);
    virtual ~TransparentTextLayer() {};

public:
    virtual void draw(Rect& dirtyRect, Rect& invalidationRect) override;
};


#endif //TEENSY_TRANSPARENTTEXTLAYER_H
