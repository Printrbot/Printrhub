#include "application.h"
#include "core/Mode.h"
#include "hal.h"
#include "config.h"
#include "controllers/ManageWifi.h"
#include "controllers/DownloadFile.h"
#include "controllers/DownloadFileToSDCard.h"
#include <EEPROM.h>
#include "event_logger.h"

Config config;
ApplicationClass Application;

ApplicationClass::ApplicationClass() {
	_firstModeLoop = true;
	_nextMode = NULL;
	_currentMode = NULL;
	_lastTime = 0;
	_deltaTime = 0;
	_buttonPressedTime = 0;
	_mk20 = new CommStack(&Serial,this);
}

ApplicationClass::~ApplicationClass() {
	delete _mk20;
	_mk20 = NULL;
}

void ApplicationClass::setup() {

	pinMode(MK20_RESET_PIN,OUTPUT);
	digitalWrite(MK20_RESET_PIN,HIGH);

	pinMode(COMMSTACK_WORKING_MARKER_PIN, OUTPUT);
	digitalWrite(COMMSTACK_WORKING_MARKER_PIN, HIGH);

    pinMode(COMMSTACK_INFO_MARKER_PIN, OUTPUT);
    digitalWrite(COMMSTACK_INFO_MARKER_PIN, HIGH);

	Serial.begin(COMMSTACK_BAUDRATE);
	SPIFFS.begin();

	Mode* manageWifi = new ManageWifi();
	Application.pushMode(manageWifi);

	config.load();

	// request info from mk20
	//getMK20Stack()->requestTask(SystemInfo);

	//connectWiFi();
 //	_server.begin();
}

void ApplicationClass::pushMode(Mode *mode) {
	LOG_VALUE("Pushing mode",mode->getName());
	_nextMode = mode;
}

float ApplicationClass::getDeltaTime() {
	return _deltaTime;
}

void ApplicationClass::loop()
{
	_mk20->process();

	//Setup new controller and delete the old one if a new controller is pushed
	if (_nextMode != NULL) {
		if (_currentMode != NULL) {
			delete _currentMode;
		}
		_currentMode = _nextMode;
		_nextMode = NULL;
		_firstModeLoop = true;
	}

	if (_currentMode != NULL) {
		Mode* mode = _currentMode;
		//Call onWillAppear event handler if this is the first time the loop function is called by the scene
		if (_firstModeLoop) {
			mode->onWillStart();
		}
		//Calculate Delta Time
		unsigned long currentTime = millis();
		if (currentTime < _lastTime) {
			//millis overflowed, just set delta to 0
			_deltaTime = 0;
		} else {
			_deltaTime = (float)(currentTime - _lastTime)/1000.0f;
		}
		//Run the scenes loop function
		mode->loop();
		_lastTime = millis();
		_firstModeLoop = false;
	}
}

bool ApplicationClass::runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool* sendResponse, bool* success)
{
	// skip all data writing tasks
	if (header.getCurrentTask() != TaskID::FileSaveData) {
		EventLogger::log("!New TASK!");
		char i[3];
		sprintf(i, "%d", header.getCurrentTask());
		EventLogger::log(i);
	}
	/*
	EventLogger::log("Current mode is:");
	char c[_currentMode->getName().length()+1];
	_currentMode->getName().toCharArray(c, sizeof(c), 0);
	EventLogger::log(c);
*/

	if (_currentMode->handlesTask(header.getCurrentTask())) {
		//EventLogger::log("RUNNIGN CURRENT TASK");
		return _currentMode->runTask(header,data,dataSize,responseData,responseDataSize,sendResponse,success);
	} else {
		EventLogger::log("CURRENT MODE DOES NOT HANDLE THIS TASK");
		switch(header.getCurrentTask()) {

            case TaskID::DownloadFile:
			case TaskID::GetProjectWithID:
			case TaskID::GetJobWithID: {
				EventLogger::log("GET PROJECT WITH ID TASK");
				char _url[header.contentLength+1];
				memset(_url,0,header.contentLength+1);
				memcpy(_url,data,header.contentLength);
				//Send the response later when we know how large the file is
				*sendResponse = false;
				//Initiate mode for file download
				EventLogger::log(_url);
                DownloadFileToSDCard* df = new DownloadFileToSDCard(String(_url));
				Application.pushMode(df);
				}
				break;

			case TaskID::StartWifi:
			case TaskID::StopWifi: {
				EventLogger::log("Wifi TASK");
				Mode* mw = new ManageWifi();
				Application.pushMode(mw);
				}
				break;

			case TaskID::SystemInfo:
				EventLogger::log("GOT SYSTEM INFO");
				break;

			case TaskID::RunPrinterGCode: {
				EventLogger::log("GOT REPLY ON GCODE RUN");
				// do not need to create new mode for this...
				}
				break;

			default: {
				*responseDataSize = 0;
				*sendResponse = false;
				EventLogger::log("UNKNOWN TASK");
				}
				break;
		}

		//return true;
	}

	return true;
}
