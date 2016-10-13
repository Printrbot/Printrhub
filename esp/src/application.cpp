#include "application.h"
#include "core/Mode.h"
#include "hal.h"
#include "config.h"
#include "controllers/ManageWifi.h"
#include "controllers/DownloadFile.h"
#include "controllers/DownloadFileToSDCard.h"
#include "controllers/PushFileToSDCard.h"
#include <EEPROM.h>
#include <controllers/ESPFirmwareUpdate.h>
#include <controllers/MK20FirmwareUpdate.h>
#include "event_logger.h"
#include "controllers/CheckForFirmwareUpdates.h"
#include "controllers/DownloadFileToSPIFFs.h"
#include "controllers/Idle.h"
#include "controllers/HandleDownloadError.h"

Config config;
ApplicationClass Application;

ApplicationClass::ApplicationClass() {
	_firstModeLoop = true;
	_nextMode = NULL;
	_currentMode = NULL;
	_lastTime = 0;
	_deltaTime = 0;
	_buttonPressedTime = 0;
	_mk20 = new MK20(&Serial,this);
    _buildNumber = FIRMWARE_BUILDNR;
	_firmwareUpdateInfo = NULL;
    _firmwareChecked = false;
    _lastMK20Ping = 0;
    _mk20OK = false;
}

ApplicationClass::~ApplicationClass() {
	delete _mk20;
	_mk20 = NULL;
}

void ApplicationClass::setup() {

    //Start with MK20 timeout at current time
    _appStartTime = millis();

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

    EventLogger::log("Waiting for MK20 ping");
}

void ApplicationClass::pushMode(Mode *mode) {

	EventLogger::log("Pushing mode %s",mode->getName().c_str());
	_nextMode = mode;
}

float ApplicationClass::getDeltaTime() {
	return _deltaTime;
}

void ApplicationClass::pingMK20() {
    //Send ping with current version to ESP
    int version = FIRMWARE_BUILDNR;
    _mk20->requestTask(TaskID::Ping,sizeof(int),(uint8_t*)&version);
}

void ApplicationClass::reset()
{
    //Send Restart message to MK20 which will pull ESP_RESET low for a hard reset
    _mk20->requestTask(TaskID::RestartESP);
}

void ApplicationClass::sendPulse(int length, int count)
{
    float d = (float)length/(float)count;
    for (int i=0;i<count;i++)
    {
        digitalWrite(COMMSTACK_INFO_MARKER_PIN,LOW);
        delay(d/2);
        digitalWrite(COMMSTACK_INFO_MARKER_PIN,HIGH);
        delay(d/2);
    }
}

