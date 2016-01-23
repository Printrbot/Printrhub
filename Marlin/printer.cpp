#include "printer.h"
#include <Arduino.h>
#include "Configuration.h"
#include "temperature.h"
#include "language.h"
#include "pins.h"
#include "PString.h"

#ifdef ENABLE_AUTO_BED_LEVELING
#include "autolevel.h"
#endif


float Printer::feedrate = 1500;

float Printer::saved_feedrate = 0;
float Printer::current_position[4] = {0,0,0,0};
float Printer::homing_feedrate[3] = {3000, 3000, 100};
float Printer::destination[4] = {0,0,0,0};
bool Printer::relative_mode = false;
bool Printer::axis_known_position[3] = {false, false, false};
bool Printer::axis_relative[4] = {false, false, false, false};
bool Printer::home_all_axis = true;
bool Printer::stopped = false;
char Printer::axis_codes[4] = {'X','Y','Z','E'};
uint8_t Printer::active_extruder = 0;
int Printer::feed_multiply = 100;
int Printer::saved_feed_multiply = 0;
int Printer::extruder_multiply = 100;
int Printer::fan_speed = 0;
float Printer::homing_offset[3] = {0,0,0};

float Printer::probe_offset[3] = {X_PROBE_OFFSET_FROM_EXTRUDER_DEFAULT,
                                  Y_PROBE_OFFSET_FROM_EXTRUDER_DEFAULT,
                                  -Z_PROBE_OFFSET_FROM_EXTRUDER_DEFAULT};

float Printer::min_position[3] = {X_MIN_POS, Y_MIN_POS, Z_MIN_POS};
float Printer::max_position[3] = {X_MAX_POS, Y_MAX_POS, Z_MAX_POS};
float Printer::offset[3] = {0,0,0};

unsigned long Printer::max_inactive_time = 0;
unsigned long Printer::stepper_inactive_time = DEFAULT_STEPPER_DEACTIVE_TIME*1000l;
unsigned long Printer::last_cmd_time = 0;

char Printer::serial_number[37] = "00000000-0000-0000-0000-000000000000";

Printer::Printer() {
}

void Printer::setHotendTemperature(float temperature) {
  // temperature.h
  setTargetHotend(temperature, Printer::active_extruder);
}

void Printer::setHotendTemperatureAndWait(float temperature) {
  // minimum target temperature, waits until heating
  char sc[8]; PString(sc, 8, temperature);
  Printer::setHotendTemperature(temperature);

  // temperature.cpp
  setWatch();
  unsigned long cn = millis();
  // temperature.h
  bool target_direction = isHeatingHotend(Printer::active_extruder);
  long residency_start;
  residency_start = -1;

  char t[8];
  //char w[2];
  while((residency_start == -1) || (residency_start >= 0 && (((unsigned int) (millis() - residency_start)) < (TEMP_RESIDENCY_TIME * 1000UL))) ) {
    if( (millis() - cn) > 1000UL ) {
      //Print Temp Reading and remaining time every 1 second while heating up/cooling down
      PString(t, 8, degHotend(Printer::active_extruder), 1);
      if(residency_start > -1)
      {
         cn = ((TEMP_RESIDENCY_TIME * 1000UL) - (millis() - residency_start)) / 1000UL;
         PBSerial::printf(F("{\"m\":\"M109\", \"d\": {\"c\": %s, \"s\": %s, \"e\": %i, \"w\": %i }}\n"), t, sc, Printer::active_extruder, cn);
      } else {
         PBSerial::printf(F("{\"m\":\"M109\", \"d\": {\"c\": %s, \"s\": %s, \"e\": %i, \"w\": -1 }}\n"), t, sc, Printer::active_extruder);
      }
      cn = millis();
    }
    // temperature.cpp
    manage_heater();
    Printer::manageInactivity();
    if ((residency_start == -1 &&  target_direction && (degHotend(Printer::active_extruder) >= (degTargetHotend(Printer::active_extruder)-TEMP_WINDOW))) ||
        (residency_start == -1 && !target_direction && (degHotend(Printer::active_extruder) <= (degTargetHotend(Printer::active_extruder)+TEMP_WINDOW))) ||
        (residency_start > -1 && labs(degHotend(Printer::active_extruder) - degTargetHotend(Printer::active_extruder)) > TEMP_HYSTERESIS) )
    {
      residency_start = millis();
    }
  }
  //starttime=millis();
  Printer::last_cmd_time = millis();
}

void Printer::setBedTemperature(float temperature) {
  // temperature.cpp
  setTargetBed(temperature);
}

