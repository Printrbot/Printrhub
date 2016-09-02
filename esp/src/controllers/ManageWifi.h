#ifndef CONNECT_WIFI_H
#define CONNECT_WIFI_H

#include "../core/Mode.h"
#include <ESP8266WiFi.h>
#include "../core/CommStack.h"

class ManageWifi: public Mode
{
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
  bool runTask(CommHeader& header, const uint8_t* data, size_t dataSize, uint8_t* responseData, uint16_t* responseDataSize, bool* sendResponse, bool* success);
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
