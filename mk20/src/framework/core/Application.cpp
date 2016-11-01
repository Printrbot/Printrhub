//
// Created by Phillip Schuster on 09.07.16.
//

#include "Application.h"
#include "SceneController.h"
#include "../animation/Animator.h"
#include "../../scenes/DownloadFileController.h"
#include "../../scenes/alerts/ErrorScene.h"
#include "../../scenes/projects/ProjectsScene.h"
#include "../../scenes/firmware/ConfirmFirmwareUpdateScene.h"
#include "../../scenes/firmware/FirmwareInProgressScene.h"
#include "../../scenes/settings/SystemInfoScene.h"
#include "Printr.h"
#include <ArduinoJson.h>
#include "../../errors.h"
#include "../../jobs/ReceiveSDCardFile.h"
#include "EventLogger.h"

ApplicationClass Application;

extern Printr printr;

ApplicationClass::ApplicationClass()
{
	_firstSceneLoop = true;
	_touched = false;
	_nextScene = NULL;
	_currentScene = NULL;
	_lastTime = 0;
	_deltaTime = 0;
  _buildNumber = FIRMWARE_BUILDNR;
	_esp = new CommStack(&Serial3,this);
  _espOK = false;
  _lastESPPing = 0;
  _currentJob = NULL;
  _nextJob = NULL;
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

void ApplicationClass::setup()
{
#ifdef TEENSYDUINO
  FLOW_ALWAYS("Using Teensyduino %d",(int)TEENSYDUINO);
#endif

	//Configure LED pin
	pinMode(LED_PIN, OUTPUT);

  pinMode(PRINTER_ACTIVE,OUTPUT);
  digitalWrite(PRINTER_ACTIVE,HIGH);

  printr.init();

  pinMode(CODE_INDICATOR_1, OUTPUT);
  digitalWrite(CODE_INDICATOR_1, HIGH);

  pinMode(CODE_INDICATOR_2, OUTPUT);
  digitalWrite(CODE_INDICATOR_2, HIGH);

  //Make sure we have a jobs folder
  //TODO: Decide if this is necessary or if the SD card is setup with this path during production
  if (!SD.exists("/jobs"))
  {
      SD.mkdir("/jobs");
  }
}

void ApplicationClass::pingESP()
{
  //Send ping with current version to ESP
  int version = FIRMWARE_BUILDNR;
  _esp->requestTask(TaskID::Ping,sizeof(int),(uint8_t*)&version);
}

void ApplicationClass::resetESP()
{
  pinMode(ESP_RESET, OUTPUT);
  digitalWrite(ESP_RESET, LOW);
  delay(100);
  digitalWrite(ESP_RESET, HIGH);
  pinMode(ESP_RESET, INPUT);
}

void ApplicationClass::loop()
{
  //Peridically send ping to ESP
  if (!_espOK) {
    if ((millis() - _lastESPPing) > 5000) {
      pingESP();
      _lastESPPing = millis();
    }
  }

	//Process Communication with ESP
	_esp->process();

	//run the loop on printr
  _esp->beginBlockPort();
	printr.loop();
  _esp->endBlockPort();

	//Run Animations
	Animator.update();

	StatusLED.loop();

	//Clear the display
	//Display.clear();

  //Handling background jobs
  if (_nextJob != NULL)
  {
    if (_currentJob != NULL) {
      FLOW_NOTICE("Replacing job %s",_currentJob->getName().c_str());
      //Send terminating handler
      _esp->beginBlockPort();
      _currentJob->onWillEnd();
      delete _currentJob;
      _esp->endBlockPort();
    }

    FLOW_NOTICE("Starting job %s",_nextJob->getName().c_str());
    _currentJob = _nextJob;
    _nextJob = NULL;

    //Send will start event
    _esp->beginBlockPort();
    _currentJob->onWillStart();
    _esp->endBlockPort();
    FLOW_NOTICE("OnWillStart called on job %s",_currentJob->getName().c_str());
  }

  if (_currentJob != NULL) {
    _esp->beginBlockPort();
    _currentJob->loop();
    _esp->endBlockPort();

    if (_currentJob->isFinished()) {
      FLOW_NOTICE("Exiting job %s",_currentJob->getName().c_str());
      _esp->beginBlockPort();
      _currentJob->onWillEnd();
      delete _currentJob;
      _currentJob = NULL;
      _esp->endBlockPort();
    }
  }

  //UI Handling
	if (_nextScene != NULL)
	{
    _esp->beginBlockPort();

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

    _esp->endBlockPort();
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
      _esp->beginBlockPort();

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
      _esp->endBlockPort();
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
    _esp->beginBlockPort();
		sceneController->loop();
    _esp->endBlockPort();
		_lastTime = millis();

		bool willRefresh = Display.willRefresh();
		if (willRefresh)
		{
			//This should be a good idea as it marks MK20 to be unable to receive data, but this does not work at the moment
      _esp->beginBlockPort();
		}

		//Relayout screen tiles
		Display.layoutIfNeeded();

    if (_firstSceneLoop)
    {
      //Inform scene controller that it's visible now
      _currentScene->onDidAppear();
    }

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
			_esp->endBlockPort();
		}

		_firstSceneLoop = false;
	}

	//Delay for a few ms if no animation is running
	if (!Animator.hasActiveAnimations())
	{
		//delay(16);
	}

}

