#ifndef NET_H
#define NET_H

#include "Arduino.h"
#include "config.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
//#include <ESP8266WiFi.h>

class Controller;

#define CONFIG_START 0

class Net {
public:
  static bool connectWifi();
  static bool connectServer();
  static bool wifiConnected;
  static bool serverConnected;
  static bool authorized;
  static void read();
  static void debugMsg(String s);
  static void sendPrinterResponse(String s);
  static void print(char *c);
  static void print(String c);
  static void println(char *c);
  static void send(char *c);
  static void sendf(char *s,...);
  static WiFiClient client;
  static int wifiRetry;
  static int serverRetry;

};


#endif
