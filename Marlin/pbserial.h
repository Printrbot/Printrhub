#ifndef PBSERIAL_H
#define PBSERIAL_H

#include "Arduino.h"
#include "Configuration.h"
#include "HardwareSerial.h"

extern HardwareSerial Serial1;

class PBSerial {
public:

  PBSerial();
  ~PBSerial();

  static void init();

  static void flush();
  static void printf(char * s, ...);
  static void printf(const __FlashStringHelper *format,...);
  static void requestResend(int last_line);

  static void print(const char* x);
  static void println(int x);
  static void println(const char* x);
  static void log(const char* x);
  static void error(const char* x);
};

#endif
