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
  _waiting = false;
  _waitStart = 0;

  _printrCurrentStatus = PRINTR_STATUS_INITIALIZING;
  _currentMode == PrintrMode::ImmediateMode;
  _lineSent = true;
  _currentLineBuffer = new MemoryStream(255);
}

Printr::~Printr() {
  delete _setupCode;
  delete _currentLineBuffer;
}

void Printr::init() {
  Serial1.begin(115200);
  Serial1.attachCts(CTS_PIN);
  Serial1.attachRts(RTS_PIN);

  reset();

  //When we init we wait for the printer to send the first status response
  _linesToSend = 4;
  sendLine("{sr:{line:t,he1t:t,he1st:t,he1at:t,stat:t}}");
  sendLine("{_leds:4}");
  sendWaitCommand(500);
  sendLine("{_leds:1}");
  sendWaitCommand(500);
  sendLine("{_leds:2}");
  sendWaitCommand(500);
  sendLine("{_leds:3}");
  sendWaitCommand(500);
  sendLine("{_leds:4}");
  //sendLine("M100({_leds:4})",false); //switch to blue light
  sendLine("{sv:1}");

}

void Printr::reset() {
  _currentMode = PrintrMode::ImmediateMode;
  _printing = false;
  _setupCode->flush();

  _linesToSend = 4;
}

void Printr::loop() {

  if (_printrCurrentStatus == PRINTR_STATUS_INITIALIZING) {
    //We only read and wait for status to become OK
    readResponses();
  } else if (_printrCurrentStatus == PRINTR_STATUS_OK) {
    //First Send Commands
    sendCommands();
    //Then read responses
    readResponses();
  } else {
    //Any other status is considered an error

  }
}

bool Printr::queryCurrentLine(Stream* stream, int lineNumber) {
  if (!stream->available()) {
    //Nothing to read from the stream
    return false;
  }

  //Add a line number if necessary
  if (lineNumber > 0) {
    _currentLineBuffer->print("N");
    _currentLineBuffer->print(lineNumber);
    _currentLineBuffer->print(" ");
  }

  //Now try to read the line
  bool lineRead = false;
  while (stream->available() > 0) {
    int byte = stream->read();
    if (byte < 0) {
      //Stream is EOF
      break;
    } else {
      _currentLineBuffer->write(byte);
      if (byte == '\n') {
        //This is the end of the line
        lineRead = true;
        break;
      }
    }
  }

  //We did not read a line, so flush the current line buffer
  if (!lineRead) {
    _currentLineBuffer->flush();
  } else {
    //Mark this line to not been sent yet
    _lineSent = false;
  }

  return lineRead;
}

void Printr::handlePBCode(const char* pbcode) {
  String code(pbcode);
  if (code.startsWith(";PBCODE;wait;")) {
    String durationString = code.replace(";PBCODE;wait;","");
    int duration = durationString.toInt();
    PRINTER_SPAM("Got a wait command: %d",duration);

    _waiting = true;
    _waitStart = millis();
    _waitDuration = duration;
  }
}

void Printr::sendCommands() {
  if (_linesToSend <= 0) {
    return;
  }

  //We had a wait command, let's wait if necessary
  if (_waiting) {
    if ((millis() - _waitStart) > _waitDuration) {
      _waiting = false;
    } else {
      return;
    }
  }

  if (_lineSent) {
    //The current line has been sent, get a new one
    if (_currentMode == PrintrMode::ImmediateMode) {
      //Only send commands from the buffer
      if (queryCurrentLine(_setupCode)) {
        PRINTER_SPAM("Immediate-Mode: Queried new line from setupCode: %s",_currentLineBuffer->c_str());
      }
    } else if (_currentMode == PrintrMode::PrintMode) {
      //If we are in print mode, we work through the setup buffer, after that we switch to the file
      if (queryCurrentLine(_setupCode)) {
        PRINTER_SPAM("Print-Mode: Queried new line from setupCode: %s",_currentLineBuffer->c_str());
      } else {
        //Setup buffer has been sent, switch to file
        if (_printFile) {
          if (queryCurrentLine(&_printFile,_lastSentProgramLine)) {
            PRINTER_SPAM("Print-Mode: Queried new line from print file: %s",_currentLineBuffer->c_str());
            _lastSentProgramLine++;
          }
        }
      }
    }
  } else {
    //Line has not been sent yet, try to send it now
    if (_currentLineBuffer->available() <= 0) {
      _lineSent = true;
    } else if (_currentLineBuffer->peek() == ';') {
      if (strncmp(";PBCODE;", _currentLineBuffer->c_str(), strlen(";PBCODE;")) == 0) {
        //We got a PB code, do something about it
        PRINTER_SPAM("Got a PBCODE: %s", _currentLineBuffer->c_str());
        handlePBCode(_currentLineBuffer->c_str());
      } else {
        //This is a comment, skip that
        PRINTER_SPAM("Skipped comment: %s", _currentLineBuffer->c_str());
      }

      _lineSent = true;
      _currentLineBuffer->flush();
    } else if (_currentLineBuffer->peek() == '\n') {
      PRINTER_SPAM("Just got a newline");
      _lineSent = true;
      _currentLineBuffer->flush();
    } else {
      //This line is ok, send it to the printer if enough space is in the buffer
      while (true) {
        if (Serial1.availableForWrite() <= 0) {
          //We cannot send anything now, break out
          break;
        } else {
          int byte = _currentLineBuffer->read();
          if (byte < 0) {
            //This line has been sent completely
            _lineSent = true;
            _currentLineBuffer->flush();
            _linesToSend--;
            PRINTER_SPAM("Line sent, lines left to send: %d",_linesToSend);
            break;
          } else {
            //Write the byte we just read to the printr
            Serial1.write(byte);
          }
        }
      }
    }
  }
}

