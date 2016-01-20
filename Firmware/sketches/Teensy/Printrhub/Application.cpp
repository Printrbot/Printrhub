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
	_focusedView = NULL;
}

ApplicationClass::~ApplicationClass()
{

}

void ApplicationClass::loop()
{
	//Run Animations
	Animator.update();

	//Clear the display
	//Display.clear();

	//Run current controller
	if (_scenes.count() > 0)
	{
		SceneController* sceneController = _scenes.peek();

		//Call onWillAppear event handler if this is the first time the loop function is called by the scene
		//The default implementation will clear the display!
		if (_firstSceneLoop)
		{
			sceneController->onWillAppear();
			_firstSceneLoop = false;
		}

		//Run the scenes loop function
		sceneController->loop();
	}

	//Relayout screen tiles
	Display.layoutIfNeeded();

	//Update display
	Display.dispatch();
	//sendScreenshot();

	Serial.print("Layers created: ");
	Serial.print(::globalLayersCreated);
	Serial.print(", deleted: ");
	Serial.println(::globalLayersDeleted);

}

void ApplicationClass::pushScene(SceneController *scene)
{
	LOG_VALUE("Pushing scene",scene->getName());

	_firstSceneLoop = true;
	_scenes.push(scene);
}

void ApplicationClass::dismissScene()
{
	if (_scenes.count() <= 1) return;
	LOG_VALUE("Dismissing scene", _scenes.peek()->getName());

	SceneController* sceneController = _scenes.pop();
	sceneController->onWillDisappear();

	_firstSceneLoop = true;
}

void ApplicationClass::setFocusedView(View *focusView)
{
	LOG_VALUE("Setting focus to view",focusView->getDescription());
	_focusedView = focusView;
}

View* ApplicationClass::getFocusedView()
{
	return _focusedView;
}

void ApplicationClass::resetFocus()
{
	_focusedView = NULL;
}

ColorTheme* ApplicationClass::getTheme()
{
	return &_theme;
}