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

    virtual void draw(Rect& renderFrame) override;
    virtual void setBitmap(const uint16_t* bitmap, uint16_t width, uint16_t height);

private:
    const uint16_t* _bitmap;
    uint16_t _width;
    uint16_t _height;
    float _alpha;

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