void Printr::readResponses() {
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
  _progress = 0.0;

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
  _currentMode = PrintrMode::PrintMode;
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
}


void Printr::cancelCurrentJob() {
  _currentMode = PrintrMode::ImmediateMode;
  _currentLineBuffer->flush();

  stopAndFlush();
  sendWaitCommand(1000);
  reset();
  turnOffHotend();
  sendLine("M100({_leds:4})"); //switch to blue light
  sendLine("G91");             //Relative mode
  sendLine("G0 Z10");          //Move up
  sendLine("G90");             //Back to absolute mode
  sendLine("G0 X110 Y150");    //Home back with bed centered

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

void Printr::programEnd(bool success) {

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
    _listener->onPrintComplete(success);
  }
}

void Printr::sendLine(String line) {
  if (_setupCode->println(line) <= 0) {
    PRINTER_ERROR("Could not send line: %s",line.c_str());
  }
}

void Printr::sendWaitCommand(int millis) {
  String line(";PBCODE;wait;");
  line = line + millis;
  sendLine(line);
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
      // Parse status response
      String sr = o["sr"];
      String r = o["r"];
      String f = o["f"];

      if (sr.length() > 0) {
        JsonObject& _sr = rBuffer.parseObject(sr);
        // {sr: {stat:0}}
        // https://github.com/synthetos/TinyG/wiki/TinyG-Status-Codes#status-report-enumerations
        if (_sr["stat"]) {
          _stat = _sr["stat"];
          switch(_stat) {
            case PRINTR_STAT_ALARM:
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

            case PRINTR_STAT_PROGRAM_STOP:
              programEnd(false);
              _totalProgramLines = _lastSentProgramLine;
              PRINTER_NOTICE("Printer got stopped, closing");
              break;

            case PRINTR_STAT_PROGRAM_END:
              // program end via M2, M30
              // finish print if printing
              PRINTER_NOTICE("Printer finished, closing");
              programEnd(true);
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

      //Parse line response
      if (r.length() > 0) {
        JsonObject& _r = rBuffer.parseObject(r);
        if (_r["n"]) {
          _processedProgramLine = _r["n"];
          _progress = ((float)_processedProgramLine / (float)_totalProgramLines);
          if (_listener != NULL) {
            _listener->onPrintProgress(_progress);
          }
        }

        //We got a r-response, so increment number of lines that can be sent
        _linesToSend++;

        PRINTER_SPAM("Got a r message, line-nr: %d, progress: %d",_processedProgramLine,(int)(_progress*100.0f));
      }

      //Parse status
      if (f.length() > 0) {
        JsonArray& _f = fBuffer.parseArray(f);
        if (_f.size() <= 0) {
          //Parsing failed
          PRINTER_ERROR("Got status array, but could not parse it: %s",f.c_str());
        } else {
          //Parsing successful, save values
          _printrCurrentStatus = _f[1];
          _printrBufferSize = _f[2];
          PRINTER_SPAM("Got status: %s, Status-Code: %d, Available line buffer: %d, Lines to send: %d",f.c_str(),_printrCurrentStatus,_printrBufferSize,_linesToSend);
        }
      }
    }
  }
}
