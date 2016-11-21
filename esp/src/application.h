/*
 * This is the main application class. It's a singleton that has a loop function
 * that is called regularly and handles communication to MK20 via CommStack
 * and handles modes that do the real work.
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/behind-the-scenes-printrbot-simple-2016/
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Mick Balaban, Phillip Schuster
 * https://github.com/Printrbot/Printrhub
 *
 * Developed in cooperation with Phillip Schuster (@appfruits) from appfruits.com
 * http://www.appfruits.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <Arduino.h>
#include "core/CommStack.h"
#include <FS.h>
#include "event_logger.h"
#include "MK20.h"
#include "errors.h"

class Mode;

#define LOG(m) //EventLogger::log(m)//Serial.println((m)) //Logger.println(logString(m));
#define LOG_VALUE(m, v) //EventLogger::log((m))//;Serial.println(v)//Logger.print(logError(m));Logger.println(v);

struct FirmwareUpdateInfo {
  int buildnr;
  String mk20_url;
  String esp_url;
  String mk20_ui_url;
};

enum class NetworkMode : uint8_t {
  Unconnected = 0,
  Client = 1,
  AccessPoint = 2
};

struct SystemInfo {
  char ipaddress[15];         //IPV4
  NetworkMode networkMode;
  char SSID[32];              //http://compnetworking.about.com/cs/wireless/g/bldef%5Fssid.htm
  char printerName[16];
  char serialNumber[37];
  char firmwareVersion[5];
  int16_t buildNr;
  bool hasPassword;
};

class ApplicationClass : CommStackDelegate {

 public:
  ApplicationClass();
  ~ApplicationClass();

  bool firmwareUpdateNotified() { return _firmwareChecked; };
  bool isMK20Available() { return _mk20OK; };
  void pingMK20();
  void updateMK20Firmware();
  int getBuildNumber() { return _buildNumber; }
  void loop();
  void setup();
  void pushMode(Mode *mode);
  Mode *currentMode() { return _currentMode; };
  MK20 *getMK20Stack() const { return _mk20; };
  float getDeltaTime();
  void startFirmwareUpdate();
  void setMK20Timeout();

  void sendPulse(int length = 5, int count = 1);

  void reset();

  void idle();
  void handleError(DownloadError error);

  void setFirmwareUpdateInfo(FirmwareUpdateInfo *info) { _firmwareUpdateInfo = info; };
  bool firmwareUpdateAvailable() { return _firmwareUpdateInfo != NULL; };
  FirmwareUpdateInfo *getFirmwareUpdateInfo() { return _firmwareUpdateInfo; };
  SystemInfo *getSystemInfo() { return &_systemInfo; };

 private:
  void initializeHub();

 public:
  bool runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool *success);

 private:
  bool _firstModeLoop;
  Mode *_nextMode;
  Mode *_currentMode;
  unsigned long _lastTime;
  float _deltaTime;
  unsigned long _buttonPressedTime;
  MK20 *_mk20;
  bool _mk20OK;
  unsigned long _appStartTime;
  unsigned long _lastMK20Ping;
//	WiFiServer _server;
  int _buildNumber;
  FirmwareUpdateInfo *_firmwareUpdateInfo;
  SystemInfo _systemInfo;
  bool _firmwareChecked;
};

extern ApplicationClass Application;

#endif //_APPLICATION_H_
