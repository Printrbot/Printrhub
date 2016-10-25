//
// Created by Phillip Schuster on 09.07.16.
//

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "Arduino.h"
#include "StackArray.h"
#include "SceneController.h"
#include "ColorTheme.h"
#include "../views/Icon.h"
#include "PHDisplay.h"
#include "ILI9341_t3.h"
#include "Adafruit_FT6206.h"
#include "../animation/Animator.h"
#include "font_PT_Sans-Narrow-Web-Regular.h"
#include "CommStack.h"
#include "HAL.h"
#include "LED.h"
#include <SoftwareSerial.h>
#include "BackgroundJob.h"
#include "EventLogger.h"

#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)
#define logError(msg) (__FILE__ " line " STRINGIZE(__LINE__) ": " msg ": ")
#define logString(msg) (__FILE__ " line " STRINGIZE(__LINE__) ": " msg)

//#define DEBUG_LOGS
#ifdef DEBUG_LOGS
#define LOG(m) DebugSerial.println(logString(m));DebugSerial.flush();
#define LOG_VALUE(m,v) DebugSerial.print(logError(m));DebugSerial.println(v);Serial.flush();
#else
#define LOG(m)
#define LOG_VALUE(m,v)
#endif

#define FIRMWARE_VERSION "0.12"
#define FIRMWARE_BUILDNR 99

enum class NetworkMode: uint8_t {
    Unconnected = 0,
    Client = 1,
    AccessPoint = 2
};

struct SystemInfo {
    char ipaddress[15];         //IPV4
    NetworkMode networkMode;
    char SSID[32];              //http://compnetworking.about.com/cs/wireless/g/bldef%5Fssid.htm
    char printerName[16];
    char serialNumber[16];
    char firmwareVersion[5];
    int16_t buildNr;
    bool hasPassword;
};

class SceneController;
class View;

class ApplicationClass : public CommStackDelegate
{
#pragma mark Constructor
public:
	ApplicationClass();
	~ApplicationClass();

#pragma mark Application Flow
  void pingESP();
  void resetESP();
  int getBuildNumber() { return _buildNumber; }
	void loop();
	void setup();
	void pushScene(SceneController* scene, bool cancelModal=false);
	SceneController* currentScene() { return _currentScene; };

#pragma mark Background Jobs
  void pushJob(BackgroundJob* job);
	BackgroundJob* currentJob() { return _currentJob; };

#pragma mark Touch Handling
	void handleTouches();

#pragma mark Color Theme
	ColorTheme* getTheme();

#pragma mark CommStackDelegate
public:
	CommStack* getESPStack();
	bool runTask(CommHeader& header, const uint8_t* data, size_t dataSize, uint8_t* responseData, uint16_t* responseDataSize, bool* sendResponse, bool* success);
	void onCommStackError();

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
  int _buildNumber;
  bool _espOK;
  unsigned long _lastESPPing;
	BackgroundJob* _currentJob;
	BackgroundJob* _nextJob;
	bool _awake;
};

extern ApplicationClass Application;
extern PHDisplay Display;
extern Adafruit_FT6206 Touch;
extern LED StatusLED;
extern EventLoggerClass EventLogger;
extern int globalLayerId;
extern int globalLayersCreated;
extern int globalLayersDeleted;

#ifdef DEBUG_USE_SOFTWARE_SERIAL
extern SoftwareSerial DebugSerial;
#endif

#endif //_APPLICATION_H_
