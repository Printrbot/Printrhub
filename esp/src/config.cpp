#include <EEPROM.h>
#include "config.h"
//#include "card.h"
#include "Arduino.h"


ConfigData data = ConfigData();

#define EEPROM_OFFSET 0
#define EEPROM_SIZE 512

#define STORE_VAR(pos, value) write(pos, (uint8_t*)&value, sizeof(value))
#define READ_VAR(pos, value) read(pos, (uint8_t*)&value, sizeof(value))

Config::Config():
data(ConfigData()) {
  EEPROM.begin(EEPROM_SIZE);
  load();
}

void Config::load() {
  int i = EEPROM_OFFSET;
  READ_VAR(i, data.blank);
  READ_VAR(i, data.accessPoint);
  READ_VAR(i, data.name);
  READ_VAR(i, data.locked);
  READ_VAR(i, data.password);
  READ_VAR(i, data.wifiSsid);
  READ_VAR(i, data.wifiPassword);
  READ_VAR(i, data.jwt);
/*
  Serial.println("READING FROM EEPROM:");
  Serial.println(data.blank);
  Serial.println(data.name);
  Serial.println(data.locked);
  Serial.println(data.password);
  Serial.println(data.wifiSsid);
  Serial.println(data.wifiPassword);
  Serial.println(data.apPassword);
  Serial.println(data.jwt);
  */
}

void Config::save() {
  int i = EEPROM_OFFSET;
  STORE_VAR(i, data.blank);
  STORE_VAR(i, data.accessPoint);
  STORE_VAR(i, data.name);
  STORE_VAR(i, data.locked);
  STORE_VAR(i, data.password);
  STORE_VAR(i, data.wifiSsid);
  STORE_VAR(i, data.wifiPassword);
  STORE_VAR(i, data.jwt);
  EEPROM.commit();
}

void Config::clear()
{
  for (int i=EEPROM_OFFSET;i<EEPROM_OFFSET+EEPROM_SIZE;i++) {
    EEPROM.write(i,0);
  }

  //Setup default data
  data.blank = false;
  data.accessPoint = false;
  strcpy(data.name,"printrbot");
  data.locked = false;

  save();
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
