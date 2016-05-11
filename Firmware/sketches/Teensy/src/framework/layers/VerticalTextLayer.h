//
// Created by Phillip Schuster on 04.05.16.
//

#ifndef TEENSY_VERTICALTEXTLAYER_H
#define TEENSY_VERTICALTEXTLAYER_H


#include "TextLayer.h"

class VerticalTextLayer : public TextLayer
{
#pragma mark Constructor
public:
	VerticalTextLayer() : TextLayer() { }
	VerticalTextLayer(const Rect &frame) : TextLayer(frame) { }

#pragma mark Layer
	virtual void draw(Rect &invalidationRect) override;
};


#endif //TEENSY_VERTICALTEXTLAYER_H
