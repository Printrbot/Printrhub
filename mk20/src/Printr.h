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

enum class PrintrMode: uint8_t {
    ImmediateMode = 0,
    PrintMode = 1
};

class Printr {
public:
	Printr();
  ~Printr();

  void init();
  void loop();
  void setListener(PrintrListener * listener) { _listener = listener;};
  void sendLine(String line);
  void sendWaitCommand(int millis);
  void stopAndFlush();
  void turnOffHotend();

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
  bool queryCurrentLine(Stream* stream, int lineNumber=0);
  void handlePBCode(const char* pbcode);

private:
  void programEnd(bool success);
  void parseResponse();

  void runJobStartGCode();

  PrintrBuffer readBuffer;

  PrintrListener * _listener;
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

};


#endif
