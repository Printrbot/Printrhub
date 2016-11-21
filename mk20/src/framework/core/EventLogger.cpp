/*
 * Logs messages to the DebugSerial port that is configured in HAL.h
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Phillip Schuster
 * https://github.com/Printrbot/Printrhub
 *
 * Developed in cooperation by Phillip Schuster (@appfruits) from appfruits.com
 * http://www.appfruits.com/printrhub
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

    if (strlen(buffer) > 0 && buffer[strlen(buffer)-1] == '\n') {
      //We already have a newline in the buffer
      DebugSerial.print(buffer);
    } else {
      DebugSerial.println(buffer);
    }

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

    if (strlen(buffer) > 0 && buffer[strlen(buffer)-1] == '\n') {
      //We already have a newline in the buffer
      DebugSerial.print(buffer);
    } else {
      DebugSerial.println(buffer);
    }

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
