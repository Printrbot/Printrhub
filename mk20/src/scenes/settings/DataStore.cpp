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