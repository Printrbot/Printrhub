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
}

RectangleLayer* IdleView::getLayer()
{
    return _layer;
}