#include "event_logger.h"

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino.h>
#include "core/CommStack.h"
#include "application.h"

extern AsyncEventSource events;

void EventLogger::log(char * msg, ...) {

  va_list ap;
  char buffer[256];

  va_start(ap, msg);
  int ret = vsnprintf(buffer, sizeof buffer, msg, ap);
  va_end(ap);

  events.send(buffer);

    //Serial.println(buffer);


//  int len = strlen(buffer);
//  Application.getMK20Stack()->requestTask(TaskID::DebugLog,(size_t)(len+1),(uint8_t*)&buffer[0]);

}

void EventLogger::log(const char * msg, ...) {

    va_list ap;
    char buffer[256];

    va_start(ap, msg);
    int ret = vsnprintf(buffer, sizeof buffer, msg, ap);
    va_end(ap);

    events.send(buffer);

    //Serial.println(buffer);

  //int len = strlen(buffer);
  //Application.getMK20Stack()->requestTask(TaskID::DebugLog,(size_t)(len+1),(uint8_t*)&buffer[0]);

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
