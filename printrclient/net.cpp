#include "net.h"
#include "config.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "controller.h"
#include "macros.h"


bool Net::wifiConnected = false;
bool Net::serverConnected = false;
bool Net::authorized = false;
int Net::wifiRetry = 5;
int Net::serverRetry = 15;
WiFiClient Net::client = client;

bool Net::connectWifi()
{
  int ret = 0;

  DEBUG(Config::data.wifiSsid);
  DEBUG(Config::data.wifiPassword);

  WiFi.begin(Config::data.wifiSsid, Config::data.wifiPassword);
  delay(2000);

  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    ret++;
    DEBUG(ret);
    if (ret > wifiRetry) {
      DEBUG("Unable to connect to wifi");
      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    DEBUG("WiFi connected");
    DEBUG("IP address: ");
    DEBUG(WiFi.localIP());
    return true;
  }

  return false;
}

bool Net::connectServer()
{
  DEBUG("connecting to server");
  long _port = atol(Config::data.port);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  DEBUG("server: ");
  DEBUG(Config::data.server);
  DEBUG("port: ");
  DEBUG(_port);

  if (!client.connect(Config::data.server, _port)) {
    DEBUG("connection failed");
    return false;
  } else {
    DEBUG("connected fine, try to authenticate");
    root["m"] = "auth";
    root["d"] = Config::data.jwt;
    root.printTo(client);
    client.print('\n');
    serverConnected = true;
    delay(10);
    return true;
  }
}


void Net::read()
{
  if (Net::client.available()) {
    String line = Net::client.readStringUntil('\n');
    StaticJsonBuffer<1024> jsonBuffer;

    if (line[0] == '{') {
        DEBUG("parsing json");
        DEBUG(line);
        JsonObject& root = jsonBuffer.parseObject(line);
        if (!root.success()) {
          DEBUG(";parseObject() failed");
          return;
        } else {
          Controller::runJson(&root);
        }
    }
  }
}

void Net::debugMsg(String s)
{
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["debug"] = s;
  root.printTo(client);
  client.print('\n');
}

void Net::print(char *c) {
  client.print(c);
}

void Net::print(String c) {
  client.print(c);
}

void Net::println(char *c) {
  client.println(c);
}

void Net::send(char *c) {
  client.print(c);
}


void Net::sendf(char *s,...) {
  char buff[256];
  va_list args;
  va_start (args,s);
  vsnprintf(buff,sizeof(buff),s,args);
  va_end (args);
  buff[sizeof(buff)/sizeof(buff[0])-1]='\0';
  client.print(buff);
}
