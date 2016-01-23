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

void SceneController::setup()
{

}

void SceneController::loop()
{
	if (Touch.touched()) return;

	if (_scrollVelocity > 0)
	{
		_scrollVelocity -= fabs(_scrollVelocity) * 0.1;
		if (_scrollVelocity < 0) _scrollVelocity = 0;
	}
	else if (_scrollVelocity < 0)
	{
		_scrollVelocity += fabs(_scrollVelocity) * 0.1;
		if (_scrollVelocity > 0) _scrollVelocity = 0;
	}

	if(_scrollVelocity != 0)
	{
		addScrollOffset(_scrollVelocity);
	}
}

void SceneController::onWillAppear()
{
	//Clear display - override if you want a nice transition effect
	uint16_t backgroundColor = Application.getTheme()->getBackgroundColor(ColorTheme::Default);
	Display.fillScreen(backgroundColor);
}

void SceneController::onWillDisappear()
{
	Application.resetFocus();
}

String SceneController::getName()
{
	return "SceneController";
}

bool SceneController::statusBarVisible()
{
	return true;
}

void SceneController::handleTouchDown(TS_Point &point)
{
	for (int i=0;i<_views.count();i++)
	{
		View* view = _views.at(i);
		View* hitView = view->hitTest(point);
		if (hitView != NULL)
		{
			LOG("Touch Down in View");
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
				LOG("Touch Up in View");
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

	if (_scrollOffset < -230)
	{
		_scrollOffset = -230;
	}
	if (_scrollOffset > 0)
	{
		_scrollOffset = 0;
	}

	Display.setScrollOffset(_scrollOffset);
}

void SceneController::handleTouchMoved(TS_Point point, TS_Point oldPoint)
{
	for (int i=0;i<_views.count();i++)
	{
		View* view = _views.at(i);
		View* hitView = view->hitTest(point);
		if (hitView != NULL)
		{
			LOG("Touch Moved in View");
			//Break out if the view returns true, means it has handled the event
			if (hitView->touchMoved(point,oldPoint))
			{
				return;
			}
		}
	}

	LOG("Handle Scrolling");

	//Handle Scrolling
	_scrollVelocity = point.x - oldPoint.x;
	addScrollOffset(_scrollVelocity);
}