void Printer::setBedTemperatureAndWait(float temperature) {
  bool td = false;
  unsigned long cn = millis();

  Printer::setBedTemperature(temperature);
  cn = millis();

  char c[8];
  char t[8]; PString(t, 8, target_temperature_bed);

  while (target_temperature_bed > current_temperature_bed && target_temperature_bed > 25.0) {
    if(( millis() - cn) > 1000 )
    {
      PString(c, 8, degBed());
      PBSerial::printf(F("{\"m\":\"M190\", \"d\": {\"c\": %s, \"s\": %s}}\n"), c, t);
      cn = millis();
    }
    manage_heater();
    Printer::manageInactivity();
  }
  Printer::last_cmd_time = millis();

}

float Printer::getMaxLength(int axis) {
  if (axis == X_AXIS)
    return X_MAX_LENGTH;
  if (axis == Y_AXIS)
    return Y_MAX_LENGTH;
  if (axis == Z_AXIS)
    return Z_MAX_LENGTH;
}

int Printer::getHomeRetractLength(int axis){
  if (axis == X_AXIS)
    return X_HOME_RETRACT_MM;
  if (axis == Y_AXIS)
    return Y_HOME_RETRACT_MM;
  if (axis == Z_AXIS)
    return Z_HOME_RETRACT_MM;
}

int Printer::getHomeDirection(int axis) {
  if (axis == X_AXIS)
    return X_HOME_DIR;
  if (axis == Y_AXIS)
    return Y_HOME_DIR;
  if (axis == Z_AXIS)
    return Z_HOME_DIR;
}

int Printer::getHomePosition(int axis) {
  if (axis == X_AXIS)
    return (X_HOME_DIR == -1) ? Printer::min_position[axis] : Printer::max_position[axis];
  else if (axis == Y_AXIS)
    return (Y_HOME_DIR == -1) ? Printer::min_position[axis] : Printer::max_position[axis];
  else if (axis == Z_AXIS)
    return (Z_HOME_DIR == -1) ? Printer::min_position[axis] : Printer::max_position[axis];
}

void Printer::homeX() {
  if ((X_MIN_PIN > -1 && X_HOME_DIR == -1) || (X_MAX_PIN > -1 && X_HOME_DIR == 1))
    Printer::homeAxis(X_AXIS);
}

void Printer::homeY() {
  if ((Y_MIN_PIN > -1 && Y_HOME_DIR == -1) || (Y_MAX_PIN > -1 && Y_HOME_DIR == 1))
    Printer::homeAxis(Y_AXIS);
}

void Printer::homeZ() {
  if ((Z_MIN_PIN > -1 && Z_HOME_DIR == -1) || (Z_MAX_PIN > -1 && Z_HOME_DIR == 1))
    Printer::homeAxis(Z_AXIS);
}

void Printer::homeAxis(int axis) {
    Printer::current_position[axis] = 0;
    plan_set_position(Printer::current_position[X_AXIS],
                      Printer::current_position[Y_AXIS],
                      Printer::current_position[Z_AXIS],
                      Printer::current_position[E_AXIS]);

    Printer::destination[axis] = 1.5 * Printer::getMaxLength(axis) * Printer::getHomeDirection(axis);
    Printer::feedrate = Printer::homing_feedrate[axis];

    plan_buffer_line(Printer::destination[X_AXIS],
                     Printer::destination[Y_AXIS],
                     Printer::destination[Z_AXIS],
                     Printer::destination[E_AXIS],
                     Printer::feedrate/60,
                     Printer::active_extruder);

    st_synchronize();

    Printer::current_position[axis] = 0;

    plan_set_position(Printer::current_position[X_AXIS],
                      Printer::current_position[Y_AXIS],
                      Printer::current_position[Z_AXIS],
                      Printer::current_position[E_AXIS]);
    // backoff
    Printer::destination[axis] = - Printer::getHomeRetractLength(axis) * Printer::getHomeDirection(axis);
    plan_buffer_line(Printer::destination[X_AXIS],
                     Printer::destination[Y_AXIS],
                     Printer::destination[Z_AXIS],
                     Printer::destination[E_AXIS],
                     Printer::feedrate/60,
                     Printer::active_extruder);
    st_synchronize();

    // go back towards endstop again
    Printer::destination[axis] = 2 * Printer::getHomeRetractLength(axis) * Printer::getHomeDirection(axis);
    plan_buffer_line(Printer::destination[X_AXIS],
                     Printer::destination[Y_AXIS],
                     Printer::destination[Z_AXIS],
                     Printer::destination[E_AXIS],
                     (Printer::homing_feedrate[axis]/2)/60,
                     Printer::active_extruder);
    st_synchronize();

    Printer::current_position[axis] = Printer::getHomePosition(axis) + Printer::homing_offset[axis];
    Printer::min_position[axis] = Printer::min_position[axis] + Printer::homing_offset[axis];
    Printer::max_position[axis] = Printer::max_position[axis] + Printer::homing_offset[axis];

    Printer::destination[axis] = Printer::current_position[axis];
    Printer::feedrate = 0.0;
    endstops_hit_on_purpose();
    Printer::axis_known_position[axis] = true;
}

