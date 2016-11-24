/*
 * Data store to save various settings
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/behind-the-scenes-printrbot-simple-2016/
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Mick Balaban
 * https://github.com/Printrbot/Printrhub
 *
 * Developed in cooperation with Phillip Schuster (@appfruits) from appfruits.com
 * http://www.appfruits.com
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

#include "DataStore.h"
#include <EEPROM.h>
#include <math.h>

DataStore::DataStore() {
  // read the data from eeprom
  EEPROM.get(0, _data);
}

DataStore::~DataStore() {

}

void DataStore::save() {
  EEPROM.put(0, _data);
}

void DataStore::setHeadOffset(float val) {
  _data.headOffset = val;
};

float DataStore::getHeadOffset() {

  if (isnan(_data.headOffset))
	return 0;

  return _data.headOffset;
};

void DataStore::setLoadedMaterial(Material material) {
  _data.material = material;
}

Material *DataStore::getLoadedMaterial() {
  // check if we have valid data

  if (isnan(_data.material.temperature)) {
	_data.material.temperature = 200;
  }

  return &_data.material;
}