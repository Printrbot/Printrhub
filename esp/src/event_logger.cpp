#include "event_logger.h"

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino.h>
#include "core/CommStack.h"

extern AsyncEventSource events;

void EventLogger::log(char * msg, ...) {
  events.send(msg);
}

void EventLogger::log(const char * msg, ...) {
  events.send(msg);

  /*
  va_list args;
  va_start(args, msg);

  while (1) {
    char ch = va_arg(args, int);
    if (!ch) break;
    events.send(&ch);
  }
  */

}
