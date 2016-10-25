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
_progress(0.0),
_processedProgramLine(0),
_currentAction(0)
{
  _setupCode = new MemoryStream(64);
  _lineBuffer = new MemoryStream(20);
  _currentStream = _setupCode;
  _firstChar = true;
  _newLine = true;
}

Printr::~Printr() {
  delete _setupCode;
  delete _lineBuffer;
}

void Printr::init() {
  Serial1.begin(115200);
  Serial1.attachCts(CTS_PIN);
  Serial1.attachRts(RTS_PIN);

  reset();
}

void Printr::reset() {
  //Start by sending in 4 lines before waiting for a response
  _printing = false;
  _setupCode->flush();
  _currentStream = _setupCode;

  _linesToSend = 0;

  sendLine("{sr:{line:t,he1t:t,he1st:t,he1at:t,stat:t}}");
  sendLine("{_leds:4}");
  //sendLine("M100({_leds:4})",false); //switch to blue light
  sendLine("{sv:1}");
}

void Printr::loop() {
  processPrint();
}

void Printr::processPrint() {
  if (_linesToSend > 0) {
    //Let's check the outgoing serial buffer first, can we send?
    int numBytesAvailableToWrite = Serial1.availableForWrite();
    if (numBytesAvailableToWrite > 0) {
      //We can send data to the printer
      digitalWrite(COMMSTACK_DATALOSS_MARKER_PIN,LOW);
      uint8_t buffer[numBytesAvailableToWrite];
      size_t bufferLength = 0;

      //Read the current input stream (setup code buffer or file) and read as long as we have filled the send buffer or until we are at the end of the line
      while (bufferLength < numBytesAvailableToWrite) {
        int byte = 0;
        if (_lineBuffer->available()) {
          byte = _lineBuffer->read();
        } else {
          byte = _currentStream->read();
        }

        if (byte < 0) {
          //Only switch to print file if we are printing
          if (_printing) {
            //We are finished with the current stream, switch to next
            if (_currentStream == _setupCode) {
              PRINTER_NOTICE("Setup code complete, now printing file");

              //Free memory used by setup buffer and clear its content
              _setupCode->flush();

              //Switch to file as input stream
              _currentStream = &_printFile;

              _newLine = true;
              _lastSentProgramLine = 1;
            } else {
              PRINTER_NOTICE("File complete, finishing print");
              // finished reading the file
              // This is a hack until total line numbers
              // is implemented, then program will end
              // when M3 is passed to it, or when _processedProgramLine == _totalProgramLines
              programEnd();
            }
          } else {
            //Nothing to read in setup code

            break;
          }
        } else {
          buffer[bufferLength++] = byte;
          if (byte == '\n') {
            //This is the last byte of the current line and buffer now contains the last part of the current line, so we break out here now
            _newLine = true;
            break;
          } else if (_newLine) {
            _firstChar = byte;
            _newLine = false;
          }
        }
      }

      //Send the buffer to the printer
      if (bufferLength > 1) {
        Serial1.write(buffer,bufferLength);
        DebugSerial.print("Sending buffer: ");
        DebugSerial.write(buffer,bufferLength);

        if (_newLine) {
          DebugSerial.println("Line complete");
          _linesToSend--;

          if (_currentStream == &_printFile) {
            //Add line numbers when printing G-Codes from file
            _lastSentProgramLine++;

            //Write the line info in the line buffer
            _lineBuffer->flush();
            _lineBuffer->print("N");
            _lineBuffer->print(_lastSentProgramLine);
            _lineBuffer->print(" ");
          }
        }
      }

      digitalWrite(COMMSTACK_DATALOSS_MARKER_PIN,HIGH);
    }
  }

  //Read from Serial
  if (Serial1.available()) {
    digitalWrite(CODE_INDICATOR_1,LOW);
    while (Serial1.available()) {
      readBuffer.line_buff[readBuffer.line_idx] = Serial1.read();
      if (readBuffer.line_buff[readBuffer.line_idx] == '\n') {
        readBuffer.line_buff[readBuffer.line_idx + 1] = '\0';
        parseResponse();
        readBuffer.line_idx = 0;
        readBuffer.line_buff[0] = '\0';

        //We want to exit to loop once we read a line
        break;
      }
      else
        readBuffer.line_idx++;
    }
    digitalWrite(CODE_INDICATOR_1,HIGH);
  }

}

void Printr::turnOffHotend() {
  sendLine("M100({he1st:0})");
}

void Printr::stopAndFlush() {
  sendLine("!%");
}

int Printr::startJob(String filePath) {
  PRINTER_NOTICE("Printing file: %s",filePath.c_str());
  _printFile = SD.open(filePath.c_str(), FILE_READ);

  _totalProgramLines = -1;

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
      _totalProgramLines= h["lines"];
      _totalPrintTime = h["time"];
      const char * ptr = h["readable"];
      _printTimeReadable = String(ptr);
      _printVolume = h["volume"];
      _printFilamentLength = h["filament"];
      _printSupport = h["support"];
      _printBrim = h["brim"];
      const char * res = h["resolution"];
      _printResolution = String(res);
      const char * infill = h["infill"];
      _printInfill = String(infill);
    }
  }

  //Setup printer and run the file
  runJobStartGCode();

  return _totalProgramLines;
}

