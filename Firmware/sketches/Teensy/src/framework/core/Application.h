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
#include "../views/Icon.h"
#include "PHDisplay.h"
#include "../../drivers/Display/ILI9341_t3.h"
#include "../../drivers/Adafruit_FT6206/Adafruit_FT6206.h"
#include "../animation/Animator.h"
#include "../../fonts/font_PT_Sans-Narrow-Web-Regular.h"
#include "CommStack.h"

#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)
#define logError(msg) (__FILE__ " line " STRINGIZE(__LINE__) ": " msg ": ")
#define logString(msg) (__FILE__ " line " STRINGIZE(__LINE__) ": " msg)

#define LOG(m) //Serial.println(logString(m));Serial.flush();
#define LOG_VALUE(m,v) //Serial.print(logError(m));Serial.println(v);Serial.flush();
#define LOG_NORMAL(m) Serial.println(logString(m));Serial.flush();
#define LOG_NORMAL_VALUE(m,v) Serial.print(logError(m));Serial.println(v);Serial.flush();

#define TFT_BACKLIGHT_PWM 22
#define LED_PIN 21

class SceneController;
class View;

class ApplicationClass : public CommStackDelegate
{
#pragma mark Constructor
public:
	ApplicationClass();
	~ApplicationClass();

#pragma mark Application Flow
	void loop();
	void pushScene(SceneController* scene);
	SceneController* currentScene() { return _currentScene; };

#pragma mark Touch Handling
	void handleTouches();

#pragma mark Color Theme
	ColorTheme* getTheme();

#pragma mark CommStackDelegate
public:
	CommStack* getESPStack();
	bool runTask(CommHeader& header, const uint8_t* data, size_t dataSize, uint8_t* responseData, uint16_t* responseDataSize, bool* sendResponse, bool* success);

#pragma mark Misc
	void sendScreenshot();

#pragma mark Time Management
	float getDeltaTime();

#pragma mark Member Variables
private:
	bool _firstSceneLoop;
	ColorTheme _theme;
	bool _touched;
	TS_Point _lastTouchPoint;
	SceneController *_nextScene;
	SceneController *_currentScene;
	unsigned long _lastTime;
	float _deltaTime;
	CommStack* _esp;
};

extern ApplicationClass Application;
extern PHDisplay Display;
extern Adafruit_FT6206 Touch;
extern int globalLayerId;
extern int globalLayersCreated;
extern int globalLayersDeleted;

#endif //_APPLICATION_H_
