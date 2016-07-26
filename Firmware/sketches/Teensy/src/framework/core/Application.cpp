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
#include "../animation/Animator.h"
#include "../../DownloadFileController.h"

ApplicationClass Application;

ApplicationClass::ApplicationClass()
{
	_firstSceneLoop = true;
	_touched = false;
	_nextScene = NULL;
	_currentScene = NULL;
	_lastTime = 0;
	_deltaTime = 0;
	_esp = new CommStack(&Serial3,this);
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
				//LOG("Touch Moved");
				//Move event
				sceneController->handleTouchMoved(point,_lastTouchPoint);
			}
		}
		else
		{
			//LOG("Touch down");
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
			//LOG("Touch up");
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

	//Process Communication with ESP
	_esp->process();

	if (_nextScene != NULL)
	{
		//Shut down display to hide the build process of the layout (which is step by step and looks flashy)
		Display.fadeOut();

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

			//Prepare display for this scene (i.e. setting scroll position and scroll offsets, etc)
			sceneController->setupDisplay();

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
		sceneController->loop();
		_lastTime = millis();

		bool willRefresh = Display.willRefresh();
		if (willRefresh)
		{
			//This should be a good idea as it marks MK20 to be unable to receive data, but this does not work at the moment
			digitalWrite(33,LOW);
		}

		//Relayout screen tiles
		Display.layoutIfNeeded();

		//Update display
		Display.dispatch();

		if (_firstSceneLoop)
		{
			//Set display brightness to full to show what's been built up since we shut down the display
			Display.fadeIn();
		}

		if (willRefresh)
		{
			//This should be a good idea as it marks MK20 to be unable to receive data, but this does not work at the moment
			digitalWrite(33,HIGH);
		}

		_firstSceneLoop = false;
	}

	//Delay for a few ms if no animation is running
	if (!Animator.hasActiveAnimations())
	{
		//delay(16);
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


void ApplicationClass::sendScreenshot()
{

}


float ApplicationClass::getDeltaTime()
{
	return _deltaTime;
}


CommStack *ApplicationClass::getESPStack()
{
	return _esp;
}

bool ApplicationClass::runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool* sendResponse, bool* success)
{
	if (_currentScene->handlesTask(header.getCurrentTask()))
	{
		LOG_VALUE("Current scene handles Task with ID",header.getCurrentTask());
		return _currentScene->runTask(header,data,dataSize,responseData,responseDataSize,sendResponse,success);
	}
	
	LOG_VALUE("Running Task with ID",header.getCurrentTask());
	LOG_VALUE("Comm-Type",header.commType);

	if (header.getCurrentTask() == GetTimeAndDate)
	{
		if (header.commType == ResponseSuccess)
		{
			LOG("Loading Date and Time from ESP");
			Display.setCursor(10,30);
			Display.println("Data available, reading...");

			char datetime[header.contentLength+1];
			memset(datetime,0,header.contentLength+1);
			memcpy(datetime,data,header.contentLength);

			LOG_VALUE("Received Datetime",datetime);

			Display.setCursor(10,50);
			Display.println("Received datetime from ESP");
			Display.println(datetime);
		}
	}
/*	else if (header.getCurrentTask() == GetJobWithID || header.getCurrentTask() == GetProjectItemWithID)
	{
		if (header.commType == Response)
		{
			//Wait for data to be arrived
			while (!stream->available())
			{
				delay(10);
			}

			//First we ask for the job id which is sent using println on the other side so we read until a newline char
			String jobID = stream->readStringUntil('\n');
			LOG_VALUE("Got Response for GetJobWithID",jobID);

			//Add file suffix to job
			jobID = jobID + ".gcode";

			//Open a file on SD card
			File file = SD.open("job.gcode",O_WRITE);
			if (!file.available())
			{
				//TODO: We should handle that. For now we will have to read data from ESP to clean the pipe but there should be better ways to handle errors
			}

			LOG("File opened for writing. Now waiting for number of bytes to read");

			//Wait for data to be arrived
			while (!stream->available())
			{
				delay(10);
			}

			//We asked for the job and now we got it, the next 4 bytes are the number of bytes sent next
			int32_t numberOfBytes = 0;
			int bytesRead = stream->readBytes((char*)&numberOfBytes,sizeof(int32_t));
			if (bytesRead == sizeof(int32_t))
			{
				LOG_VALUE("Expecting data of size",numberOfBytes);

				//We should have the correct number of bytes to read next
				int32_t currentByteIndex = 0;
				while (currentByteIndex < numberOfBytes)
				{
					if (stream->available())
					{
						int byteRead = stream->read();
						if (byteRead >= 0)
						{
							file.write(byteRead);
						}

						currentByteIndex++;

						if (currentByteIndex % 100 == 0)
						{
							float fraction = (float)currentByteIndex / (float)numberOfBytes;
							DownloadFileController* downloadFileScene = (DownloadFileController*)_currentScene;
							downloadFileScene->getProgressBar()->setValue(fraction);

							LOG_VALUE("Download-Progress",fraction);

							//Update the display - we need to do that here as wo don't run the application loop which does call this method each "frame"
							Display.dispatch();
						}
					}
				}

				LOG("Finished downloading file");
				LOG_VALUE("Download bytes",currentByteIndex);
				LOG_VALUE("Expected number of bytes",numberOfBytes);

				file.close();
			}
		}
	}*/
	return true;
}


