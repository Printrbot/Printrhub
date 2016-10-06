#include "Printr.h"
#include <ArduinoJson.h>
#include "SD.h"
#include "framework/core/HAL.h"
#include "scenes/settings/DataStore.h"

extern DataStore dataStore;

Printr::Printr():
readBuffer(PrintrBuffer()),
_sendNext(true),
_printing(false),
_listener(NULL),
_homeX(false),
_homeY(false),
_homeZ(false),
_currentAction(0){

}

Printr::~Printr() {
}

void Printr::init() {
  Serial1.begin(115200);
  Serial1.attachCts(CTS_PIN);
  Serial1.attachRts(RTS_PIN);

  sendLine("{sr:{line:t,he1t:t,he1st:t,he1at:t,stat:t}}");
  delay(200);
  sendLine("{_leds:4}");
  delay(200);
  sendLine("{sv:1}");

  //Serial1.println("{sr:{line:t},_leds:4}");
}

void Printr::loop() {
  readSerial();

  if (!_printing) return;
  if (!_sendNext) return;

  String inStr;

  // check if we are in startup sequence
  if (_currentAction == 1) {
    if (_startGCodeFile.available()) {
      inStr = _startGCodeFile.readStringUntil('\n');
    } else {
      // done with startup gcode, start the main job file
      _currentAction = 2;
      _lastSentProgramLine = 1;
      _processedProgramLine = 0;
      // close the start gcode file
      _startGCodeFile.close();


      sendLine("M100({_leds:1})");
      sendLine("G0 Z5");

      float headOffset = 5.0 - dataStore.getHeadOffset();
      String gco = String("G92 Z") + String(headOffset);


      sendLine(gco); // offset should come from eeprom

      int sent = 0;
      // send 6 lines of job gcode
      // then wait for responses to send more
      while(1) {
        inStr = _printFile.readStringUntil('\n');
        inStr.trim();
        char f = inStr[0];
        if (inStr.length() > 0 && inStr[0] != ';') {
          sendLine(inStr);
          sent++;
        }
        if (sent > 6)
          break;
      }
      return;
    }
  }
  else if (_currentAction == 2) {
    if (_printFile.available()) {
      inStr = _printFile.readStringUntil('\n');
    } else {
      // finished reading the file
      // This is a hack until total line numbers
      // is implemented, then program will end
      // when M3 is passed to it, or when _processedProgramLine == _totalProgramLines
      programEnd();
    }
  }

  inStr.trim();
  // send it to printer
  if (inStr.length() > 0 && inStr[0] != ';') {
    sendLine(String("N") + String(_lastSentProgramLine) + String(" ") + inStr);
    _lastSentProgramLine++;
    _sendNext = false;
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

int Printr::startJob(String filePath) {
  _printFile = SD.open(filePath.c_str(), FILE_READ);

  int totalLines = -1;

  // read json header (if available)
  char i[2];
  _printFile.read(&i, 2);
  if (i[0] == ';' && i[1] == '{') {
    // found json string in header, parse it now
    _printFile.seek(1);
    String js = _printFile.readStringUntil('\n');

    StaticJsonBuffer<512> jb;
    JsonObject& h = jb.parseObject(js);

    if (h.success()) {
      totalLines = h["lines"];
    }
  }

  // TODO: set the temperature based on material selected
  sendLine("M100({he1st:195})");
  sendLine("G92.1 X0 Y0 Z0 A0 B0");
  runJobStartGCode();

  return totalLines;
}

void Printr::runJobStartGCode() {
  _startGCodeFile = SD.open("/gc/start", FILE_READ);
  _lastSentProgramLine = 1;
  _processedProgramLine = 0;
  _currentAction = 1;
  _printing = true;
}

void Printr::runJobEndGCode() {

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

  if (!_printing)
    return;

  sendLine("M100({_leds:4})");

  _printFile.close();
  _printing = false;
  _lastSentProgramLine = 0;
  _processedProgramLine = 0;
  _totalProgramLines = 0;
  // turn off the hotend just in case
  turnOffHotend();

  if (_listener != nullptr)
    _listener->printrCallback("end", nullptr, nullptr);
}

void Printr::sendLine(String line) {
  Serial1.println(line);
}

void Printr::parseResponse() {

  char * line = readBuffer.line_buff;

  _sendNext = true;

  if (line[0] == '{') {
    StaticJsonBuffer<512> jsonBuffer;
    StaticJsonBuffer<512> rBuffer;

    JsonObject& o = jsonBuffer.parseObject(line);

    if (!o.success()) {
      // failed...
    } else {

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
            _listener->printrCallback("he1t", &_hotend1Temp, nullptr);
          }
        }

        if (_sr["line"]) {
          _sendNext = true;
          _processedProgramLine = _sr["line"];

          //_progress = (float) (_processedProgramLine / _totalProgramLines);
          if (_listener != NULL)
            _listener->printrCallback("line", nullptr, &_processedProgramLine);

        }
      }
    }
  }
}
