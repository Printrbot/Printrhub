#include <EEPROM.h>
#include "config.h"
//#include "card.h"
#include "Arduino.h"


ConfigData Config::data = ConfigData();

void Config::save() {
/*
  this->net->debugMsg("Saving to eeprom");
  for (unsigned int i=0; i<sizeof(data); i++) {
      EEPROM.write(CONFIG_START + i, *((char*)&data + i));
      this->net->debugMsg(*((char*)&data + i));
  }
  EEPROM.commit();
  */
}

void Config::load() {
/*
  this->net->debugMsg("Loading from eeprom");
  for (unsigned int i=0; i<sizeof(data); i++) {
    *((char*)&data + i) = EEPROM.read(CONFIG_START + i);
    this->net->debugMsg(EEPROM.read(CONFIG_START + i));
  }

  this->net->debugMsg(data.wifiSsid);
  this->net->debugMsg(data.wifiPassword);
  */
}
