//
// Created by Phillip Schuster on 19.01.16.
//

#ifndef TEENSYCMAKE_BITMAPLAYER_H
#define TEENSYCMAKE_BITMAPLAYER_H

#include <stdint.h>
#include "Layer.h"

class BitmapLayer: public Layer
{
public:
    BitmapLayer();
    BitmapLayer(Rect rect);
    virtual ~BitmapLayer();

    virtual void draw(Rect& dirtyRect, Rect& invalidationRect) override;
    virtual void setBitmap(const uint8_t* bitmap, uint16_t width, uint16_t height);

    const uint16_t &getColor() const
    {
        return _color;
    }

    void setColor(const uint16_t &color)
    {
        _color = color;
    }

private:
    const uint8_t* _bitmap;
    uint16_t _width;
    uint16_t _height;
    float _alpha;
    uint16_t _color;

public:
    float getAlpha() const
    {
        return _alpha;
    }

    void setAlpha(float _alpha)
    {
        BitmapLayer::_alpha = _alpha;
        _needsDisplay = true;
    }
};


#endif //TEENSYCMAKE_BITMAPLAYER_H
