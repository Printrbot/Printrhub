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

#ifndef __SCENECONTROLLER_H_
#define __SCENECONTROLLER_H_

#include "View.h"
#include "Application.h"

class SceneController
{
public:
	SceneController();
	virtual ~SceneController();

	virtual void setup();
	virtual void loop();

	virtual String getName() = 0;

	//Events
	virtual void onWillAppear();
	virtual void onWillDisappear();

	bool statusBarVisible();

	//Display
	virtual void display() = 0;
};


#endif //__SCENECONTROLLER_H_