void ApplicationClass::loop()
{
    //Process CommStack, this also senses CommStack state of the other side by reading its data flow pin
	_mk20->process();

    //Check if CommStack on other side has been initialized
    if (!_mk20->isReady())
    {
        //If MK20 did not raise CommStack pin to HIGH after 20 seconds we have to apply a firmware
        if ((millis() - _appStartTime) > 20000)
        {
            //TODO: Find a better way of doing that, this is triggered too often unintentially
            /*
            //If we have infos about the current MK20 firmware version
            if (_firmwareUpdateInfo != NULL) {

                //Only do that once
                if (!_firmwareChecked) {

                    //We have a bricked MK20 a WiFi connection and latest firmware infos, download firmware and flash MK20
                    _firmwareChecked = true;
                    String mk20FirmwareFile("/mk20_100.bin");

                    //Prepare modes for subsequent execution
                    MK20FirmwareUpdate* mk20UpdateFirmware = new MK20FirmwareUpdate(mk20FirmwareFile);
                    DownloadFileToSPIFFs* downloadMK20Firmware = new DownloadFileToSPIFFs(_firmwareUpdateInfo->mk20_url,mk20FirmwareFile);
                    downloadMK20Firmware->setNextMode(mk20UpdateFirmware);

                    //Start firmware update of MK20
                    Application.pushMode(downloadMK20Firmware);
                }
            }*/
        }
    }
    else
    {
        //CommStack on other side is ready, ping it to establish connection
        if (!_mk20OK) {
            if ((millis() - _lastMK20Ping) > 5000) {
                pingMK20();
                _lastMK20Ping = millis();
            }
        }
    }

    //Check if we have received an MK20 version request
/*    if (!_mk20->isAlive()) {
        if (_mk20->getNumTries() < 2 &&_mk20->isTimedOut()) {
            EventLogger::log("MK20 timed out, reseting MK20 now");
            //We did not receive a signal from MK20
            if (_mk20->getNumTries() <= 0) {
                //MK20 did not send a ping for 5 seconds, reset it and try again
                //_mk20->reset();
                _mk20->incrementNumTries();
            }
            else {
                EventLogger::log("MK20 timed out again, seems to be bricked, flashing firmware");
                //MK20 did not send a ping for 10 seconds with being reset in between. We now consider MK20 to be bricked
                //_mk20->updateFirmware();
                _mk20->incrementNumTries();
            }
        }
    } else {
        if (_mk20->needsUpdate()) {
            EventLogger::log("MK20 is alive but needs firmware update. Updating firmware now.");
            //MK20 firmware is older than ESP, trigger firmware update
            _mk20->showUpdateFirmwareNotification();
            //_mk20->updateFirmware();
        } else {
            if (!_firmwareChecked) {
                EventLogger::log("MK20 is alive and ready, checking for firmware updates");
                //OK, MK20 is listening and has the correct firmware installed, check for new firmware
                _firmwareChecked = true;
                CheckForFirmwareUpdates* updateCheck = new CheckForFirmwareUpdates();
                Application.pushMode(updateCheck);
            }
		}
    }*/

    if (_mk20OK)
    {
        if (!_firmwareChecked && _firmwareUpdateInfo != NULL) {
            _firmwareChecked = true;
            EventLogger::log("MK20 is ready, firmware is loaded.");

            if (_firmwareUpdateInfo->buildnr > FIRMWARE_BUILDNR) {
                EventLogger::log("New firmware available, asking for update");
                _mk20->showUpdateFirmwareNotification();
            }
        }
    }

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

void ApplicationClass::idle()
{
    Idle* idle = new Idle();
    pushMode(idle);
}

void ApplicationClass::handleError(DownloadError error)
{
    idle();
}


void ApplicationClass::startFirmwareUpdate()
{
    EventLogger::log("Updating firmware to build number %d with these files:",_firmwareUpdateInfo->buildnr);
    EventLogger::log("UI: %s",_firmwareUpdateInfo->mk20_ui_url.c_str());
    EventLogger::log("MK20 Firmware: %s",_firmwareUpdateInfo->mk20_url.c_str());
    EventLogger::log("ESP Firmware: %s",_firmwareUpdateInfo->esp_url.c_str());

    Mode* firstMode = NULL;

    //Define file names
    String mk20FirmwareFile("/mk20_100.bin");
    String mk20UIFile("/ui.min");

    //Define modes
    DownloadFileToSPIFFs* downloadMK20Firmware = new DownloadFileToSPIFFs(_firmwareUpdateInfo->mk20_url,mk20FirmwareFile);
    MK20FirmwareUpdate* mk20UpdateFirmware = new MK20FirmwareUpdate(mk20FirmwareFile);
    DownloadFileToSPIFFs* downloadUI = new DownloadFileToSPIFFs(_firmwareUpdateInfo->mk20_ui_url,mk20UIFile);
    PushFileToSDCard* pushUIToSDCard = new PushFileToSDCard(mk20UIFile,mk20UIFile,false,Compression::RLE16);
    ESPFirmwareUpdate* espUpdateFirmware = new ESPFirmwareUpdate(_firmwareUpdateInfo->esp_url);

    //Firmware update is composed of a few basic steps
    //1) Download ui file from server and push to SD card (only if MK20 is alive)
    if (_mk20OK) {
        EventLogger::log("Skipping UI update as MK20 is not alive");
        firstMode = downloadUI;
        downloadUI->setNextMode(pushUIToSDCard);
        pushUIToSDCard->setNextMode(downloadMK20Firmware);
    }

    //2) Download MK20 firmware and flash MK20
    downloadMK20Firmware->setNextMode(mk20UpdateFirmware);

    //3) Download ESP firmware and update ESP
    mk20UpdateFirmware->setNextMode(espUpdateFirmware);

    //Start with MK20 firmware update if no steps before have been defined
    if (firstMode == NULL) {
        firstMode = downloadMK20Firmware;
    }

    //Let's get started
    Application.pushMode(firstMode);
}

bool ApplicationClass::runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool* sendResponse, bool* success)
{
	if (_currentMode != NULL && _currentMode->handlesTask(header.getCurrentTask())) {
		//EventLogger::log("RUNNIGN CURRENT TASK");
		return _currentMode->runTask(header,data,dataSize,responseData,responseDataSize,sendResponse,success);
	}

    TaskID taskID = header.getCurrentTask();
	if (taskID == TaskID::DownloadFile || taskID == TaskID::GetProjectItemWithID || taskID == TaskID::GetJobWithID)
    {
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
    else if (taskID == TaskID::Ping)
    {
        if (header.commType == Request) {
            EventLogger::log("Received MK20 Ping");
            //Read build number from MK20 firmware
            int buildNumber = 0;
            memcpy(&buildNumber,data,sizeof(int));
            _mk20->setBuildNumber(buildNumber);
            EventLogger::log("MK20 Build-Number: %d",buildNumber);

            //Stop sending pings to MK20
            _mk20OK = true;
            _firmwareChecked = false;

            //Send ESP build number in response
            buildNumber = FIRMWARE_BUILDNR;
            *sendResponse = true;
            *responseDataSize = sizeof(int);
            memcpy(responseData,&buildNumber,sizeof(int));
        } else if (header.commType == ResponseSuccess) {
            EventLogger::log("MK20 answered ping response");
            int buildNumber = 0;
            memcpy(&buildNumber,data,dataSize);

            _mk20OK = true;
            _mk20->setBuildNumber(buildNumber);
        }
    }
    else if (taskID == TaskID::StartFirmwareUpdate)
    {
        //TODO: Give URL for ESP firmware
        *sendResponse = false;
        *responseDataSize = 0;

        startFirmwareUpdate();

        //ESPFirmwareUpdate* espUpdateFirmware = new ESPFirmwareUpdate(_firmwareUpdateInfo->esp_url);
        //Application.pushMode(espUpdateFirmware );
    }
    else if (taskID == TaskID::StartWifi || taskID == TaskID::StopWifi)
    {
        EventLogger::log("Wifi TASK");
        Mode* mw = new ManageWifi();
        Application.pushMode(mw);

        *sendResponse = false;
    }
    else if (taskID == TaskID::SystemInfo)
    {
        EventLogger::log("GOT SYSTEM INFO");
        *sendResponse = false;
    }
    else if (taskID == TaskID::RunPrinterGCode)
    {
        EventLogger::log("GOT REPLY ON GCODE RUN");
        // do not need to create new mode for this...

        *sendResponse = false;
    }
    else if (taskID == TaskID::DebugLog)
    {
        //Just ignore it and don't send a response
        *sendResponse = false;
    }

	return true;
}