void Printer::home(bool x, bool y, bool z) {
  #ifdef ENABLE_AUTO_BED_LEVELING
    // planner.h
    plan_bed_level_matrix.set_to_identity();
  #endif

  Printer::saved_feedrate = Printer::feedrate;
  Printer::last_cmd_time = millis();
  // stepper.cpp
  enable_endstops(true);

  for (int8_t i=0; i<4; i++) {
    Printer::destination[i] = Printer::current_position[i];
  }

  Printer::feedrate = 0;
  Printer::home_all_axis = !(x || y || z);

  // If homing away from BED do Z first
  #if Z_HOME_DIR > 0
  if((Printer::home_all_axis) || z) {
    Printer::homeZ();
  }
  #endif

  if ((Printer::home_all_axis) || x) {
    Printer::homeX();
  }
  if ((Printer::home_all_axis) || y) {
    Printer::homeY();
  }
/*
  if (x)
  {
    if(this->gcode.getLongCodeValue() != 0) {
      Printer::current_position[X_AXIS]= this->gcode.getCodeValue()+Printer::homing_offset[0];
    }
  }

  if (this->gcode.hasCode(Printer::axis_codes[Y_AXIS])) {
    if(this->gcode.getLongCodeValue() != 0) {
      Printer::current_position[Y_AXIS]=this->gcode.getCodeValue() + Printer::homing_offset[1];
    }
  }
  */
  // If homing towards BED do Z last
  #if Z_HOME_DIR < 0
    if (Printer::home_all_axis) {
      Printer::destination[Z_AXIS] = Z_RAISE_BEFORE_HOMING * Z_HOME_DIR * (-1);
      Printer::current_position[Z_AXIS] = 0;

      plan_set_position(Printer::current_position[X_AXIS],
                        Printer::current_position[Y_AXIS],
                        Printer::current_position[Z_AXIS],
                        Printer::current_position[E_AXIS]);

      plan_buffer_line(Printer::destination[X_AXIS],
                       Printer::destination[Y_AXIS],
                       Printer::destination[Z_AXIS],
                       Printer::destination[E_AXIS],
                       XY_TRAVEL_SPEED,
                       Printer::active_extruder);
      st_synchronize();
      Printer::homeZ();
    }

    // check if X and Y are homed and probe is inside bed area.
    if (z) {
      if ((Printer::axis_known_position[X_AXIS]) && (Printer::axis_known_position[Y_AXIS]) \
        && (Printer::current_position[X_AXIS]+Printer::probe_offset[0] >= Printer::min_position[0]) \
        && (Printer::current_position[X_AXIS]+Printer::probe_offset[0] <= Printer::max_position[0]) \
        && (Printer::current_position[Y_AXIS]+Printer::probe_offset[1] >= Printer::min_position[1]) \
        && (Printer::current_position[Y_AXIS]+Printer::probe_offset[1] <= Printer::max_position[1])) {

        Printer::current_position[Z_AXIS] = 0;
        plan_set_position(Printer::current_position[X_AXIS],
                          Printer::current_position[Y_AXIS],
                          Printer::current_position[Z_AXIS],
                          Printer::current_position[E_AXIS]);

        Printer::destination[Z_AXIS] = Z_RAISE_BEFORE_HOMING * Z_HOME_DIR * (-1);    // Set destination away from bed
        Printer::feedrate = max_feedrate[Z_AXIS];

        plan_buffer_line(Printer::destination[X_AXIS],
                         Printer::destination[Y_AXIS],
                         Printer::destination[Z_AXIS],
                         Printer::destination[E_AXIS],
                         Printer::feedrate,
                         Printer::active_extruder);

        st_synchronize();
        Printer::homeZ();

      } else if (!((Printer::axis_known_position[X_AXIS]) && (Printer::axis_known_position[Y_AXIS]))) {
          PBSerial::error(ERR_POSITION_UNKNOWN);
      } else {
          PBSerial::error(ERR_ZPROBE_OUT);
      }
    }
  #endif
/*
  if (this->gcode.hasCode(Printer::axis_codes[Z_AXIS])) {
    if(this->gcode.getLongCodeValue() != 0) {
      Printer::current_position[Z_AXIS]=this->gcode.getCodeValue() + Printer::homing_offset[2];
    }
  }
  */
  #ifdef ENABLE_AUTO_BED_LEVELING
    if ((Printer::home_all_axis) || z) {
      Printer::current_position[Z_AXIS] += -Printer::probe_offset[2];  //Add Z_Probe offset (the distance is negative)
    }
  #endif

  plan_set_position(Printer::current_position[X_AXIS],
                    Printer::current_position[Y_AXIS],
                    Printer::current_position[Z_AXIS],
                    Printer::current_position[E_AXIS]);

  #ifdef ENDSTOPS_ONLY_FOR_HOMING
    // stepper.cpp
    enable_endstops(false);
  #endif

  Printer::feedrate = saved_feedrate;
  Printer::last_cmd_time = millis();
  endstops_hit_on_purpose();
}

