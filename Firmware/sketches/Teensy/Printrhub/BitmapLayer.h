//
// Created by Phillip Schuster on 19.01.16.
//

#ifndef TEENSYCMAKE_BITMAPLAYER_H
#define TEENSYCMAKE_BITMAPLAYER_H

#include "Layer.h"

class BitmapLayer: public Layer
{
public:
    BitmapLayer();
    BitmapLayer(Rect rect);
    virtual ~BitmapLayer();

    virtual void draw();
    virtual void setBitmap(const uint16_t* bitmap, uint16_t width, uint16_t height);

private:
    const uint16_t* _bitmap;
    uint16_t _width;
    uint16_t _height;
};


#endif //TEENSYCMAKE_BITMAPLAYER_H
