/*
 * Updates the ESP firmware over the air (using WiFi)
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/behind-the-scenes-printrbot-simple-2016/
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Mick Balaban
 * https://github.com/Printrbot/Printrhub
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include <EEPROM.h>
#include "config.h"
//#include "card.h"
#include "Arduino.h"

ConfigData data = ConfigData();

#define EEPROM_OFFSET 0
#define EEPROM_SIZE 512

#define STORE_VAR(pos, value) write(pos, (uint8_t*)&value, sizeof(value))
#define READ_VAR(pos, value) read(pos, (uint8_t*)&value, sizeof(value))

Config::Config() :
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

void Config::clear() {
  for (int i = EEPROM_OFFSET; i < EEPROM_OFFSET + EEPROM_SIZE; i++) {
	EEPROM.write(i, 0);
  }

  //Setup default data
  data.blank = false;
  data.accessPoint = false;
  strcpy(data.name, "printrbot");
  data.locked = false;

  save();
}

void Config::write(int &pos, uint8_t *value, uint8_t size) {
  for (uint8_t i = 0; i < size; i++) {
	EEPROM.write(pos, *value);
	pos++;
	value++;
	delay(20);
  }
}

void Config::read(int &pos, uint8_t *value, uint8_t size) {
  for (uint8_t i = 0; i < size; i++) {
	*value = EEPROM.read(pos);
	pos++;
	value++;
  }
}
