//
// Created by Phillip Schuster on 20.01.16.
//

#include "BitmapButton.h"
#include "BitmapLayer.h"

BitmapButton::BitmapButton(Rect frame):
        View(frame)
{

}

void BitmapButton::setBitmap(const uint16_t *bitmap, uint16_t width, uint16_t height)
{
    BitmapLayer* layer = new BitmapLayer(_frame);
    layer->setBitmap(bitmap,width,height);

    addLayer(layer);
}

BitmapButton::~BitmapButton()
{

}

void BitmapButton::setFrame(Rect frame)
{
    if (_frame == frame) return;

    View::setFrame(frame);

    _layers.at(0)->setFrame(frame);
}
