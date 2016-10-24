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

class Printr {
public:
	Printr();
  ~Printr();

  void init();
  void loop();
  void setListener(SceneController * listener) { _listener = listener;};
  void sendLine(String line, bool buffered=true);
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

private:
  void programEnd();
  void parseResponse();

  void runJobStartGCode();

  PrintrBuffer readBuffer;

  SceneController * _listener;
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
  Stream* _currentStream;
  int _linesToSend;
  int _firstChar;
  bool _newLine;

};


#endif
