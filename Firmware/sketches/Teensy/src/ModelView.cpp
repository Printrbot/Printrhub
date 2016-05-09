//
// Created by Phillip Schuster on 30.01.16.
//

#include "ModelView.h"
#include "framework/core/Application.h"
#include "Bitmaps.h"
#include "framework/layers/SDBitmapLayer.h"

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

    _imageLayer = new SDBitmapLayer(Rect(_frame.x + 9,59,162,129));
    _imageLayer->setBitmap(_imageFileName,180,150);
    addLayer(_imageLayer);

    _jobNameLayer = new TextLayer(Rect(_frame.x + 9,5,267-9,30));
    _jobNameLayer->setFont(&PTSansNarrow_28);
    _jobNameLayer->setText(_jobName);
    addLayer(_jobNameLayer);

    _materialNameLayer = new TextLayer(Rect(_frame.x + 9,37,267-9,14));
    _materialNameLayer->setFont(&PTSansNarrow_14);
    _materialNameLayer->setText(_materialName);
    addLayer(_materialNameLayer);

    _printButton = new TextLayer(Rect(_frame.x + 9 + 162 + 3,59,93,63));
    _printButton->setText(_printButtonText);
    _printButton->setFont(&PTSansNarrow_18);
    _printButton->setTextAlign(TEXTALIGN_CENTERED);
    _printButton->setVerticalTextAlign(TEXTALIGN_CENTERED);
    addLayer(_printButton);

    _deleteButton = new TextLayer(Rect(_frame.x + 9 + 162 + 3,125,93,63));
    _deleteButton->setText(_deleteButtonText);
    _deleteButton->setFont(&PTSansNarrow_18);
    _deleteButton->setTextAlign(TEXTALIGN_CENTERED);
    _deleteButton->setVerticalTextAlign(TEXTALIGN_CENTERED);
    addLayer(_deleteButton);

 /*   RectangleLayer* bottomLayer = new RectangleLayer(Rect(_frame.x,238,_frame.width,1));
    bottomLayer->setStrokeColor(ILI9341_WHITE);
    addLayer(bottomLayer);*/

    View::display();
}