void Printer::clampToSoftEndstops(float target[3]) {
  if (MIN_SOFTWARE_ENDSTOPS) {
    if (target[X_AXIS] < Printer::min_position[X_AXIS])
      Printer::destination[X_AXIS] = Printer::min_position[X_AXIS];
    if (target[Y_AXIS] < Printer::min_position[Y_AXIS])
      Printer::destination[Y_AXIS] = Printer::min_position[Y_AXIS];
    if (target[Z_AXIS] < Printer::min_position[Z_AXIS])
      Printer::destination[Z_AXIS] = Printer::min_position[Z_AXIS];
  }

  if (MAX_SOFTWARE_ENDSTOPS) {
    if (target[X_AXIS] > Printer::max_position[X_AXIS])
      Printer::destination[X_AXIS] = Printer::max_position[X_AXIS];
    if (target[Y_AXIS] > Printer::max_position[Y_AXIS])
      Printer::destination[Y_AXIS] = Printer::max_position[Y_AXIS];
    if (target[Z_AXIS] > Printer::max_position[Z_AXIS])
      Printer::destination[Z_AXIS] = Printer::max_position[Z_AXIS];
  }
}

void Printer::stop()
{
  // temperature.cpp
  disable_heater();
  if(!Printer::stopped) {
    Printer::stopped = true;

    // todo: save last gcode line for restart

    PBSerial::error(ERR_STOPPED);
  }
}

void Printer::dwell(unsigned long &t)
{
  st_synchronize();
  t += millis();
  Printer::last_cmd_time = millis();
  while(millis()  < t ){
    manage_heater();
    Printer::manageInactivity();
  }
}

void Printer::manageInactivity()
{
  if( (millis() - Printer::last_cmd_time) >  Printer::max_inactive_time )
    if(Printer::max_inactive_time)
      Printer::kill();
  if(Printer::stepper_inactive_time)  {
    if( (millis() - Printer::last_cmd_time) >  Printer::stepper_inactive_time )
    {
      if(blocks_queued() == false) {
        Printer::disableX();
        Printer::disableY();
        Printer::disableZ();
        Printer::disableE0();
        Printer::disableE1();
      }
    }
  }

  #if defined(CONTROLLERFAN_PIN) && CONTROLLERFAN_PIN > -1
    controllerFan(); //Check if fan should be turned on to cool stepper drivers down
  #endif
  // in planner.cpp
  check_axes_activity();
}

void Printer::kill() {
  cli(); // Stop interrupts
  // temperature.cpp
  disable_heater();

  Printer::disableX();
  Printer::disableY();
  Printer::disableZ();
  Printer::disableE0();
  Printer::disableE1();

  PBSerial::error(ERR_KILLED);
  // Wait for reset
  while(1) {};
}

void Printer::disableX() {
  #if defined(X_ENABLE_PIN) && X_ENABLE_PIN > -1
  WRITE(X_ENABLE_PIN,!X_ENABLE_ON); Printer::axis_known_position[X_AXIS] = false;
  #endif
}

void Printer::disableY() {
  #if defined(Y_ENABLE_PIN) && Y_ENABLE_PIN > -1
  WRITE(Y_ENABLE_PIN,!Y_ENABLE_ON); Printer::axis_known_position[Y_AXIS] = false;
  #endif
}

void Printer::disableZ() {
  #if defined(Z_ENABLE_PIN) && Z_ENABLE_PIN > -1
  WRITE(Z_ENABLE_PIN,!Z_ENABLE_ON); Printer::axis_known_position[Z_AXIS] = false;
  #endif
}

void Printer::disableE0() {
  #if defined(E0_ENABLE_PIN) && E0_ENABLE_PIN > -1
  WRITE(E0_ENABLE_PIN,!E_ENABLE_ON);
  #endif
}

