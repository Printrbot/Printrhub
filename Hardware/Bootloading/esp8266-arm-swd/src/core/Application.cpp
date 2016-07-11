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
#include "Mode.h"
#include <FS.h>
#include "../MK20FirmwareUpdate.h"

ApplicationClass Application;

ApplicationClass::ApplicationClass()
{
	_firstModeLoop = true;
	_nextMode = NULL;
	_currentMode = NULL;
	_lastTime = 0;
	_deltaTime = 0;
	_buttonPressedTime = 0;
	_mk20 = new CommStack(&Serial,this);
}

ApplicationClass::~ApplicationClass()
{
	delete _mk20;
	_mk20 = NULL;
}

void ApplicationClass::setup()
{
	Serial.begin(111520);
	SPIFFS.begin();

	//wiFiManager.autoConnect("Printrbot");
}

void ApplicationClass::loop()
{
	//Check MK20 communication by updating the comm stack. This may result in a call to
	//the runTask member function which in turn most likely sets a new current mode controller
	_mk20->process();

	//Setup new controller and delete the old one if a new controller is pushed
	if (_nextMode != NULL)
	{
		if (_currentMode != NULL)
		{
			delete _currentMode;
		}

		_currentMode = _nextMode;
		_nextMode = NULL;
		_firstModeLoop = true;
	}

	//Run current controller
	if (_currentMode != NULL)
	{
		//LOG("Run Controller");
		Mode* mode = _currentMode;

		//Call onWillAppear event handler if this is the first time the loop function is called by the scene
		//The default implementation will clear the display!
		if (_firstModeLoop)
		{
			LOG("First loop");

			LOG_VALUE("Appearing mode", mode->getName());
			mode->onWillStart();
			LOG("Mode started");
		}

		//Calculate Delta Time
		unsigned long currentTime = millis();
		if (currentTime < _lastTime)
		{
			//millis overflowed, just set delta to 0
			_deltaTime = 0;
		}
		else
		{
			_deltaTime = (float)(currentTime - _lastTime)/1000.0f;
		}

		//Run the scenes loop function
		mode->loop();
		_lastTime = millis();

		_firstModeLoop = false;
	}

	//Special mode - initiate firmware update by button pressed
	if (digitalRead(0) == LOW)
	{
		if (_buttonPressedTime == 0)
		{
			_buttonPressedTime = millis();
		}
		else
		{
			if ((millis() - _buttonPressedTime) > 2000)
			{
				if (digitalRead(0) == LOW)
				{
					//Button pressed for two seconds
					_buttonPressedTime = 0;

					Mode* mode = new MK20FirmwareUpdate();
					Application.pushMode(mode);
					return;
				}
			}
		}
	}
	else if (_buttonPressedTime > 0)
	{
		_buttonPressedTime = 0;
	}
}

void ApplicationClass::pushMode(Mode *mode)
{
	LOG_VALUE("Pushing mode",mode->getName());

	_nextMode = mode;
}

float ApplicationClass::getDeltaTime()
{
	return _deltaTime;
}

bool ApplicationClass::runTask(CommHeader &header, Stream *stream)
{
	LOG_VALUE("Running Task with ID",header.getCurrentTask());
	LOG_VALUE("Comm-Type",header.commType);
	if (header.getCurrentTask() == GetTimeAndDate)
	{
		if (header.commType == Request)
		{
			LOG("Date and Time written zu Stream");
			stream->println("2016-07-11 19:03:00 CEST");
		}
		else
		{
			LOG("Don't know what to do with a GetTimeAndDate Response");
		}
	}

	return true;
}