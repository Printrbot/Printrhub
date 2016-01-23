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
#include <ILI9341_t3.h>
#include <Adafruit_FT6206.h>
#include <font_Arial.h>
#include "Animator.h"

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
	void dismissScene();

	void setFocusedView(View* focusView);
	View* getFocusedView();
	void resetFocus();

	void handleTouches();

	void sendScreenshot();

	ColorTheme* getTheme();

private:
	StackArray<SceneController*> _scenes;
	bool _firstSceneLoop;
	View* _focusedView;
	ColorTheme _theme;
	bool _touched;
	TS_Point _lastTouchPoint;
};

extern ApplicationClass Application;
extern PHDisplay Display;
extern Adafruit_FT6206 Touch;
extern int globalLayerId;

extern int globalLayersCreated;
extern int globalLayersDeleted;

#endif //_APPLICATION_H_
