#include "ManageWifi.h"
#include "../core/Commstack.h"
#include <ESP8266WiFi.h>
#include "../config.h"
#include <ESP8266mDNS.h>
#include "../web_server.h"
#include "Idle.h"
#include "Application.h"
#include "event_logger.h"

extern WebServer webserver;
extern ApplicationClass Application;

ManageWifi::ManageWifi() :
_state(StateOffline),
_currentTask(StartWifi),
_webServerStarted(false) {

}

ManageWifi::~ManageWifi() {

}

bool ManageWifi::handlesTask(TaskID taskID) {
  switch(taskID) {
    case StartWifi:
    case StopWifi:
      return true;
      break;
    default:
      return false;
  }
}

void ManageWifi::loop() {

  if (_state == StateOffline && _currentTask == StartWifi) {
    _state = StateConnecting;
    if (Config::data.blank) {
  		// start the wifi in adhoc mode
  		createAP();
  	} else {
  		connectWifiStation();
  	}
    return;
  }

  // always start the web server after starting wifi
  if (_state == StateConnected && ! _webServerStarted) {
    webserver.begin();
    _webServerStarted = true;
    _state = StateSuccess;
  }

  if (_state == StateSuccess) {
    Mode* mode = new Idle();
    Application.pushMode(mode);
  }

  if (_state == StateError) {
    Mode* mode = new Idle();
    Application.pushMode(mode);
  }
}

void ManageWifi::connectWifiStation() {
  _connectRetry = 0;
  WiFi.mode(WIFI_STA);
  WiFi.begin(Config::data.wifiSsid, Config::data.wifiPassword);

  while (WiFi.status() != WL_CONNECTED) {
    delay(3000);
    _connectRetry++;

    if (_connectRetry > _maxConnectRetry) {
      // if the connection failed,
      // set the response to error
      _state = StateError;
      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    _ipAddress = WiFi.localIP();
    _state = StateConnected;
    MDNS.begin(Config::data.name);
  }
}

void ManageWifi::createAP() {
  WiFi.mode(WIFI_AP);
  if (Config::data.blank) {
    WiFi.softAP("printrbot", "printrbot");
    MDNS.begin("printrbot");
  }
  else {
    WiFi.softAP(Config::data.name, Config::data.apPassword);
    MDNS.begin(Config::data.name);
  }
  _ipAddress = WiFi.softAPIP();
  _state = StateConnected;
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

bool ManageWifi::runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool* success) {
  if (header.getCurrentTask() == StartWifi) {
    if (header.commType == Request) {
      _state = StateOffline;
      _currentTask = StartWifi;
    }
  }
  return true;
}
