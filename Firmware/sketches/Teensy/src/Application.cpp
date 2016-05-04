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

#include "Application.h"
#include "SceneController.h"
#include "Animator.h"

ApplicationClass Application;

ApplicationClass::ApplicationClass()
{
	_firstSceneLoop = true;
	_touched = false;
	_nextScene = NULL;
	_currentScene = NULL;
}

ApplicationClass::~ApplicationClass()
{

}


void ApplicationClass::handleTouches()
{
	//If we don't have a screen controller we don't have to handle touches
	if (_currentScene == NULL) return;

	//Get current scene controller
	SceneController* sceneController = _currentScene;

	//Touches infinite state machine
	if (Touch.touched())
	{
		//Get touch point and transform due to screen rotation
		TS_Point point = Touch.getPoint();
		swap(point.x,point.y);
		point.y = 240-point.y;

		if (_touched)
		{
			if (point.x != _lastTouchPoint.x || point.y != _lastTouchPoint.y)
			{
				LOG("Touch Moved");
				//Move event
				sceneController->handleTouchMoved(point,_lastTouchPoint);
			}
		}
		else
		{
			LOG("Touch down");
			//Touch down event
			sceneController->handleTouchDown(point);
			_touched = true;
		}

		_lastTouchPoint = point;
	}
	else
	{
		if (_touched)
		{
			LOG("Touch up");
			//Touch up event
			sceneController->handleTouchUp(_lastTouchPoint);
		}

		_touched = false;
	}
}

void ApplicationClass::loop()
{
	//Run Animations
	Animator.update();

	//Clear the display
	//Display.clear();

	if (_nextScene != NULL)
	{
		//Shut down display to hide the build process of the layout (which is step by step and looks flashy)
		digitalWrite(TFT_BACKLIGHT_PWM,LOW);

		//Clear the display
		Display.clear();

		if (_currentScene != NULL)
		{
			delete _currentScene;
		}

		_currentScene = _nextScene;
		_nextScene = NULL;
		_firstSceneLoop = true;
	}

	//Run current controller
	if (_currentScene != NULL)
	{
		//LOG("Run Controller");
		SceneController* sceneController = _currentScene;

		//Call onWillAppear event handler if this is the first time the loop function is called by the scene
		//The default implementation will clear the display!
		if (_firstSceneLoop)
		{
			LOG("First loop");
			Display.clear();

			LOG_VALUE("Appearing scene", sceneController->getName());
			sceneController->onWillAppear();
			LOG("Scene appeared");

/*			Display.fillRect(0,0,50,240,Application.getTheme()->getPrimaryColor());
			Display.setTextRotation(270);
			Display.setCursor(13,130);
			Display.setTextColor(ILI9341_WHITE);
			Display.setFont(PTSansNarrow_24);
			Display.print("PROJECTS");

			Display.setTextRotation(0);*/
		}

		//Touch handling
		handleTouches();

		//Run the scenes loop function
		sceneController->loop();

		//Relayout screen tiles
		Display.layoutIfNeeded();

		//Update display
		Display.dispatch();

		if (_firstSceneLoop)
		{
			//Set display brightness to full to show what's been built up since we shut down the display
			digitalWrite(TFT_BACKLIGHT_PWM,HIGH);
		}

		_firstSceneLoop = false;
	}

	//Delay for a few ms if no animation is running
	if (!Animator.hasActiveAnimations())
	{
		delay(16);
	}

}

void ApplicationClass::pushScene(SceneController *scene)
{
	LOG_VALUE("Pushing scene",scene->getName());

	_nextScene = scene;
}

ColorTheme* ApplicationClass::getTheme()
{
	return &_theme;
}
