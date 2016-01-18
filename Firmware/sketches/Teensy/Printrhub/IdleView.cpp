//
// Created by Phillip Schuster on 16.01.16.
//

#include "IdleView.h"
#include "Application.h"

IdleView::IdleView(const Rect &frame):
    View(frame)
{
    _layer = new RectangleLayer(frame);
    _layer->setBackgroundColor(ILI9341_BLUE);
    _layers.push(_layer);

    _layer2 = new RectangleLayer(frame);
    _layer2->setBackgroundColor(ILI9341_GREEN);
    _layers.push(_layer2);
}

RectangleLayer* IdleView::getLayer()
{
    return _layer;
}

RectangleLayer *IdleView::getLayer2()
{
    return _layer2;
}
