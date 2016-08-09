#include <EEPROM.h>
#include "config.h"
//#include "card.h"
#include "Arduino.h"


ConfigData Config::data = ConfigData();

#define EEPROM_OFFSET 0

#define STORE_VAR(pos, value) Config::write(pos, (uint8_t*)&value, sizeof(value))
#define READ_VAR(pos, value) Config::read(pos, (uint8_t*)&value, sizeof(value))

void Config::load() {
  int i = EEPROM_OFFSET;
  READ_VAR(i, Config::data.blank);
  READ_VAR(i, Config::data.accessPoint);
  READ_VAR(i, Config::data.name);
  READ_VAR(i, Config::data.locked);
  READ_VAR(i, Config::data.password);
  READ_VAR(i, Config::data.wifiSsid);
  READ_VAR(i, Config::data.wifiPassword);
  READ_VAR(i, Config::data.jwt);
/*
  Serial.println("READING FROM EEPROM:");
  Serial.println(Config::data.blank);
  Serial.println(Config::data.name);
  Serial.println(Config::data.locked);
  Serial.println(Config::data.password);
  Serial.println(Config::data.wifiSsid);
  Serial.println(Config::data.wifiPassword);
  Serial.println(Config::data.apPassword);
  Serial.println(Config::data.jwt);
  */
}

void Config::save() {

  int i = EEPROM_OFFSET;
  STORE_VAR(i, Config::data.blank);
  STORE_VAR(i, Config::data.accessPoint);
  STORE_VAR(i, Config::data.name);
  STORE_VAR(i, Config::data.locked);
  STORE_VAR(i, Config::data.password);
  STORE_VAR(i, Config::data.wifiSsid);
  STORE_VAR(i, Config::data.wifiPassword);
  STORE_VAR(i, Config::data.jwt);
  EEPROM.commit();

  ESP.restart();
}

void Config::write(int &pos, uint8_t* value, uint8_t size) {
  for (uint8_t i=0; i<size; i++) {
    EEPROM.write(pos, *value);
    pos++;
    value++;
    delay(20);
  }
}

void Config::read(int &pos, uint8_t* value, uint8_t size) {
  for (uint8_t i=0; i<size; i++) {
    *value = EEPROM.read(pos);
    pos++;
    value++;
  }
}
