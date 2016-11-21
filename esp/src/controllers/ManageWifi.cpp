/*
 * Handles WiFi connection. If WiFi connection is configured it tries to connect
 * to the given WiFi network, if it's not configured yet or connection fails it
 * falls back to access port mode where ESP creates a small HTTP server that can
 * be used to configure the WiFi connection settings.
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/behind-the-scenes-printrbot-simple-2016/
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Mick Balaban
 * https://github.com/Printrbot/Printrhub
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

#include "ManageWifi.h"
#include "../core/Commstack.h"
#include <ESP8266WiFi.h>
#include "../config.h"
#include <ESP8266mDNS.h>
#include "../web_server.h"
#include "Idle.h"
#include "Application.h"
#include "event_logger.h"
#include "CheckForFirmwareUpdates.h"
#include <ArduinoJson.h>

extern WebServer webserver;
extern ApplicationClass Application;
extern Config config;

ManageWifi::ManageWifi() :
	Mode(),
	_state(StateOffline),
	_currentTask(TaskID::StartWifi),
	_webServerStarted(false) {

}

ManageWifi::~ManageWifi() {

}

bool ManageWifi::handlesTask(TaskID taskID) {
  switch (taskID) {
	case TaskID::StartWifi:
	case TaskID::StopWifi:
	case TaskID::ScanWifi:return true;
	  break;
	default:return false;
  }
}

void ManageWifi::loop() {

  if (_state == StateOffline && _currentTask == TaskID::StartWifi) {
	_state = StateConnecting;
	if (config.data.blank) {
	  // start the wifi in adhoc mode
	  createAP();
	} else {
	  connectWifiStation();
	}
	return;
  }

  if (_currentTask == TaskID::ScanWifi && _state != StateSuccess) {
	// currently just outputs detected networks to event logger for debugging
	EventLogger::log("SCANNING WIFI");

	StaticJsonBuffer<1000> jsonBuffer;
	JsonObject &root = jsonBuffer.createObject();
	JsonArray &data = root.createNestedArray("networks");

	File netfile = SPIFFS.open("/net.json", "w");
	int n = WiFi.scanNetworks();
	EventLogger::log("scan done");
	if (n == 0)
	  EventLogger::log("no networks found");
	else {
	  for (int i = 0; i < n; ++i) {
		//netfile.write(WiFi.SSID(i).c_str());
		data.add(String(WiFi.SSID(i)));
		EventLogger::log(WiFi.SSID(i).c_str());
		delay(10);
	  }
	}
	root.printTo(netfile);
	netfile.close();
	EventLogger::log("done");
	_state = StateSuccess;
	return;
  }

  // always start the web server after starting wifi
  if (_state == StateConnected && !_webServerStarted) {
	webserver.begin();
	_webServerStarted = true;
	_state = StateSuccess;
  }

  if (_state == StateSuccess) {

	if (config.data.blank) {
	  //We are in AP mode, don't check firmware update as we don't have internet connection yet
	  Mode *mode = new Idle();
	  Application.pushMode(mode);
	} else {
	  //Check firmware data
	  CheckForFirmwareUpdates *updateCheck = new CheckForFirmwareUpdates();
	  Application.pushMode(updateCheck);
	}

  }

  if (_state == StateError) {
	Mode *mode = new Idle();
	Application.pushMode(mode);
  }
}

void ManageWifi::connectWifiStation() {
  _connectRetry = 0;
  WiFi.mode(WIFI_STA);
  WiFi.begin(config.data.wifiSsid, config.data.wifiPassword);

  while (WiFi.status() != WL_CONNECTED) {
	delay(3000);
	_connectRetry++;

	if (_connectRetry > _maxConnectRetry) {
	  // if the connection failed,
	  // create AP
	  createAP();
	  break;
	}
  }

  if (WiFi.status() == WL_CONNECTED) {
	_ipAddress = WiFi.localIP();
	_state = StateConnected;
	MDNS.begin(config.data.name);

	//Set System Info
	Application.getSystemInfo()->networkMode = NetworkMode::Client;
	strcpy(Application.getSystemInfo()->ipaddress, _ipAddress.toString().c_str());
	strcpy(Application.getSystemInfo()->SSID, config.data.wifiSsid);
	strcpy(Application.getSystemInfo()->printerName, config.data.name);
  }
}

void ManageWifi::createAP() {
  WiFi.mode(WIFI_AP);
  if (config.data.blank) {
	WiFi.softAP("printrbot", "printrbot");
	MDNS.begin("printrbot");
  } else {
	WiFi.softAP(config.data.name, config.data.apPassword);
	MDNS.begin(config.data.name);
  }
  _ipAddress = WiFi.softAPIP();
  _state = StateConnected;

  //Set System Info
  Application.getSystemInfo()->networkMode = NetworkMode::AccessPoint;
  strcpy(Application.getSystemInfo()->ipaddress, _ipAddress.toString().c_str());
  strcpy(Application.getSystemInfo()->SSID, WiFi.BSSIDstr().c_str());
  strcpy(Application.getSystemInfo()->printerName, config.data.name);

  if (strlen(config.data.apPassword) > 0) {
	Application.getSystemInfo()->hasPassword = true;
  }
}

void ManageWifi::onWillStart() {

}

void ManageWifi::onWillEnd() {

}

String ManageWifi::getName() {
  return "ManageWifi";
}

/*
bool ManageWifi::handlesTask(TaskID taskID) {
  if (taskID == this.HandlesT ) {
    return true;
  }
  return false;
}
*/

bool ManageWifi::runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool *success) {
  if (header.getCurrentTask() == TaskID::StartWifi) {
	if (header.commType == Request) {
	  _state = StateOffline;
	  _currentTask = TaskID::StartWifi;
	}
  }

  return true;
}
