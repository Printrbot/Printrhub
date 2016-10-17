#ifndef WEB_SERVER_H
#define WEB_SERVER_H
#include <ESP8266WiFi.h>
//#include <WebSocketsServer.h>
#include <FS.h>
#include <ESPAsyncWebServer.h>

class WebServer {
public:
  WebServer();
  void begin();
  void update();

private:
  bool validateAuthentication(AsyncWebServerRequest *request);
};


#endif
