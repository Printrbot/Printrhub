//
// Created by Phillip Schuster on 30.01.16.
//

#include "ModelView.h"
#include "framework/core/Application.h"
#include "Bitmaps.h"
#include "framework/layers/SDBitmapLayer.h"
#include "framework/layers/TransparentTextLayer.h"

ModelView::ModelView(uint16_t x, uint16_t y, uint16_t width, uint16_t height):
        View(x,y,width,height)
{
    //TODO: Set material from database, file (currently hardcoded to PLA)
    _jobNameLayer = NULL;
    _materialNameLayer = NULL;
    _name = "ModelView";
    _materialName = "Material: PLA";
    _printButtonText = "PRINT";
    _deleteButtonText = "DELETE";
}

ModelView::ModelView(Rect frame):
        View(frame)
{
    //TODO: Set material from database, file (currently hardcoded to PLA)
    _jobNameLayer = NULL;
    _materialNameLayer = NULL;
    _name = "ModelView";
    _materialName = "Material: PLA";
    _printButtonText = "PRINT";
    _deleteButtonText = "DELETE";
}



void ModelView::display()
{
/*    _imageLayer = new RectangleLayer(Rect(_frame.x + 70,10,180,150));
    _imageLayer->setBackgroundColor(Application.getTheme()->getSecondaryColor1());
    addLayer(_imageLayer);*/

/*    _imageLayer = new RectangleLayer(_frame);
    _imageLayer->setBackgroundColor(_color);
    addLayer(_imageLayer);*/

    SDBitmapLayer* imageLayer = new SDBitmapLayer(_frame);
    imageLayer->setBitmap(_imageFileName.c_str(), 270, 240);
    Display.addLayer(imageLayer);

    _imageLayer = imageLayer;

    _jobNameLayer = new TransparentTextLayer(Rect(_frame.x+15,_frame.y+10,Display.getLayoutWidth()-30,25));
    _jobNameLayer->setTextAlign(TEXTALIGN_LEFT);
    _jobNameLayer->setFont(&PTSansNarrow_24);
    _jobNameLayer->setText(_jobName);
    _jobNameLayer->setForegroundColor(Application.getTheme()->getColor(OverlayTextColor));
    addLayer(_jobNameLayer);

    View::display();
}
