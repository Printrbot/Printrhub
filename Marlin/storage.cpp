#include <EEPROM.h>
#include "storage.h"
#include "Configuration_adv.h"
#include "planner.h"
#include "temperature.h"

#define EEPROM_OFFSET 200

#define STORE_VAR(pos, value) Storage::write(pos, (uint8_t*)&value, sizeof(value))
#define READ_VAR(pos, value) Storage::read(pos, (uint8_t*)&value, sizeof(value))

Storage::Storage() {};

void Storage::loadDefaults() {
  Serial.println("LOADING DEFAULTS");
  float spu[] = DEFAULT_AXIS_STEPS_PER_UNIT;
  float mf[] = DEFAULT_MAX_FEEDRATE;
  long ma[] = DEFAULT_MAX_ACCELERATION;
  for (uint8_t i = 0; i<4; i++) {
      axis_steps_per_unit[i] = spu[i];
      max_feedrate[i] = mf[i];
      max_acceleration_units_per_sq_second[i] = ma[i];
  }
  acceleration = DEFAULT_ACCELERATION;
  retract_acceleration = DEFAULT_RETRACT_ACCELERATION;
  minimumfeedrate = DEFAULT_MINIMUMFEEDRATE;
  minsegmenttime = DEFAULT_MINSEGMENTTIME;
  mintravelfeedrate = DEFAULT_MINTRAVELFEEDRATE;
  max_xy_jerk = DEFAULT_XYJERK;
  max_z_jerk = DEFAULT_ZJERK;
  max_e_jerk = DEFAULT_EJERK;
  Printer::homing_offset[0] = 0;
  Printer::homing_offset[1] = 0;
  Printer::homing_offset[2] = 0;
  Printer::probe_offset[0] = X_PROBE_OFFSET_FROM_EXTRUDER_DEFAULT;
  Printer::probe_offset[1] = Y_PROBE_OFFSET_FROM_EXTRUDER_DEFAULT;
  Printer::probe_offset[2] = Z_PROBE_OFFSET_FROM_EXTRUDER_DEFAULT;
  Kp = DEFAULT_Kp;
  Ki = scalePID_i(DEFAULT_Ki);
  Kd = scalePID_d(DEFAULT_Kd);
  Printer::min_position[0] = X_MIN_POS;
  Printer::max_position[0] = X_MAX_POS;
  Printer::min_position[1] = Y_MIN_POS;
  Printer::max_position[1] = Y_MAX_POS;
  Printer::min_position[2] = Z_MIN_POS;
  Printer::max_position[2] = Z_MAX_POS;
  Serial.println("DONE LOADING DEFAULTS");
  updatePID();
  reset_acceleration_rates();
}

void Storage::writeSettings() {
  int i = EEPROM_OFFSET;
  char e_version[3] = EEPROM_VERSION;
  Serial.println("WRITING SETTINGS");
  STORE_VAR(i, e_version);
  STORE_VAR(i, axis_steps_per_unit);
  STORE_VAR(i, max_feedrate);
  STORE_VAR(i, max_acceleration_units_per_sq_second);
  STORE_VAR(i, acceleration);
  STORE_VAR(i, retract_acceleration);
  STORE_VAR(i, minimumfeedrate);
  STORE_VAR(i, mintravelfeedrate);
  STORE_VAR(i, minsegmenttime);
  STORE_VAR(i, max_xy_jerk);
  STORE_VAR(i, max_z_jerk);
  STORE_VAR(i, max_e_jerk);
  STORE_VAR(i, Printer::homing_offset);
  STORE_VAR(i, Printer::probe_offset[0]);
  STORE_VAR(i, Printer::probe_offset[1]);
  STORE_VAR(i, Printer::probe_offset[2]);
  STORE_VAR(i, Kp);
  STORE_VAR(i, Ki);
  STORE_VAR(i, Kd);
  STORE_VAR(i, Printer::min_position[0]);
  STORE_VAR(i, Printer::max_position[0]);
  STORE_VAR(i, Printer::min_position[1]);
  STORE_VAR(i, Printer::max_position[1]);
  STORE_VAR(i, Printer::min_position[2]);
  STORE_VAR(i, Printer::max_position[2]);
  STORE_VAR(i, Printer::homing_feedrate[0]);
  STORE_VAR(i, Printer::homing_feedrate[1]);
  STORE_VAR(i, Printer::homing_feedrate[2]);
}

void Storage::readSettings() {
  int i = EEPROM_OFFSET;
  char e_version[3];
  READ_VAR(i, e_version);

  if (strncmp(EEPROM_VERSION, e_version,3) == 0) {
    READ_VAR(i, axis_steps_per_unit);
    READ_VAR(i, max_feedrate);
    READ_VAR(i, max_acceleration_units_per_sq_second);
    READ_VAR(i, acceleration);
    READ_VAR(i, retract_acceleration);
    READ_VAR(i, minimumfeedrate);
    READ_VAR(i, mintravelfeedrate);
    READ_VAR(i, minsegmenttime);
    READ_VAR(i, max_xy_jerk);
    READ_VAR(i, max_z_jerk);
    READ_VAR(i, max_e_jerk);
    READ_VAR(i, Printer::homing_offset);
    READ_VAR(i, Printer::probe_offset[0]);
    READ_VAR(i, Printer::probe_offset[1]);
    READ_VAR(i, Printer::probe_offset[2]);
    READ_VAR(i, Kp);
    READ_VAR(i, Ki);
    READ_VAR(i, Kd);
    READ_VAR(i, Printer::min_position[0]);
    READ_VAR(i, Printer::max_position[0]);
    READ_VAR(i, Printer::min_position[1]);
    READ_VAR(i, Printer::max_position[1]);
    READ_VAR(i, Printer::min_position[2]);
    READ_VAR(i, Printer::max_position[2]);
    READ_VAR(i, Printer::homing_feedrate[0]);
    READ_VAR(i, Printer::homing_feedrate[1]);
    READ_VAR(i, Printer::homing_feedrate[2]);
    updatePID();
    reset_acceleration_rates();
  } else {
    Storage::loadDefaults();
  }
  // read serial number
  Storage::readSerial();
}

void Storage::writeSerial(char *sn) {
  for (uint8_t i=0; i<36; i++) {
    EEPROM.write(i, sn[i]);
    Printer::serial_number[i] = sn[i];
    delay(10);
  }
  //STORE_VAR(i, sn);
}

void Storage::readSerial() {
  for (uint8_t i=0; i<36; i++) {
    Printer::serial_number[i] = EEPROM.read(i);
  }
}

void Storage::write(int &pos, uint8_t* value, uint8_t size) {
  for (uint8_t i=0; i<size; i++) {
    EEPROM.write(pos, *value);
    pos++;
    value++;
    delay(20);
  }
}

void Storage::read(int &pos, uint8_t* value, uint8_t size) {
  for (uint8_t i=0; i<size; i++) {
    *value = EEPROM.read(pos);
    pos++;
    value++;
  }
}
