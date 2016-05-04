//
// Created by Phillip Schuster on 30.01.16.
//

#ifndef TEENSYCMAKE_MODELVIEW_H
#define TEENSYCMAKE_MODELVIEW_H

#include "View.h"
#include "TextLayer.h"
#include "RectangleLayer.h"
#include "BitmapLayer.h"
#include "SDBitmapLayer.h"
#include "LabelButton.h"

class ModelView: public View
{
public:
    ModelView(Rect frame);
    ModelView(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

    void setJobName(String jobName) { _jobName = jobName; };

    virtual void setBitmap(const uint16_t* bitmap) { _bitmap = bitmap; };
    virtual void setImageFileName(const char* imageFileName) { _imageFileName = imageFileName;};

    uint16_t _width;

private:
    const char* _imageFileName;
    TextLayer* _jobNameLayer;
    TextLayer* _materialNameLayer;
    //RectangleLayer* _imageLayer;
    SDBitmapLayer* _imageLayer;
    const uint16_t* _bitmap;
    String _jobName;
    String _materialName;
    String _printButtonText;
    String _deleteButtonText;
    TextLayer* _printButton;
    TextLayer* _deleteButton;
public:
    virtual void display() override;
};


#endif //TEENSYCMAKE_MODELVIEW_H
