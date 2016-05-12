//
// Created by Phillip Schuster on 30.01.16.
//

#ifndef TEENSYCMAKE_MODELVIEW_H
#define TEENSYCMAKE_MODELVIEW_H

#include "framework/views/View.h"
#include "framework/layers/TextLayer.h"
#include "framework/layers/RectangleLayer.h"
#include "framework/layers/BitmapLayer.h"
#include "framework/layers/SDBitmapLayer.h"
#include "framework/views/LabelButton.h"

class ModelView: public View
{
public:
    ModelView(Rect frame);
    ModelView(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

    void setJobName(String jobName) { _jobName = jobName; };

    virtual void setBitmap(const uint16_t* bitmap) { _bitmap = bitmap; };
    virtual void setImageFileName(const char* imageFileName) { _imageFileName = imageFileName;};
    virtual void setColor(const uint16_t color) { _color = color; };

    uint16_t _width;

private:
    const char* _imageFileName;
    TextLayer* _jobNameLayer;
    TextLayer* _materialNameLayer;
    //RectangleLayer* _imageLayer;
    Layer* _imageLayer;
    const uint16_t* _bitmap;
    String _jobName;
    String _materialName;
    String _printButtonText;
    String _deleteButtonText;
    TextLayer* _printButton;
    TextLayer* _deleteButton;
    uint16_t _color;
public:
    virtual void display() override;
};


#endif //TEENSYCMAKE_MODELVIEW_H
