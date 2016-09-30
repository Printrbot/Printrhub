#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <Arduino.h>
#include "core/CommStack.h"
#include <FS.h>
#include "event_logger.h"

class Mode;

#define LOG(m) //EventLogger::log(m)//Serial.println((m)) //Logger.println(logString(m));
#define LOG_VALUE(m,v) //EventLogger::log((m))//;Serial.println(v)//Logger.print(logError(m));Logger.println(v);

#define FIRMWARE_URL_ESP "http://www.appfruits.com/files/esp/firmware.bin"
#define FIRMWARE_URL_MK20 "http://www.appfruits.com/files/mk29/firmware.bin"

class ApplicationClass: CommStackDelegate
{

public:
	ApplicationClass();
	~ApplicationClass();

	void loop();
	void setup();
	void pushMode(Mode* mode);
	Mode* currentMode() { return _currentMode; };
	CommStack* getMK20Stack() const { return _mk20; };
	float getDeltaTime();

public:
	bool runTask(CommHeader& header, const uint8_t* data, size_t dataSize, uint8_t* responseData, uint16_t* responseDataSize, bool* sendResponse, bool* success);

private:
	bool _firstModeLoop;
	Mode *_nextMode;
	Mode *_currentMode;
	unsigned long _lastTime;
	float _deltaTime;
	unsigned long _buttonPressedTime;
	CommStack* _mk20;
//	WiFiServer _server;
};

extern ApplicationClass Application;

#endif //_APPLICATION_H_
