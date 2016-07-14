/*
 * Printrhub -- your best friend in your lab
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
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include "CommStack.h"
#include "PubNub.h"
#include "PubNubLogger.h"
#include "FileLogger.h"

// Pin definitions
const int LED_PIN = 0;

extern FileLogger Logger;

extern void pubnub_callback(char* message);
extern void logToPubNub(const char* message);
extern void logValueToPubNub(const char* message, int value);
extern void logValueToPubNub(const char* message, float value);
extern void logValueToPubNub(const char* message, String value);
extern void logValueToPubNub(const char* message, size_t value);

#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)
#define logError(msg) (__FILE__ " line " STRINGIZE(__LINE__) ": " msg ": ")
#define logString(msg) (__FILE__ " line " STRINGIZE(__LINE__) ": " msg)

#define LOG(m) Logger.println(logString(m));
#define LOG_VALUE(m,v) Logger.print(logError(m));Logger.println(v);

class Mode;

class ApplicationClass: CommStackDelegate
{
#pragma mark Constructor
public:
	ApplicationClass();
	~ApplicationClass();

#pragma mark Application Flow
	void loop();
	void setup();
	void pushMode(Mode* mode);
	Mode* currentMode() { return _currentMode; };
	void connectWiFi();
	CommStack* getMK20Stack() const { return _mk20; };
	void updateServer();

#pragma mark Time Management
	float getDeltaTime();

#pragma mark CommStackDelegate
public:
	bool runTask(CommHeader& header, const uint8_t* data, uint8_t* responseData, uint16_t* responseDataSize);

#pragma mark Member Variables
private:
	bool _firstModeLoop;
	Mode *_nextMode;
	Mode *_currentMode;
	unsigned long _lastTime;
	float _deltaTime;
	unsigned long _buttonPressedTime;
	WiFiManager wiFiManager;
	CommStack* _mk20;
	WiFiServer _server;
};

extern ApplicationClass Application;

#endif //_APPLICATION_H_
