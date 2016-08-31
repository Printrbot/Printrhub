#include "Printr.h"
#include <ArduinoJson.h>
#include "SD.h"
#include "framework/core/HAL.h"

Printr::Printr():
readBuffer(PrintrBuffer()),
_sendNext(true),
_printing(false),
_listener(NULL),
_homeX(false),
_homeY(false),
_homeZ(false) {

}

Printr::~Printr() {
}

void Printr::init() {
  Serial1.begin(115200);
  Serial1.attachCts(CTS_PIN);
  Serial1.attachRts(RTS_PIN);
  Serial1.println("{sr:{line:t,he1t,he1st,he1at,dist:t,stat:t}}");
}

void Printr::loop() {
  readSerial();
  if (_printing) {
    // handle file printing here...
  }
}

void Printr::turnOffHotend() {
  Serial1.println("M100({he1st:0})");
}

void Printr::stopAndFlush() {
  Serial1.println("!%");
  delay(1000);
}

void Printr::readSerial() {
  while (Serial1.available()) {

    readBuffer.line_buff[readBuffer.line_idx] = Serial1.read();
    if (readBuffer.line_buff[readBuffer.line_idx] == '\n') {
      readBuffer.line_buff[readBuffer.line_idx + 1] = '\0';
      parseResponse();
      readBuffer.line_idx = 0;
    }
    else
      readBuffer.line_idx++;
  }
}

void Printr::startPrint(File file) {
  _lastSentProgramLine = 1;
  _processedProgramLine = 0;
  // figure out total lines....
  _printFile = file;
  _sendNext = true;
  _printing = true;
}

void Printr::homeX() {
  sendLine("G28.2 X0");
  _homeX = true;
}

void Printr::homeY() {
  sendLine("G28.2 Y0");
  _homeY = true;
}

void Printr::homeZ() {
  sendLine("G28.2 Z0");
  _homeZ = true;
}

void Printr::programEnd() {
  if (_listener != nullptr)
    _listener->printrCallback("end", nullptr);

  if (!_printing)
    return;

  _printFile.close();
  _printing = false;
  _lastSentProgramLine = 0;
  _processedProgramLine = 0;
  _totalProgramLines = 0;
  // turn off the hotend just in case
  turnOffHotend();
}

void Printr::sendLine(String line) {
  Serial1.println(line);
}

void Printr::parseResponse() {
  if (_listener == NULL || _listener == nullptr) return;
  char * line = readBuffer.line_buff;
  if (line[0] == '{') {
    StaticJsonBuffer<512> jsonBuffer;
    StaticJsonBuffer<512> rBuffer;

    JsonObject& o = jsonBuffer.parseObject(line);
    if (!o.success()) {
      // failed...
    } else {
      // https://github.com/synthetos/TinyG/wiki/TinyG-Status-Codes#status-codes
      // footer f: {}
/*
      String foot = o["f"];
      if (foot.length() > 0) {
        // get sr
        JsonArray& _f = rBuffer.parseArray(foot);

        switch((int) _f[1]) {
          case 20:
            //fatal error
            Display.fillRect(0,0,320,240,ILI9341_WHITE);
            Display.setCursor(10,10);
            Display.setTextColor(ILI9341_BLACK);
            Display.println("Error 20: internal error!");
            Display.fadeIn();
            break;

          default:
            _sendNext = true;
        }
      }
*/
/*
      // parse response r: {}
      String rr = o["r"];
      if (rr.length() > 0) {

        //JsonObject& r = rBuffer.parseObject(rr);
      }
*/
      // parse sr
      String sr = o["sr"];
      if (sr.length() > 0) {
        JsonObject& _sr = rBuffer.parseObject(sr);
        // {sr: {stat:0}}
        // https://github.com/synthetos/TinyG/wiki/TinyG-Status-Codes#status-report-enumerations
        if (_sr["stat"]) {
          _stat = _sr["stat"];
          switch(_stat) {
            case 2:
              // hmmmm ... need to handle this better
              // alarm
              Display.fillRect(0,0,320,240,ILI9341_RED);
              Display.setCursor(10,10);
              Display.setTextColor(ILI9341_WHITE);
              Display.println("ALARM: machine is in alarm state!");
              Display.setCursor(10,30);
              Display.println(_stat);
              Display.fadeIn();
              break;

            case 4:
              // program end via M2, M30
              // finish print if printing
              programEnd();
              break;
          }
        }

        // parse hotend 1 temperature
        if (_sr["he1t"]) {
          _hotend1Temp = (float) _sr["he1t"];
          if (_listener != NULL) {
            _listener->printrCallback("he1t", &_hotend1Temp);
          }
        }

        if (_sr["line"]) {
          _processedProgramLine = _sr["line"];
          /*
          if (_processedProgramLine == _totalProgramLines)  {
            programEnd();
          }
          */
          _progress = (float) (_processedProgramLine / _totalProgramLines);

          if (_listener != NULL)
            _listener->printrCallback("line", &_progress);
        }
      }
    }
  }
}
