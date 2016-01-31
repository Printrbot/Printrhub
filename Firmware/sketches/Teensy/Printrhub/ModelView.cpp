//
// Created by Phillip Schuster on 30.01.16.
//

#include "ModelView.h"
#include "Application.h"
#include "Bitmaps.h"

ModelView::ModelView(uint16_t x, uint16_t y, uint16_t width, uint16_t height):
        View(x,y,width,height)
{
    _jobNameLayer = NULL;
    _name = "ModelView";
}

ModelView::ModelView(Rect frame):
        View(frame)
{
    _jobNameLayer = NULL;
    _name = "ModelView";
}

void ModelView::display()
{
/*    _imageLayer = new RectangleLayer(Rect(_frame.x + 70,10,180,150));
    _imageLayer->setBackgroundColor(Application.getTheme()->getSecondaryColor1());
    addLayer(_imageLayer);*/

    _imageLayer = new BitmapLayer(Rect(_frame.x + 70,10,180,150));
    _imageLayer->setBitmap(_bitmap,180,150);
    addLayer(_imageLayer);

    uint16_t x = _frame.x + 10;
    uint16_t width = Display.textWidth(&PTSansNarrow_20,_jobName);
    x += (300 - width) / 2;

    _jobNameLayer = new TextLayer(Rect(x,190,width,25));
    _jobNameLayer->setFont(&PTSansNarrow_20);
    _jobNameLayer->setText(&_jobName);
    _jobNameLayer->setBackgroundColor(Application.getTheme()->getBackgroundColor());
    addLayer(_jobNameLayer);

    RectangleLayer* bottomLayer = new RectangleLayer(Rect(_frame.x,238,_frame.width,1));
    addLayer(bottomLayer);

    View::display();
}

