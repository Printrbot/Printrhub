#ifndef CONFIG_H
#define CONFIG_H

#include "Arduino.h"

#define FIRMWARE_VERSION "0.11"

struct ConfigData {
  bool blank;
  bool accessPoint;
  char name[16];
  bool locked;
  char password[50];
  char wifiSsid[50];
  char wifiPassword[50];
  char apPassword[50];
  char jwt[500];
};

class Config {
public:
  static ConfigData data;
  static void save();
  static void load();
private:
  static int addr;
  static void write(int &pos, uint8_t* value, uint8_t size);
  static void read(int &pos, uint8_t* value, uint8_t size);
};

#endif
