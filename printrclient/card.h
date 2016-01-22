#ifndef SDCARD_H
#define SDCARD_H

#include <SPI.h>
#include <SD.h>
#include "config.h"

class Net;

class Card {
public:
  static void init();
  static void updateConfigFromSd();
  static void listFiles();
  static bool ok;
  static bool hasKeyFile;
  static void downloadFile(String short_id, String url);
};

#endif
