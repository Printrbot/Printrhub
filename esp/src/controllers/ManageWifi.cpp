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
extern Config config;

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
    case ScanWifi:
      return true;
      break;
    default:
      return false;
  }
}

void ManageWifi::loop() {

  if (_state == StateOffline && _currentTask == StartWifi) {
    _state = StateConnecting;
    if (config.data.blank) {
  		// start the wifi in adhoc mode
  		createAP();
  	} else {
  		connectWifiStation();
  	}
    return;
  }

  if (_currentTask == ScanWifi && _state != StateSuccess ) {
    // currently just outputs detected networks to event logger for debugging
    EventLogger::log("SCANNING WIFI");
    int n = WiFi.scanNetworks();
    EventLogger::log("scan done");
    if (n == 0)
      EventLogger::log("no networks found");
    else {
      for (int i = 0; i < n; ++i) {
        EventLogger::log(WiFi.SSID(i).c_str());
        delay(10);
      }
    }
    EventLogger::log("done");
    _state = StateSuccess;
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
  }
}

void ManageWifi::createAP() {
  WiFi.mode(WIFI_AP);
  if (config.data.blank) {
    WiFi.softAP("printrbot", "printrbot");
    MDNS.begin("printrbot");
  }
  else {
    WiFi.softAP(config.data.name, config.data.apPassword);
    MDNS.begin(config.data.name);
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
