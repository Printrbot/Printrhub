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

#include "SceneController.h"
#include "Application.h"

SceneController::SceneController()
{
	_currentTouchedView = NULL;
	_scrollOffset = 0;
}

SceneController::~SceneController()
{
	while (_views.count() > 0)
	{
		View* view = _views.pop();
		delete view;
	}
}

void SceneController::loop()
{
	if (Touch.touched()) return;

	if (_scrollVelocity > 0)
	{
		_scrollVelocity -= fabs(_scrollVelocity) * 0.1 * 0.5;
		if (_scrollVelocity < 0) _scrollVelocity = 0;
	}
	else if (_scrollVelocity < 0)
	{
		_scrollVelocity += fabs(_scrollVelocity) * 0.1 * 0.5;
		if (_scrollVelocity > 0) _scrollVelocity = 0;
	}

	if (fabsf(_scrollVelocity) < 0.05)
	{
		_scrollVelocity = 0;
	}

	if(_scrollVelocity != 0)
	{
		addScrollOffset(_scrollVelocity);
	}
}

void SceneController::onWillAppear()
{
	//Clear display - override if you want a nice transition effect
//	uint16_t backgroundColor = Application.getTheme()->getBackgroundColor(ColorTheme::Default);
//	Display.fillScreen(backgroundColor);

	for (int i=0;i<_views.count();i++)
	{
		LOG_VALUE("Display View: ",_views.count());
		View *view = _views.at(i);
		view->display();
		LOG("View displayed");
	}
}

void SceneController::setupDisplay()
{
	Display.setScrollInsets(0,0);
	Display.setScroll(0);
}

void SceneController::onWillDisappear()
{

}

String SceneController::getName()
{
	return "SceneController";
}

void SceneController::handleTouchDown(TS_Point &point)
{
	for (int i=0;i<_views.count();i++)
	{
		View* view = _views.at(i);
		View* hitView = view->hitTest(point);
		if (hitView != NULL)
		{
			LOG_VALUE("Touch Down in View: ",hitView->getName());
			//Break out if the view returns true, means it has handled the event
			if (hitView->touchDown(point))
			{
				_currentTouchedView = hitView;
				break;
			}
		}
	}
}

void SceneController::handleTouchUp(TS_Point &point)
{
	for (int i=0;i<_views.count();i++)
	{
		View* view = _views.at(i);
		View* hitView = view->hitTest(point);

		//Only send touch up to those views that received a touch down before
		if (hitView != NULL)
		{
			if (hitView == _currentTouchedView)
			{
				//This delay makes sure we can see the action previously triggered by touchDown (i.e. drawing a control highlighted)
				delay(200);

				LOG_VALUE("Touch Up in View: ",hitView->getName());
				//Break out if the view returns true, means it has handled the event
				if (hitView->touchUp(point))
				{
					return;
				}
			}
		}
	}

	//If we are here and have _currentTouchedView we have to send a cancel
	//as the touch moved away from the control out of its boundaries
	if (_currentTouchedView != NULL)
	{
		_currentTouchedView->touchCancelled();
		_currentTouchedView = NULL;
	}
}

void SceneController::addScrollOffset(float scrollOffset)
{
	if (scrollOffset == 0) return;

	_scrollOffset += scrollOffset;
	LOG_VALUE("_scrollOffset: ",_scrollOffset);

	Display.setScrollOffset(_scrollOffset);
	_scrollOffset = Display.getScrollOffset();
}

void SceneController::handleTouchMoved(TS_Point point, TS_Point oldPoint)
{
	for (int i=0;i<_views.count();i++)
	{
		View* view = _views.at(i);
		View* hitView = view->hitTest(point);
		if (hitView != NULL)
		{
			LOG_VALUE("Touch Moved in View: ",hitView->getName());
			//Break out if the view returns true, means it has handled the event
			if (hitView->touchMoved(point,oldPoint))
			{
				return;
			}
		}
	}

	//Current touched view does not handle move, so we are likely scrolling
	if (_currentTouchedView)
	{
		_currentTouchedView->touchCancelled();
		_currentTouchedView = NULL;
	}

	LOG("Handle Scrolling");

	//Handle Scrolling
	_scrollVelocity = point.x - oldPoint.x;

	LOG_VALUE("Point.X: ",point.x);
	LOG_VALUE("OldPoint.X: ",oldPoint.x);
	LOG_VALUE("Velocity: ",_scrollVelocity);
	addScrollOffset(_scrollVelocity);
}

uint16_t SceneController::getBackgroundColor()
{
	return Application.getTheme()->getColor(BackgroundColor);
}
