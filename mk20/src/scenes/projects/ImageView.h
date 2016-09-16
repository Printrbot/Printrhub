#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H

#include "../../framework/views/View.h"
#include "../../framework/layers/TextLayer.h"
#include "../../framework/layers/RectangleLayer.h"
#include "../../framework/layers/BitmapLayer.h"
#include "../../framework/layers/SDBitmapLayer.h"
#include "../../framework/views/LabelButton.h"

class ImageView: public View
{
public:
    ImageView(Rect frame, uint32_t offset = 0);
    ImageView(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t offset = 0);

    void setImageTitle(String imageTitle) { _imageTitle = imageTitle; };

    void setBitmap(const uint16_t* bitmap) { _bitmap = bitmap; };
    void setColor(const uint16_t color) { _color = color; };
    void setIndexFileName(String fileName) { _indexFileName = fileName; };
    void setJobFileName(String fileName) { _jobFileName = fileName; };
    String getIndexFileName() { return _indexFileName; };

    uint16_t _width;
    virtual void display() override;

private:
    String _imageTitle;
    String _indexFileName;
    String _jobFileName;
    TextLayer* _imageTitleLayer;
    Layer* _imageLayer;
    const uint16_t* _bitmap;
    uint16_t _color;
    uint32_t _offset;
};


#endif
