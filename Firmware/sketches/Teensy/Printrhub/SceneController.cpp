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

}

SceneController::~SceneController()
{

}

void SceneController::setup()
{

}

void SceneController::loop()
{
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