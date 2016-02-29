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

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "Arduino.h"
#include "StackArray.h"
#include "SceneController.h"
#include "ColorTheme.h"
#include "Icon.h"
#include "PHDisplay.h"
#include "ILI9341_t3.h"
#include <Adafruit_FT6206.h>
#include "Animator.h"
#include "font_PT_Sans-Narrow-Web-Regular.h"

#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)
#define logError(msg) (__FILE__ " line " STRINGIZE(__LINE__) ": " msg)

#define LOG(m) Serial.println(m)
#define LOG_VALUE(m,v) Serial.print(m);Serial.print(logError(m));Serial.println(v);

class SceneController;
class View;

class ApplicationClass
{
public:
	ApplicationClass();
	~ApplicationClass();

	void loop();
	void pushScene(SceneController* scene);
	SceneController* currentScene() { return _currentScene; };

	void handleTouches();

	void sendScreenshot();

	ColorTheme* getTheme();

private:
	bool _firstSceneLoop;
	ColorTheme _theme;
	bool _touched;
	TS_Point _lastTouchPoint;
	SceneController *_nextScene;
	SceneController *_currentScene;
};

extern ApplicationClass Application;
extern PHDisplay Display;
extern Adafruit_FT6206 Touch;
extern int globalLayerId;

extern int globalLayersCreated;
extern int globalLayersDeleted;

#endif //_APPLICATION_H_
