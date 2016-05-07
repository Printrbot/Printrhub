//
// Created by Phillip Schuster on 19.01.16.
//

#ifndef TEENSYCMAKE_BITMAPLAYER_H
#define TEENSYCMAKE_BITMAPLAYER_H

#include <stdint.h>
#include "Layer.h"

class BitmapLayer: public Layer
{
#pragma mark Constrcutor
public:
    BitmapLayer();
    BitmapLayer(Rect rect);
    virtual ~BitmapLayer();

#pragma mark Layer
    virtual void draw(Rect& dirtyRect, Rect& invalidationRect) override;
    virtual void setBitmap(const uint8_t* bitmap, uint16_t width, uint16_t height);

#pragma mark Getter/Setter
    const uint16_t &getColor() const { return _color; }
    void setColor(const uint16_t &color) { _color = color; }

#pragma mark Member Variables
private:
    const uint8_t* _bitmap;
    uint16_t _width;
    uint16_t _height;
    uint16_t _color;
};


#endif //TEENSYCMAKE_BITMAPLAYER_H
