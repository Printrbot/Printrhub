#ifndef PRINTER_H
#define PRINTER_H

#define CMD_BUFF_SIZE 2
#define MAX_CMD_SIZE 96

#include "Arduino.h"
#include <ArduinoJson.h>
#include "net.h"
#include <SPI.h>
#include <SD.h>

struct PrinterInfo {
  char model[32];
  char serial[32];
  int bed_x;
  int bed_y;
  bool heated_bed;
};

struct PrintBuffer {
  char line_buff[CMD_BUFF_SIZE];
  char in_char;
  bool comment_mode;
  uint8_t line_idx;
  PrintBuffer() {
    comment_mode = false;
    line_idx = 0;
  }
};


class Printer {
  public:
    static void startPrint(String file);
    static bool printing;
    static bool paused;
    static void process();
    static void update();
    static void getInfo();
    static char cmd_buffer[CMD_BUFF_SIZE][MAX_CMD_SIZE];
    static PrintBuffer printBuffer;
    static uint8_t cmd_buff_read_idx;
    static uint8_t cmd_buff_write_idx;
    static uint8_t cmd_buff_len;
    static File printFile;
    static bool sendNext;
};

#endif
