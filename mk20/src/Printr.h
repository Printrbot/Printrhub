/*
 * Handles the communication and state handling with the g2 printer board. It needs to
 * handle this communication without blocking the main loop. This results in a somewhat
 * more complicated code. It's also important that g2 gets new GCodes all the time, so
 * timing is important
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/behind-the-scenes-printrbot-simple-2016/
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Mick Balaban, Phillip Schuster
 * https://github.com/Printrbot/Printrhub
 *
 * Developed in cooperation with Phillip Schuster (@appfruits) from appfruits.com
 * http://www.appfruits.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef PRINTR_H
#define PRINTR_H

#include <Arduino.h>
#include "SD.h"
#include "framework/core/SceneController.h"
#include "framework/core/MemoryStream.h"

struct PrintrBuffer {
  char line_buff[512];
  uint8_t line_idx;
  PrintrBuffer() {
	line_idx = 0;
  }
};

#define PRINTR_STATUS_OK 0
#define PRINTR_STATUS_INITIALIZING 15

#define PRINTR_STAT_INITIALIZING 0
#define PRINTR_STAT_READY 1
#define PRINTR_STAT_ALARM 2
#define PRINTR_STAT_PROGRAM_STOP 3
#define PRINTR_STAT_PROGRAM_END 4

class PrintrListener {
 public:
  virtual void onNewNozzleTemperature(float temp) = 0;
  virtual void onPrintProgress(float progress) = 0;
  virtual void onPrintComplete(bool success) = 0;
};

enum class PrintrMode : uint8_t {
  ImmediateMode = 0,
  PrintMode = 1
};

class Printr {
 public:
  Printr();
  ~Printr();

  void init();
  void loop();
  void setListener(PrintrListener *listener) { _listener = listener; };
  void sendLine(String line);
  void sendWaitCommand(int millis);
  void stopAndFlush();
  void turnOffHotend();
  void startListening();
  void stopListening();

  int startJob(String filePath);
  void cancelCurrentJob();
  bool isHomed() { return _homeX && _homeY && _homeZ; };
  bool isPrinting() { return _printing; };
  int getTotalJobLines() { return _totalProgramLines; };
  String getResolution() { return _printResolution; };
  String getInfill() { return _printInfill; };
  String getFilamentLength() { return String(String(_printFilamentLength) + String("mm")); };
  String getSupport() { return _printSupport ? String("Yes") : String("No"); };
  String getPrintTime() { return _printTimeReadable; }

  void reset();

  void homeX();
  void homeY();
  void homeZ();

  void processPrint();
  void sendCommands();
  void readResponses();
  bool queryCurrentLine(Stream *stream, int lineNumber = 0);
  void handlePBCode(const char *pbcode);

  void turnLightOn();
  void turnLightOff();
  void setLightColor(int colorId);
  bool isLightOn() { return _lightOn; };
  int getLightColor() { return _lightColor; };

 private:
  void programEnd(bool success);
  void parseResponse();

  void runJobStartGCode();

  PrintrBuffer readBuffer;

  PrintrListener *_listener;
  float _hotend1Temp;
  float _bedTemp;
  int _stat;
  int _currentAction;
  int _currentProgramLine;
  int _processedProgramLine;

  int _totalProgramLines;
  int _totalPrintTime;
  int _printVolume;
  int _printFilamentLength;
  bool _printSupport;
  bool _printBrim;
  String _printResolution;
  String _printInfill;
  String _printTimeReadable;

  int _lastSentProgramLine;
  float _progress;
  bool _sendNext;
  bool _printing;
  File _printFile;
  bool _homeX;
  bool _homeY;
  bool _homeZ;
  MemoryStream *_setupCode;
  int _linesToSend;

  MemoryStream *_currentLineBuffer;
  int _printrCurrentStatus;
  int _printrBufferSize;
  PrintrMode _currentMode;
  bool _lineSent;

  bool _waiting;
  unsigned long _waitStart;
  int _waitDuration;

  bool _lightOn;
  int _lightColor;

};

#endif
