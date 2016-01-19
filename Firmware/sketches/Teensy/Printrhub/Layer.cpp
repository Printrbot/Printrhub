//
// Created by Phillip Schuster on 16.01.16.
//

#include "Layer.h"
#include "Application.h"
#include "GapLayer.h"

Layer::Layer():
_strokeColor(ILI9341_BLACK),
_backgroundColor(ILI9341_BLACK),
_strokeWidth(0),
_sublayers(NULL)
{
    ::globalLayersCreated++;
}

Layer::Layer(const Rect &frame):
Layer()
{
    _frame = frame;
}

Layer::~Layer()
{
    LOG_VALUE("Layer deleted with ID",this->uniqueId);

    ::globalLayersDeleted++;

    removeAllSublayers();

    delete _sublayers;
    _sublayers = NULL;
}

void Layer::setBackgroundColor(const uint16_t &color)
{
    _backgroundColor = color;
}

uint16_t Layer::getBackgroundColor() const
{
    return _backgroundColor;
}

void Layer::setStrokeColor(const uint16_t &strokeColor)
{
    _strokeColor = strokeColor;
}

uint16_t Layer::getStrokeColor() const
{
    return _strokeColor;
}

void Layer::setStrokeWidth(const uint8_t strokeWidth)
{
    _strokeWidth = strokeWidth;
}

uint8_t Layer::getStrokeWidth() const
{
    return _strokeWidth;
}

bool Layer::isLeaf()
{
    if (_sublayers == NULL) return true;
    if (_sublayers->count() <= 0) return true;
    return false;
}

void Layer::splitVertically(int x, Layer** left, Layer** right)
{
    if (!_frame.containsX(x))
    {
        *left = NULL;
        *right = NULL;
        return;
    }

    if (left != NULL)
    {
        //Create the top layer
        Layer* leftLayer = new GapLayer(Rect(_frame.x,_frame.y,x-_frame.x,_frame.bottom()));
        leftLayer->uniqueId = ::globalLayerId++;
        *left = leftLayer;
        LOG_VALUE("Layer created",leftLayer->uniqueId);
    }

    if (right != NULL)
    {
        Layer* rightLayer = new GapLayer(Rect(x,_frame.y,_frame.right()-x,_frame.bottom()));
        rightLayer->uniqueId = ::globalLayerId++;
        *right = rightLayer;
        LOG_VALUE("Layer created",rightLayer->uniqueId);
    }
}

/*
 * receiver will receive the left layer in 0 and (if available) the right lay
 *
 */
void Layer::splitHorizontally(int y, Layer**top, Layer**bottom)
{
    if (!_frame.containsY(y))
    {
        *top = NULL;
        *bottom = NULL;
        return;
    }

    //Create the top layer
    if (top != NULL)
    {
        Layer* topLayer = new GapLayer(Rect(_frame.x,_frame.y,_frame.width,y-_frame.y));
        topLayer->uniqueId = ::globalLayerId++;
        *top = topLayer;
        LOG_VALUE("Layer created",topLayer->uniqueId);
    }

    if (bottom != NULL)
    {
        Layer* bottomLayer = new GapLayer(Rect(_frame.x,y,_frame.width,_frame.bottom()-y));
        bottomLayer->uniqueId = ::globalLayerId++;
        *bottom = bottomLayer;
        LOG_VALUE("Layer created",bottomLayer->uniqueId);
    }
}

void Layer::splitWithRect(Rect& rect)
{
    LOG("splitWithRect:001");
    if (!isLeaf())
    {
        LOG("Has Sublayer, split sublayers");
        LOG("splitWithRect:002");
        //This layer has sublayers, so we have to split the sublayers
        for (int i=0;i<_sublayers->count();i++)
        {
            LOG("splitSublayer");
            Layer* layer = _sublayers->at(i);
            layer->splitWithRect(rect);
        }
    }
    else
    {
        LOG("splitWithRect:003");
        //This is a leaf layer, do the actual work.
        if (!_frame.intersectsRect(rect)) return;

        LOG("splitWithRect:004");

        //Split the left edge (contains x does not include the "outline"
        Layer* left = NULL;
        Layer* right = NULL;
        Layer* top = NULL;
        Layer* bottom = NULL;
        if (_frame.containsX(rect.left()))
        {
            LOG("splitWithRect:005");
            splitVertically(rect.left(), &left, &right);

            addSublayer(left);
            addSublayer(right);

            LOG("splitWithRect:006");
            right->splitWithRect(rect);
        }
        else if (_frame.containsX(rect.right()))
        {
            LOG("splitWithRect:007");
            splitVertically(rect.right(), &left, &right);

            addSublayer(left);
            addSublayer(right);

            LOG("splitWithRect:008");

            left->splitWithRect(rect);
        }
        else if (_frame.containsY(rect.top()))
        {
            LOG("splitWithRect:009");
            splitHorizontally(rect.top(),&top,&bottom);

            addSublayer(top);
            addSublayer(bottom);

            LOG("splitWithRect:010");
            bottom->splitWithRect(rect);
        }
        else if (_frame.containsY(rect.bottom()))
        {
            LOG("splitWithRect:011");
            splitHorizontally(rect.bottom(),&top,&bottom);

            //TODO: Memory Bug!


            addSublayer(bottom);

            LOG("splitWithRect:012");
            top->splitWithRect(rect);
        }
    }
}

void Layer::display()
{
    LOG("Sending layer");
    if (_sublayers == NULL || _sublayers->count() <= 0)
    {
        LOG("No sublayers, just draw");

        draw();
        return;
    }

    LOG("Sublayers");
    for (int i=0;i<_sublayers->count();i++)
    {
        LOG_VALUE("Drawing sublayer at",i);
        Layer* layer = _sublayers->at(i);
        layer->display();
    }
}

void Layer::addSublayer(Layer *layer)
{
    getSublayers()->push(layer);
}

StackArray<Layer *> *Layer::getSublayers()
{
    if (_sublayers == NULL)
    {
        _sublayers = new StackArray<Layer*>();
    }
    return _sublayers;
}

void Layer::removeAllSublayers()
{
    if (_sublayers == NULL) return;

    while (_sublayers->count() > 0)
    {
        Layer* layer = _sublayers->pop();
        delete layer;
    }
}