void ApplicationClass::pushScene(SceneController *scene, bool cancelModal)
{
  if (_currentScene != NULL && _currentScene->isModal() && cancelModal == false)
  {
    //Don't push this scene as the current screen is modal and should not be canceled
    return;
  }

	LOG_VALUE("Pushing scene",scene->getName());

	_nextScene = scene;
}

void ApplicationClass::pushJob(BackgroundJob *job) {
  FLOW_NOTICE("Pushed job %s",job->getName().c_str());
  _nextJob = job;
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

void ApplicationClass::onCommStackError()
{
	StatusLED.pulse(0.5,false);
}

bool ApplicationClass::runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool* sendResponse, bool* success)
{
  COMMSTACK_SPAM("Application received task with ID %d",header.getCurrentTask());

	if (_currentScene != NULL && _currentScene->handlesTask(header.getCurrentTask()))
	{
    COMMSTACK_SPAM("Sending task with ID %d to scene %s",header.getCurrentTask(),_currentScene->getName().c_str());
		return _currentScene->runTask(header,data,dataSize,responseData,responseDataSize,sendResponse,success);
	}

  if (_currentJob != NULL && _currentJob->handlesTask(header.getCurrentTask())) {
    COMMSTACK_SPAM("Sending task with ID %d to job %s",header.getCurrentTask(),_currentJob->getName().c_str());
    return _currentJob->runTask(header, data, dataSize, responseData, responseDataSize, sendResponse, success);
  }

  COMMSTACK_NOTICE("Application handles task with ID %d",header.getCurrentTask());

	if (header.getCurrentTask() == TaskID::SaveProjectWithID) {
    if (header.commType == Request) {

      StaticJsonBuffer<500> jsonBuffer;
      String jsonObject((const char *) data);
      JsonObject &root = jsonBuffer.parseObject(jsonObject);

      if (root.success()) {
        String url = root["url"];
        if (url.length() > 0) {

          // check if we can run download now
          if (_currentScene->isModal()) {
             LOG("Hub is busy can can not start download");
          } else {
            String idx = root["id"].asString();
            DownloadFileController *dfc = new DownloadFileController(url, idx);
            Application.pushScene(dfc);
          }
        }
      } else {
        LOG("Could not parse SaveProjectWithID data package from JSON");
      }

      //Do not send a response as we will trigger a "mode" change on ESP in the next request
      *sendResponse = false;
    }
  } else if (header.getCurrentTask() == TaskID::DownloadError) {
    if (header.commType == Request) {

      //Cast data into local error code variable
      uint8_t error = *data;
      DownloadError errorCode = (DownloadError) error;

      if (errorCode == DownloadError::Timeout) {
        Application.pushScene(new ErrorScene("Timeout"),true);
      } else if (errorCode == DownloadError::InternalServerError) {
        Application.pushScene(new ErrorScene("Internal Server Error"),true);
      } else if (errorCode == DownloadError::FileNotFound) {
        Application.pushScene(new ErrorScene("File not found"),true);
      } else if (errorCode == DownloadError::Forbidden) {
        Application.pushScene(new ErrorScene("Forbidden"),true);
      } else if (errorCode == DownloadError::UnknownError) {
        Application.pushScene(new ErrorScene("Unknown Error"),true);
      } else if (errorCode == DownloadError::ConnectionFailed) {
        Application.pushScene(new ErrorScene("Connection failed"),true);
      } else if (errorCode == DownloadError::PrepareDownloadedFileFailed) {
        Application.pushScene(new ErrorScene("File preparation failed"),true);
      } else if (errorCode == DownloadError::RemoveOldFilesFailed) {
        Application.pushScene(new ErrorScene("Remove old file failed"),true);
      }

      *sendResponse = false;
    }
  } else if (header.getCurrentTask() == TaskID::FirmwareUpdateError) {
    if (header.commType == Request) {

      //Cast data into local error code variable
      uint8_t error = *data;
      FirmwareUpdateError errorCode = (FirmwareUpdateError) error;

      if (errorCode == FirmwareUpdateError::UnknownError) {
        Application.pushScene(new ErrorScene("Unknown Error"));
      }

      *sendResponse = false;
    }
  } else if (header.getCurrentTask() == TaskID::GetTimeAndDate) {
    if (header.commType == ResponseSuccess) {
      LOG("Loading Date and Time from ESP");
      Display.setCursor(10, 30);
      Display.println("Data available, reading...");

      char datetime[header.contentLength + 1];
      memset(datetime, 0, header.contentLength + 1);
      memcpy(datetime, data, header.contentLength);

      LOG_VALUE("Received Datetime", datetime);

      Display.setCursor(10, 50);
      Display.println("Received datetime from ESP");
      Display.println(datetime);
    }
  } else if (header.getCurrentTask() == TaskID::StartFirmwareUpdate) {
    //We ask ESP therefore we get the response
    if (header.commType == ResponseSuccess) {
      ErrorScene *scene = new ErrorScene("Updating Firmware", false);
      Application.pushScene(scene);
    }
  } else if (header.getCurrentTask() == TaskID::Ping) {
    if (header.commType == ResponseSuccess) {
      //We have received the response from ESP on our ping - do nothing
      int buildNumber = 0;
      memcpy(&buildNumber, data, dataSize);

      //Stop sending pings
      _espOK = true;

      //Communication with ESP established, show project scene
      ProjectsScene *mainScene = new ProjectsScene();
      Application.pushScene(mainScene);
    } else if (header.commType == Request) {
      //Read build number from MK20 firmware
      int buildNumber = 0;
      memcpy(&buildNumber, data, sizeof(int));

      //Stop sending pings to MK20
      _espOK = true;

      //Send ESP build number in response
      buildNumber = FIRMWARE_BUILDNR;
      *sendResponse = true;
      *responseDataSize = sizeof(int);
      memcpy(responseData, &buildNumber, sizeof(int));
    }
  } else if (header.getCurrentTask() == TaskID::ShowFirmwareUpdateNotification) {
    if (header.commType == Request) {
      COMMSTACK_NOTICE("Received ShowFirmwareUpdateNotification request");
      *sendResponse = false;

      ConfirmFirmwareUpdateScene *scene = new ConfirmFirmwareUpdateScene();
      Application.pushScene(scene);
    }
  } else if (header.getCurrentTask() == TaskID::DebugLog) {
    *sendResponse = false;
  } else if (header.getCurrentTask() == TaskID::RestartESP) {
    *sendResponse = false;
    COMMSTACK_NOTICE("Received RestartESP request, restarting ESP");

    //Restart ESP
    resetESP();
  } else if (header.getCurrentTask() == TaskID::FirmwareUpdateComplete) {
    //Don't send response as we restart ESP
    *sendResponse = false;

    //Restart ESP
    resetESP();

    //Show Project scene
    ProjectsScene *scene = new ProjectsScene();
    pushScene(scene, true);
  } else if (header.getCurrentTask() == TaskID::ShowFirmwareUpdateInProgress) {
    if (header.commType == Request) {
      FirmwareInProgressScene* scene = new FirmwareInProgressScene();
      pushScene(scene,true);
    }
  } else if (header.getCurrentTask() == TaskID::FileOpenForWrite) {
    if (header.commType == Request) {

      StaticJsonBuffer<200> jsonBuffer;
      String jsonObject((const char *) data);
      JsonObject &root = jsonBuffer.parseObject(jsonObject);

      if (root.success()) {
        String localFilePath = root["localFilePath"];
        if (localFilePath.length() > 0) {
          COMMSTACK_NOTICE("Received FileOpenForWrite request with local file path: %s",localFilePath.c_str());

          *responseDataSize = 0;
          *sendResponse = true;
          *success = true;

          size_t fileSize = root["fileSize"];
          Compression compression = (Compression)(uint8_t)root["compression"];

          ReceiveSDCardFile *job = new ReceiveSDCardFile(localFilePath, fileSize, compression);
          pushJob(job);
        } else {
          COMMSTACK_ERROR("Could not handle FileOpenForWrite as local file path is empty");

          *responseDataSize = 0;
          *sendResponse = true;
          *success = false;
        }
      } else {
        COMMSTACK_ERROR("Could not handle FileOpenForWrite as JSON could not be parsed");
        *sendResponse = true;
        *success = false;
      }
    }
  } else if (header.getCurrentTask() == TaskID::ShowWiFiInfo) {
    if (header.commType == Request) {
      SystemInfoScene *scene = new SystemInfoScene();
      pushScene(scene, true);

      *sendResponse = false;
    }
  } else if (header.getCurrentTask() == TaskID::SetPassword) {
    if (header.commType == ResponseSuccess) {
      FLOW_NOTICE("Setting/Clearing password successful");
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
