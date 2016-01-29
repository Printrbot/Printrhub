#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include "card.h"
#include "config.h"
#include "net.h"
#include "controller.h"
#include "printer.h"
#include <Wire.h>
#include "macros.h"


void setup() {

  Serial.begin(115200);
  Card::init();

  if (Card::ok) {
    // check if there is a config.txt config file available
    if (Card::hasKeyFile) {
      Card::updateConfigFromSd();
    } else {
      DEBUG("error: no key file found, aborting!");
      while (1) { delay(1000); };
    }
  } else {
    DEBUG("error: no SD card found, aborting!");
    while (1) { delay(1000); };
  }

  // try to connect to wifi
  int ret = 0;
  if (Net::connectWifi()) {
    // if connected, try connecting to server
    while (!Net::connectServer()) {
      ret++;
      if (ret > Net::serverRetry) {
        DEBUG("Unable to connect to server. aborting!");
        while (1) { delay(1000); };
      }
    }
  } else {
    // unable to connect to wifi, reboot
    delay(2000);
    ESP.restart();
  }
}

char inbuff[MAX_CMD_SIZE];
int readIdx = 0;

void loop() {

  if (Net::serverConnected)
    Net::read();

  // process commands from the print buffer
  Printer::process();

  // read serial
  while (Serial.available()) {
//    if (Printer::printing) {
      inbuff[readIdx] = Serial.read();
      if (inbuff[readIdx] == '\n') {
        Controller::fromPrinter(inbuff);
        readIdx = 0;
      }
      else
        readIdx++;
  }

}
