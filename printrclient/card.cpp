#include "Arduino.h"
#include "card.h"
#include <SPI.h>
#include <SD.h>
#include "net.h"
#include <ESP8266HTTPClient.h>
#include "printer.h"
#include "macros.h"

bool Card::ok = false;
bool Card::hasKeyFile = false;


void Card::init() {
  if (!SD.begin(4)) {
    ok = false;
    DEBUG("can't init sd card");
  } else {
    ok = true;
    if (SD.exists("config.txt")) {
      hasKeyFile = true;
      DEBUG("HAS KEY FILE");
    } else {
      DEBUG("NO KEY FILE");
    }
  }
}

void Card::downloadFile(String short_id, String url) {

  char fid[8];
  short_id.toCharArray(fid, 8);

  if (SD.exists(fid)) {
    DEBUG("FILE EXISTS");
    Printer::startPrint(fid);
    return;
  }

  HTTPClient http;
  http.begin(url); //HTTP
  int httpCode = http.GET();

  if (httpCode > 0) {
    if (httpCode == 200) {
      // create a file on sd card
      File gfile = SD.open(fid, FILE_WRITE);
      DEBUG("WRITING GCODE TO SDCARD");
      int len = http.getSize();
      uint8_t buff[128] = { 0 };

      WiFiClient * stream = http.getStreamPtr();
      // read all data from server
      while(http.connected() && (len > 0 || len == -1)) {
          // get available data size
          size_t size = stream->available();
          if(size) {
              // read up to 128 byte
              int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));
              gfile.write(buff, c);

              if(len > 0) {
                  len -= c;
              }
          }
          delay(1);
      }
      gfile.close();
      DEBUG("WRITING DONE");

      // kick off print
      Printer::startPrint(fid);

    } else {
      DEBUG("error downloading gcode file");
    }
  }
}

void Card::listFiles() {
  if (Card::ok) {
    File root = SD.open("/");

    String response = "{\"m\":\"files\", \"d\":[";
    int cnt = 0;

    root.rewindDirectory();
    while(true) {
      File entry =  root.openNextFile();
      if (!entry) {
        // no more files
        break;
      }

      if (cnt > 0)
        response += ",";

      response += "\"" + String(entry.name()) + "\"";

      cnt++;
      entry.close();
    }

    response += "]}\n";
    DEBUG(response);
    Net::client.print(response);
  } else {
    Net::client.print(String("{\"error\":\"sd card not available\"}\n"));
  }
}

void Card::updateConfigFromSd() {
  int s;
  int x = 0;
  int y = 0;
  DEBUG("reading file on sd card");

  File keyFile = SD.open("config.txt", FILE_READ);

  while ((s = keyFile.read()) > 0) {
        if (s == 10) s = '\0';
        switch(y) {
          case 0:
            Config::data.wifiSsid[x] = s;
            break;
          case 1:
            Config::data.wifiPassword[x] = s;
            break;
          case 2:
            Config::data.server[x] = s;
            break;
          case 3:
            Config::data.port[x] = s;
            break;
          case 4:
            Config::data.jwt[x] = s;
            break;
        }

        if (s == '\0') {
          y++;
          x = 0;
        } else {
          x++;
        }
  }
  keyFile.close();
}
