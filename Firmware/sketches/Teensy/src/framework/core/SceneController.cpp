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
	_scrollSnap = 0;
	_decelerationRate = 270.0f*7;   //Pixels per Second
	_scrollAnimation = NULL;
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

	if (_scrollSnap == 0)
	{
		if (_scrollVelocity > 0)
		{
			_scrollVelocity -= (_decelerationRate * Application.getDeltaTime());
			if (_scrollVelocity < 0) _scrollVelocity = 0;
		}
		else if (_scrollVelocity < 0)
		{
			_scrollVelocity += (_decelerationRate * Application.getDeltaTime());
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
	_scrollOffset = 0;
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
	LOG("Check");
	//Stop scrolling immediately
	_scrollVelocity = 0;

	if (_scrollAnimation != NULL)
	{
		_scrollAnimation->stop();
		_scrollAnimation = NULL;
	}

	LOG("Check");

	for (int i=0;i<_views.count();i++)
	{
		LOG("Check");
		View* view = _views.at(i);
		View* hitView = view->hitTest(point);
		if (hitView != NULL)
		{
			LOG_VALUE("Touch Down in View: ",hitView->getName());
			//Break out if the view returns true, means it has handled the event
			if (hitView->touchDown(point))
			{
				LOG("Check");
				_currentTouchedView = hitView;
				break;
			}
		}
	}
	LOG("Check");
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

	//Handle snapping
	if (_snapMode == SnapMode::OneByOne)
	{
		if (_scrollAnimation != NULL)
		{
			_scrollAnimation->stop();
		}
		float scrollOffset = roundf(_scrollOffset / _scrollSnap) * _scrollSnap;
		Animation* animation = Animator.getAnimationSlot();
		if (!animation != NULL)
		{
			animation->setTargetValue(scrollOffset);
			animation->setDuration(0.2);
			animation->setKey("scrollOffset");
			animation->setInitialValue(_scrollOffset);
			addAnimation(animation);
			_scrollAnimation = animation;
		}
	}
	else
	{
		//Calculate Target Offset based on current speed and decelerationRate
		float stoppingDistance = (_scrollVelocity*_scrollVelocity)/(2*_decelerationRate);
		float targetOffset = _scrollOffset;

		//Clamp snapping distance to max 3 slots
		float maxDistance = _scrollSnap*2;
		if (stoppingDistance < -maxDistance)
		{
			stoppingDistance = -maxDistance;
		}
		if (stoppingDistance > maxDistance)
		{
			stoppingDistance = maxDistance;
		}

		if (_scrollVelocity > 0)
		{
			targetOffset = _scrollOffset + stoppingDistance;
		}
		else
		{
			targetOffset = _scrollOffset - stoppingDistance;
		}

		//Clamp the scroll target that it fits in the current space
		float snapPosition = roundf(targetOffset/_scrollSnap) * _scrollSnap;
		snapPosition = Display.clampScrollTarget(snapPosition);

		LOG_VALUE("Velocity",_scrollVelocity);
		LOG_VALUE("Stopping-Distance",stoppingDistance);
		LOG_VALUE("Offset",_scrollOffset);
		LOG_VALUE("Target-Offset",targetOffset);
		LOG_VALUE("Snap-Position",snapPosition);

		_scrollVelocity = 0;
		if (_scrollAnimation != NULL)
		{
			_scrollAnimation->stop();
		}
		Animation* animation = Animator.getAnimationSlot();
		if (animation != NULL)
		{
			animation->setTargetValue(snapPosition);
			animation->setDuration(0.6);
			animation->setKey("scrollOffset");
			animation->setInitialValue(_scrollOffset);
			addAnimation(animation);
			_scrollAnimation = animation;
		}
	}
}

void SceneController::addScrollOffset(float scrollOffset)
{
	if (scrollOffset == 0) return;

	//_scrollOffset += scrollOffset;
	//LOG_VALUE("_scrollOffset: ",scrollOffset + _scrollOffset);

	Display.setScrollOffset(_scrollOffset + scrollOffset);
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
	_scrollVelocity = 0;
	if (Application.getDeltaTime() > 0)
	{
		_scrollVelocity = (point.x - oldPoint.x)/Application.getDeltaTime();
	}

	LOG_VALUE("Point.X: ",point.x);
	LOG_VALUE("OldPoint.X: ",oldPoint.x);
	LOG_VALUE("Velocity: ",_scrollVelocity);
	addScrollOffset((point.x - oldPoint.x));
}

uint16_t SceneController::getBackgroundColor()
{
	return Application.getTheme()->getColor(BackgroundColor);
}

#pragma mark AnimatableObject


void SceneController::animationUpdated(Animation *animation, float currentValue, float deltaValue, float timeLeft)
{
	if (animation == _scrollAnimation)
	{
		//LOG_VALUE("Animation-Delta",deltaValue);
		//LOG_VALUE("Animation-Current",currentValue);

		addScrollOffset(deltaValue);
	}
}

void SceneController::animationFinished(Animation *animation)
{
	if (animation == _scrollAnimation)
	{
		LOG("Scroll animation finished");
		_scrollAnimation = NULL;
	}

	AnimatableObject::animationFinished(animation);
}