void Printer::disableE1() {
  #if defined(E1_ENABLE_PIN) && E1_ENABLE_PIN > -1
  WRITE(E1_ENABLE_PIN,!E_ENABLE_ON);
  #endif
}

void Printer::enableX() {
  #if defined(X_ENABLE_PIN) && X_ENABLE_PIN > -1
  WRITE(X_ENABLE_PIN, X_ENABLE_ON);
  #endif
}

void Printer::enableY() {
  #if defined(Y_ENABLE_PIN) && Y_ENABLE_PIN > -1
  WRITE(Y_ENABLE_PIN, Y_ENABLE_ON);
  #endif
}

void Printer::enableZ() {
  #if defined(Z_ENABLE_PIN) && Z_ENABLE_PIN > -1
  WRITE(Z_ENABLE_PIN, Z_ENABLE_ON);
  #endif
}

void Printer::enableE0() {
  #if defined(E0_ENABLE_PIN) && E0_ENABLE_PIN > -1
  WRITE(E0_ENABLE_PIN, E_ENABLE_ON);
  #endif
}

void Printer::enableE1() {
  #if defined(E1_ENABLE_PIN) && E1_ENABLE_PIN > -1
  WRITE(E1_ENABLE_PIN, E_ENABLE_ON);
  #endif
}

void Printer::setMaxFeedrate() {
  float f = 1000;
  if (Printer::destination[X_AXIS] != Printer::current_position[X_AXIS])
    f = max_feedrate[X_AXIS];
  if (Printer::destination[Y_AXIS] != Printer::current_position[Y_AXIS] && f > max_feedrate[Y_AXIS]) {
      f = max_feedrate[Y_AXIS];
  }
  if (Printer::destination[Z_AXIS] != Printer::current_position[Z_AXIS] && f > max_feedrate[Z_AXIS]) {
      f = max_feedrate[Z_AXIS];
  }
  Printer::feedrate = f*60;
}

void Printer::disableAllSteppers() {
  Printer::disableX();
  Printer::disableY();
  Printer::disableZ();
  Printer::disableE0();
  Printer::disableE1();
}

void Printer::reportEndstops()
{
  int x_min = -1,
      x_max = -1,
      y_min = -1,
      y_max = -1,
      z_min = -1,
      z_max = -1,
      e_min = -1,
      e_max = -1;

  #if defined(X_MIN_PIN) && X_MIN_PIN > -1
    x_min = (READ(X_MIN_PIN) ^ X_MIN_ENDSTOP_INVERTING) ? 1 : 0;
  #endif
  #if defined(X_MAX_PIN) && X_MAX_PIN > -1
    x_max = (READ(X_MAX_PIN) ^ X_MAX_ENDSTOP_INVERTING) ? 1 : 0;
  #endif
  #if defined(Y_MIN_PIN) && Y_MIN_PIN > -1
    y_min = (READ(Y_MIN_PIN) ^ Y_MIN_ENDSTOP_INVERTING) ? 1 : 0;
  #endif
  #if defined(Y_MAX_PIN) && Y_MAX_PIN > -1
    y_max = (READ(Y_MAX_PIN) ^ Y_MAX_ENDSTOP_INVERTING) ? 1 : 0;
  #endif
  #if defined(Z_MIN_PIN) && Z_MIN_PIN > -1
    z_min = (READ(Z_MIN_PIN) ^ Z_MIN_ENDSTOP_INVERTING) ? 1 : 0;
  #endif
  #if defined(Z_MAX_PIN) && Z_MAX_PIN > -1
    z_max = (READ(Z_MAX_PIN) ^ Z_MAX_ENDSTOP_INVERTING) ? 1 : 0;
  #endif
  #if defined(E_MIN_PIN) && E_MIN_PIN > -1
    e_min = (READ(E_MIN_PIN) ^ E_MIN_ENDSTOP_INVERTING) ? 1 : 0;
  #endif
  #if defined(E_MAX_PIN) && E_MAX_PIN > -1
    e_max = (READ(E_MAX_PIN) ^ E_MAX_ENDSTOP_INVERTING) ? 1 : 0;
  #endif

  PBSerial::printf(F("{\"m\":\"m119\", \"d\": {"
      "\"x-min\": %i, \"x-max\": %i,"
      "\"y-min\": %i, \"y-max\": %i,"
      "\"z-min\": %i, \"z-max\": %i,"
      "\"e-min\": %i, \"e-max\": %i}}\n"),
      x_min, x_max,
      y_min, y_max,
      z_min, z_max,
      e_min, e_max);
}
