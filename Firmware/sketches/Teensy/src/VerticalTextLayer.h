//
// Created by Phillip Schuster on 04.05.16.
//

#ifndef TEENSY_VERTICALTEXTLAYER_H
#define TEENSY_VERTICALTEXTLAYER_H


#include "TextLayer.h"

class VerticalTextLayer : public TextLayer
{
public:
	virtual void draw(Rect &dirtyRect, Rect &invalidationRect) override;

	VerticalTextLayer() : TextLayer() { }
	VerticalTextLayer(const Rect &frame) : TextLayer(frame) { }



};


#endif //TEENSY_VERTICALTEXTLAYER_H
