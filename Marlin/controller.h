#ifndef CONTROLLER_H
#define CONTROLLER

#include "Arduino.h"
#include "pbserial.h"
#include "gcode.h"

struct devBuffer {
  char line_buff[MAX_CMD_SIZE];
  char in_char;
  bool comment_mode;
  uint8_t line_idx;
  devBuffer() {
    comment_mode = false;
    line_idx = 0;
  }
};


class Controller {
public:
  Controller();
  void update();
  void process();


private:
  void processChar(devBuffer &b);
  char cmd_buffer[CMD_BUFF_SIZE][MAX_CMD_SIZE];
  uint8_t cmd_buff_read_idx;
  uint8_t cmd_buff_write_idx;
  uint8_t cmd_buff_len;
  Gcode gcode;
  devBuffer serPort1;
  devBuffer serPort2;
  void runCommand();
  void runGcode();
  void runMcode();
  void runTcode();
  void prepareRapidMove();
  void prepareLinearMove();
  void prepareArcMove(char isclockwise);
};


#endif
