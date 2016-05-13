/*
 * Little Helper -- your best friend in your lab
 * http://www.appfruits.com/littlehelper
 *
 * This software is part of the firmware running Little Helper. You may use, copy, 
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software 
 * but you must include this copyright notice and this permission in all copies or 
 * substantial portions of the Software.
 *
 * Appfruits invests time and resources to make this open source. Please support us
 * and the open-source hardware initiative.
 * 
 * Copyright 2014-2015 Phillip Schuster (@appfruits)
 * http://www.appfruits.com
 * 
 * MIT-License
 */

#include "View.h"
#include "../core/Application.h"

View::~View()
{
	while (_layers.count() > 0)
	{
		Layer* layer = _layers.pop();
		delete layer;
	}
}

View::View(int x, int y, int width, int height)
{
	_frame = Rect(x,y,width,height);
	_opaque = false;
	_needsDisplay = true;
	_visible = true;
	_userInteractionEnabled = true;
	_name = "Untitled";
	_context = DisplayContext::Scrolling;
}

View::View(Rect frame)
{
	_frame = frame;
	_userInteractionEnabled = true;
	_name = "Untitled";
	_context = DisplayContext::Scrolling;
}

void View::draw()
{
	//LOG_VALUE("Draw",this->getDescription());

	if (_needsDisplay && _opaque)
	{
		//display.fillRect(_frame.x,_frame.y,_frame.width,_frame.height,_backgroundColor);
	}
}

void View::update()
{
	if (isVisible())
	{
		if (this->_needsDisplay)
		{
			//LOG_VALUE("Draw",this->getDescription());
			draw();
		}
	}
	else
	{
		//LOG_VALUE("Draw skipped, invisible",this->getDescription());
	}

	didDraw();
}

bool View::isVisible()
{
	return _visible;
}


void View::log()
{
	LOG_VALUE("View",(int)this);
	LOG_VALUE("Frame",_frame.toString());
}


void View::setVisible(bool visible)
{
	if (_visible == visible) return;
	_visible = visible;

	for (int i=0;i<_layers.count();i++)
	{
		Layer *layer = _layers.at(i);
		layer->setVisible(visible);
	}

	if (!visible)
	{
//		display.fillRect(_frame,Application.getTheme()->getBackgroundColor());
	}

	setNeedsDisplay();
}

void View::animationUpdated(Animation *animation, float currentValue, float deltaValue, float timeLeft)
{
	if (animation->getKey() == "frame.x")
	{
		//display.fillRect(_frame,Application.getTheme()->getBackgroundColor());
		_frame.x = currentValue;
		layout();
	}
}

void View::layout()
{
	setNeedsDisplay();
}

void View::didDraw()
{
	if (this->_needsDisplay == false) return;

	//LOG_VALUE("Did draw",this->getDescription());

	this->_needsDisplay = false;
}

void View::setBackgroundColor(uint16_t backgroundColor)
{
	_backgroundColor = backgroundColor;
}

uint16_t View::getBackgroundColor()
{
	return _backgroundColor;
}

void View::setOpaque(bool opaque)
{
	_opaque = opaque;
}

void View::setNeedsDisplay()
{
	for (int i=0;i<_layers.count();i++)
	{
		Layer *layer = _layers.at(i);
		layer->setNeedsDisplay();
	}
}

String View::getDescription()
{
	return "View";
}

void View::display()
{
	for (int i=0;i<_layers.count();i++)
	{
		Layer* layer = _layers.at(i);
		Display.addLayer(layer);
	}
}

void View::addLayer(Layer *layer)
{
	layer->setContext(getContext());
	_layers.push(layer);
}

void View::setFrame(Rect frame)
{
	UIElement::setFrame(frame);

	Display.setNeedsLayout();
}

bool View::touchDown(TS_Point &point)
{
	LOG_VALUE("TOUCH DOWN",_name);
	//Return false so the next view is informed (when Views overlap)
	return false;
}

bool View::touchMoved(TS_Point &point, TS_Point &lastPoint)
{
	//Return false so the next view is informed (when Views overlap)
	return false;
}

bool View::touchUp(TS_Point &point)
{
	//Return false so the next view is informed (when Views overlap)
	return false;
}

void View::touchCancelled()
{
}

View *View::hitTest(TS_Point &point)
{
	if (this->getContext() == DisplayContext::Fixed)
	{
		//Transform point to view space as point is in screen-space (i.e. 0-320 wrapped)
		if (_frame.containsPoint(point.x,point.y))
		{
			return this;
		}
	}
	else
	{
//		LOG_VALUE("Hit testing view with rect",_frame.toString());
//		LOG_VALUE("Point-X",point.x);
//		LOG_VALUE("Transformed Point-X",point.x+(-Display.getScrollOffset())-Display.getLayoutStart());
//		LOG_VALUE("Point-Y",point.y);
//		LOG_VALUE("ScrollOffset",Display.getScrollOffset());
//		LOG_VALUE("Display Start",Display.getLayoutStart());

		//Transform point to view space as point is in screen-space (i.e. 0-320 wrapped)
		if (_frame.containsPoint(point.x+(-Display.getScrollOffset())-Display.getLayoutStart(),point.y))
		{
			return this;
		}
	}

	return NULL;
}


void View::setContext(const DisplayContext context)
{
	UIElement::setContext(context);

	for (int i=0;i<_layers.count();i++)
	{
		Layer* layer = _layers.at(i);
		layer->setContext(getContext());
	}

	setNeedsDisplay();
}

