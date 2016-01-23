#ifndef STORAGE_H
#define STORAGE_H

#include "Configuration.h"

class Storage {
public:
  Storage();
  static void loadDefaults();
  static void writeSettings();
  static void readSettings();
  static void writeSerial(char *sn);
  static void readSerial();
private:
  static void write(int &pos, uint8_t* value, uint8_t size);
  static void read(int &pos, uint8_t* value, uint8_t size);
};

#endif
