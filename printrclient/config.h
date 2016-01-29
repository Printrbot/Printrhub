#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include "Arduino.h"
#define CONFIG_START 0
#define FIRMWARE_VERSION 0.2

struct ConfigData {
  char server[16];
  char port[8];
  char wifiSsid[50];
  char wifiPassword[50];
  char jwt[500];
};

class Config {
public:
  static ConfigData data;
  static void save();
  static void load();
private:
  static int addr;
};

#endif
