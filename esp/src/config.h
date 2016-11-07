#ifndef CONFIG_H
#define CONFIG_H

#include "Arduino.h"

#define FIRMWARE_VERSION "0.13"
#define FIRMWARE_BUILDNR 106

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
  Config();
  ConfigData data;
  void save();
  void load();
  void clear();
private:
  void write(int &pos, uint8_t* value, uint8_t size);
  void read(int &pos, uint8_t* value, uint8_t size);
};

#endif
