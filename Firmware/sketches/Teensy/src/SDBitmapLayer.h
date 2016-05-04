//
// Created by Phillip Schuster on 22.02.16.
//

#ifndef TEENSYCMAKE_SDBITMAPLAYER_H
#define TEENSYCMAKE_SDBITMAPLAYER_H

#include "Layer.h"
#include "SD.h"

class SDBitmapLayer: public Layer
{
public:
    SDBitmapLayer();
    SDBitmapLayer(Rect rect);
    virtual ~SDBitmapLayer();

    virtual void draw(Rect& dirtyRect, Rect& invalidationRect) override;
    virtual void setBitmap(const char *filePath, uint16_t width, uint16_t height);

private:
    const char *_filePath;
    File _file;
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
        SDBitmapLayer::_alpha = _alpha;
        _needsDisplay = true;
    }
};


#endif //TEENSYCMAKE_SDBITMAPLAYER_H
