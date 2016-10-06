#ifndef EVENT_LOGGER_H
#define EVENT_LOGGER_H

#include <Arduino.h>
#include "core/CommStack.h"

class EventLogger {
public:
  static void log(char * msg, ...);
  static void log(const char * msg, ...);
};

#endif
