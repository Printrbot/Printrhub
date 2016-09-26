#ifndef PRINTR_H
#define PRINTR_H

#include <Arduino.h>
#include "SD.h"
#include "framework/core/SceneController.h"

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
  void sendLine(String line);
  void stopAndFlush();
  void turnOffHotend();

  void startJob(String filePath);
  bool isHomed() { return _homeX && _homeY && _homeZ; };

  void homeX();
  void homeY();
  void homeZ();

private:
  void programEnd();
  void readSerial();
  void parseResponse();

  void runJobStartGCode();
  void runJobEndGCode();


  PrintrBuffer readBuffer;

  SceneController * _listener;
  float _hotend1Temp;
  float _bedTemp;
  int _stat;
  int _currentAction;
  int _currentProgramLine;
  int _processedProgramLine;
  int _totalProgramLines;
  int _lastSentProgramLine;
  float _progress;
  bool _sendNext;
  bool _printing;
  File _printFile;
  File _startGCodeFile;
  File _endGCodeFile;
  bool _homeX;
  bool _homeY;
  bool _homeZ;

};


#endif
