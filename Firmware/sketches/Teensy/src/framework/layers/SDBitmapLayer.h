//
// Created by Phillip Schuster on 22.02.16.
//

#ifndef TEENSYCMAKE_SDBITMAPLAYER_H
#define TEENSYCMAKE_SDBITMAPLAYER_H

#include "Layer.h"
#include "../../drivers/SD/SD.h"

class SDBitmapLayer: public Layer
{
#pragma mark Constructor
public:
    SDBitmapLayer();
    SDBitmapLayer(Rect rect);
    virtual ~SDBitmapLayer();

#pragma mark Layer
    virtual void draw(Rect& dirtyRect, Rect& invalidationRect) override;

#pragma mark Getter/Setter
    virtual void setBitmap(const char *filePath, uint16_t width, uint16_t height);

#pragma mark Member Variables
private:
    const char *_filePath;
    File _file;
    uint16_t _width;
    uint16_t _height;
};

#endif //TEENSYCMAKE_SDBITMAPLAYER_H