void Printr::runJobStartGCode() {
  _printing = true;

  _lastSentProgramLine = 1;
  _processedProgramLine = 0;

  // TODO: set the temperature based on material selected
  sendLine("M100({he1st:195})");
  sendLine("G92.1 X0 Y0 Z0 A0 B0");

  // reset all
  sendLine("G28.2 X0 Y0");
  sendLine("G0 X110");
  sendLine("M100({_leds:2})");
  sendLine("M101 ({he1at:t})");
  sendLine("M100({_leds:3})");
  sendLine("G28.2 Z0");
  sendLine("G0 X0 Y145 Z6");
  sendLine("G38.2 Z-10 F200");
  sendLine("G0 Z5");
  sendLine("M100({_leds:5})");
  sendLine("G0 X210 Y65");
  sendLine("G38.2 Z-10 F200");
  sendLine("G0 Z5");
  sendLine("M100({_leds:6})");
  sendLine("G0 X0 Y10");
  sendLine("G38.2 Z-10 F200");
  sendLine("G0 Z5");
  sendLine("M100({_leds:3})");
  sendLine("M100 ({tram:1})");
  sendLine("G92 A0");

  // switch to white light
  sendLine("M100({_leds:1})");
  sendLine("G0 Z5");

  // apply hotend offset
  float headOffset = 5.0 - dataStore.getHeadOffset();
  String gco = String("G92 Z") + String(headOffset);
  sendLine(gco);

  // clean the nozzle
  sendLine("G0 X0 Y0 Z0.3");
  sendLine("G1 X220.000 A10 F1200");
  sendLine("G0 Z1");
  sendLine("G92 A0");

  //Make sure we start with startup buffer
  _currentStream = _setupCode;
}


void Printr::cancelCurrentJob() {
  stopAndFlush();
  programEnd();
  sendLine("G0 X110 Y150");
  sendLine("M100({_leds:4})"); //switch to blue light
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
  sendLine("G28.2 Z0",false);
  _homeZ = true;
}

void Printr::programEnd() {

  if (!_printing)
    return;

  //sendLine("M100({_leds:4})");

  //Reset the printer and prepare memory buffers
  reset();

  _printFile.close();
  _printing = false;
  _lastSentProgramLine = 0;
  _processedProgramLine = 0;
  _totalProgramLines = 0;
  // turn off the hotend just in case
  turnOffHotend();

  if (_listener != nullptr) {
    _listener->onPrintComplete(true);
  }
}

void Printr::sendLine(String line, bool buffered) {
  if (buffered) {
    _setupCode->println(line);
  } else {
    Serial1.println(line);
    _sendNext = false;
  }
}

void Printr::parseResponse() {

  char * line = readBuffer.line_buff;

  PRINTER_SPAM("Received line: %s",line);

  if (line[0] == '{') {
    StaticJsonBuffer<512> jsonBuffer;
    StaticJsonBuffer<512> rBuffer;
    StaticJsonBuffer<512> fBuffer;

    JsonObject& o = jsonBuffer.parseObject(line);

    if (!o.success()) {
      // failed...
      PRINTER_ERROR("Could not parse printer response: %s",line);
      digitalWrite(CODE_INDICATOR_2,LOW);
      delayMicroseconds(5);
      digitalWrite(CODE_INDICATOR_2,HIGH);

      //Make sure we proceed sending data so we don't stall
      _sendNext = true;
    } else {
      // parse sr
      String sr = o["sr"];
      String r = o["r"];
      String f = o["f"];

      if (sr.length() > 0) {
        PRINTER_SPAM("Got a sr message");
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
            _listener->onNewNozzleTemperature(_hotend1Temp);
          }
        }

        if (_sr["line"]) {
          _sendNext = true;
          _processedProgramLine = _sr["line"];

          _progress = ((float)_processedProgramLine / (float)_totalProgramLines);
          if (_listener != NULL) {
            _listener->onPrintProgress(_progress);
          }
        }
      }

      if (r.length() > 0) {
        JsonObject& _r = rBuffer.parseObject(r);
        if (_r["n"]) {
          _processedProgramLine = _r["n"];
          _progress = ((float)_processedProgramLine / (float)_totalProgramLines);
          if (_listener != NULL) {
            _listener->onPrintProgress(_progress);
          }
        }

        PRINTER_SPAM("Got a r message, line-nr: %d, progress: %d",_processedProgramLine,(int)(_progress*100.0f));
      }

      if (f.length() > 0) {
        JsonArray& _f = fBuffer.parseArray(f);
        uint8_t lineBufferSpace = 0;
        uint8_t statusCode = 0;
        if (_f.size() > 0) {
          lineBufferSpace = _f[2];
          statusCode = _f[1];
          if (statusCode == 0) {
            if (_linesToSend > lineBufferSpace) {
              _linesToSend = lineBufferSpace;
            } else {
              _linesToSend++;
            }
          }
          PRINTER_SPAM("Got status: %s, Status-Code: %d, Available line buffer: %d, Lines to send: %d",f.c_str(),statusCode,lineBufferSpace,_linesToSend);
        } else {
          _linesToSend++;
          PRINTER_ERROR("Got status array, but could not parse it: %s",f.c_str());
        }
      }
    }
  }
}
