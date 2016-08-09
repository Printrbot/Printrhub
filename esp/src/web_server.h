#ifndef WEB_SERVER_H
#define WEB_SERVER_H
#include <ESP8266WiFi.h>
//#include <WebSocketsServer.h>
#include <FS.h>

class WebServer {
public:
  WebServer();
  void begin();
  void update();
};


#endif
