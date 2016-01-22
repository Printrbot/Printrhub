#include "pbserial.h"
#include "HardwareSerial.h"
#include <Arduino.h>
#include <stdarg.h>
#include "Configuration.h"

PBSerial::PBSerial() {
};

void PBSerial::init() {
  Serial.begin(BAUDRATE1);
  Serial1.begin(BAUDRATE2);
};

void PBSerial::flush() {
  Serial.flush();
  Serial1.flush();
}

void PBSerial::print(const char* x) {
  Serial.print(x);
  Serial1.print(x);
};

void PBSerial::printf(char *format, ...)
{
  char buff[256];
  va_list args;
  va_start(args,format);
  vsnprintf(buff,256,format,args);
  va_end (args);
  Serial.print(buff);
  Serial1.print(buff);
}

void PBSerial::printf(const __FlashStringHelper *format,...)
{
  char buff[256];
  va_list ap;
  va_start(ap,format);
  vsnprintf_P(buff, 256, (const char *)format, ap);
  va_end (ap);
  Serial.print(buff);
  Serial1.print(buff);
}

void PBSerial::println(int x) {
  Serial.println(x);
  Serial1.println(x);
};

void PBSerial::println(const char* x) {
  Serial.println(x);
  Serial1.println(x);
};

void PBSerial::log(const char* x) {
  Serial.print(";");
  Serial.println(x);
};

void PBSerial::error(const char* x) {
  Serial.print(";");
  Serial.println(x);
};

void PBSerial::requestResend(int last_line) {
  // TODO
}
