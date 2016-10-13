//
// Created by Phillip Schuster on 12.10.16.
//

#include "EventLogger.h"
#include "HAL.h"

EventLoggerClass::EventLoggerClass() {
  _logLevel = LOG_WARNING;
  _contexts = LOG_FLOW;
}

void EventLoggerClass::log(uint8_t logContext, uint8_t logLevel, char * msg, ...) {
  if (logContext & _contexts && logLevel >= _logLevel) {
    va_list ap;
    char buffer[256];

    va_start(ap, msg);
    int ret = vsnprintf(buffer, sizeof buffer, msg, ap);
    va_end(ap);

    if (logLevel == LOG_NOTICE) {
      DebugSerial.print("Notice: ");
    } else if (logLevel == LOG_WARNING) {
      DebugSerial.print("Warning: ");
    } else if (logLevel == LOG_ERROR) {
      DebugSerial.print("Error: ");
    } else {
      DebugSerial.print("Log: ");
    }
    DebugSerial.println(buffer);
    DebugSerial.flush();
  }


  //Serial.println(buffer);
  //Serial.print(0);


//  int len = strlen(buffer);
//  Application.getMK20Stack()->requestTask(TaskID::DebugLog,(size_t)(len+1),(uint8_t*)&buffer[0]);

}

void EventLoggerClass::log(uint8_t logContext, uint8_t logLevel, const char * msg, ...) {
  if (logContext & _contexts && logLevel >= _logLevel) {
    va_list ap;
    char buffer[256];

    va_start(ap, msg);
    int ret = vsnprintf(buffer, sizeof buffer, msg, ap);
    va_end(ap);

    if (logLevel == LOG_NOTICE) {
      DebugSerial.print("Notice: ");
    } else if (logLevel == LOG_WARNING) {
      DebugSerial.print("Warning: ");
    } else if (logLevel == LOG_ERROR) {
      DebugSerial.print("Error: ");
    } else {
      DebugSerial.print("Log: ");
    }

    DebugSerial.println(buffer);
    DebugSerial.flush();
  }

  //Serial.println(buffer);
  //Serial.print(0);

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
