#include "controller.h"
#include <ArduinoJson.h>
#include "printer.h"
#include "net.h"
#include "card.h"
#include "macros.h"
//#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

void Controller::runJson(JsonObject * o)
{
  String msg = o->get("m");
  DEBUG(msg);

  // AUTH--------------------------------------------------
  if (msg == String("auth")) {
    Net::authorized = o->get("d");
    if (Net::authorized) {
      DEBUG("YOU ARE AUTHORIZED");
      Printer::getInfo();
    } else {
      DEBUG("YOU ARE NOT AUTHORIZED :\"(");
    }
  }
  else if (msg == String("download")) {
    DEBUG("dowloading gcode");
    Card::downloadFile(o->get("shortid"), o->get("gcode"));
  }
  else if (msg == String("fwupdate")) {
    t_httpUpdate_return ret = ESPhttpUpdate.update(o->get("d"));
    switch(ret) {
        case HTTP_UPDATE_FAILED:
            Serial.printf(";HTTP_UPDATE_FAILD Error");
            break;

        case HTTP_UPDATE_NO_UPDATES:
            DEBUG("HTTP_UPDATE_NO_UPDATES");
            break;

        case HTTP_UPDATE_OK:
            DEBUG("HTTP_UPDATE_OK");
            break;
    }
  }
  else if (msg == String("listfiles")) {
    DEBUG("listing files");
    Card::listFiles();
  }
  else if (msg == String("gc")) {
    String gc = o->get("d");
    // send gcode to printer
    Serial.println(gc);
  }
  else if (msg == String("print")) {
    String f = o->get("d");
    Printer::startPrint(f);
  }
  else if (msg == String("stat")) {
    String response = "{\"m\":\"stat\", \"d\": {\"printing\":\""+ String(Printer::printing)+ "\"," +
                      "\"paused\":\""+ String(Printer::paused)+"\"}}\n";
    Net::client.print(response);
  }
};

void Controller::fromPrinter(char * c)
{
  if (c[0] == '{') {
    // anything that is coming from printer that is in json format
    // should be directed to server
    Net::println(c);
  }
  else if (c[0] == 'o' && c[1] == 'k') {
    if (Printer::printing) {
      Printer::sendNext = true;
    }
    return;
  }

  DEBUG("esp: received from printer: ");
  DEBUG(c);
}
