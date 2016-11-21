/*
 * Handles WiFi connection. If WiFi connection is configured it tries to connect
 * to the given WiFi network, if it's not configured yet or connection fails it
 * falls back to access port mode where ESP creates a small HTTP server that can
 * be used to configure the WiFi connection settings.
 *
 * Copyright (c) 2016 Printrbot Inc.
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

#ifndef CONNECT_WIFI_H
#define CONNECT_WIFI_H

#include "../core/Mode.h"
#include <ESP8266WiFi.h>
#include "../core/CommStack.h"

class ManageWifi : public Mode {
 public:
  typedef enum State {
	StateError = 0,
	StateSuccess = 1,
	StateOffline = 2,
	StateConnectStation = 3,
	StateConnecting = 5,
	StateUpdateConfig = 6,
	StateConnected = 7,
	StateScanning = 8
  };

  ManageWifi();
  ~ManageWifi();

  void loop();
  void onWillStart();
  void onWillEnd();

  //bool handlesTask(TaskID taskID);
  bool runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool *success);
  virtual bool handlesTask(TaskID taskID);
  String getName();
  void setTask(TaskID taskID) { _currentTask = taskID; };

 private:
  void createAP();
  void connectWifiStation();
  WiFiMode_t _wifiMode;

  State _state;
  IPAddress _ipAddress;
  int _connectRetry = 0;
  int _maxConnectRetry = 5;
  TaskID _currentTask;
  bool _webServerStarted;
};

#endif
