#ifndef PRINTER_H
#define PRINTER_H

#include "Arduino.h"

class Printer {
public:
  Printer();
  static float feedrate;
  static float saved_feedrate;
  static float homing_feedrate[];
  static bool relative_mode;
  static char axis_codes[4];
  static bool axis_relative[4];
  static float current_position[4];
  static float destination[4];
  static uint8_t active_extruder;
  static unsigned long last_cmd_time;
  static unsigned long max_inactive_time;
  static unsigned long stepper_inactive_time;
  static int feed_multiply;
  static int saved_feed_multiply;
  static int extruder_multiply;
  static int fan_speed;
  static float min_position[3];
  static float max_position[3];
  static float homing_offset[3];
  static float probe_offset[3];
  static bool axis_known_position[3];
  static bool home_all_axis;
  static bool stopped;
  static float offset[3];
  static char serial_number[37];


  static void setHotendTemperature(float temperature);
  static void setHotendTemperatureAndWait(float temperature);

  static void setBedTemperature(float temperature);
  static void setBedTemperatureAndWait(float temperature);

  static float getMaxLength(int axis);
  static int getHomePosition(int axis);
  static int getHomeRetractLength(int axis);
  static int getHomeDirection(int axis);
  static void clampToSoftEndstops(float target[3]);

  static void home(bool x, bool y, bool z);
  static void homeX();
  static void homeY();
  static void homeZ();
  static void stop();
  static void dwell(unsigned long &t);
  static void manageInactivity();
  static void kill();
  static void disableX();
  static void disableY();
  static void disableZ();
  static void disableE0();
  static void disableE1();
  static void enableX();
  static void enableY();
  static void enableZ();
  static void enableE0();
  static void enableE1();
  static void setMaxFeedrate();
  static void disableAllSteppers();
  static void reportEndstops();

private:
  static void homeAxis(int axis);
};


#endif
