//
// Created by Phillip Schuster on 16.01.16.
//

#include "IdleView.h"
#include "Application.h"
#include "Bitmaps.h"

IdleView::IdleView(const Rect &frame):
    View(frame)
{
    _layer = new RectangleLayer(frame);
    _layer->setBackgroundColor(ILI9341_BLUE);
    _layers.push(_layer);

    _layer2 = new RectangleLayer(frame);
    _layer2->setBackgroundColor(ILI9341_GREEN);
    _layers.push(_layer2);

    _buttonLayer = new BitmapLayer(Rect(200,10,44,40));
    _buttonLayer->setBitmap(&printerButton[0],44,40);
    _layers.push(_buttonLayer);
}

RectangleLayer* IdleView::getLayer()
{
    return _layer;
}

RectangleLayer *IdleView::getLayer2()
{
    return _layer2;
}
