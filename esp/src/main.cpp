#include <SPI.h>

#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include "application.h"
#include "core/CommStack.h"
#include "core/Mode.h"


extern "C" {
  #include "user_interface.h"
}

//Net net;
//WebServer web;


void setup() {
  system_update_cpu_freq(160);
  Application.setup();
}


void loop() {
  Application.loop();
}
