/*
 * Permits storing log messages that can be requested using /events GET request
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/behind-the-scenes-printrbot-simple-2016/
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Mick Balaban, Phillip Schuster
 * https://github.com/Printrbot/Printrhub
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

#include "event_logger.h"
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino.h>
#include "core/CommStack.h"
#include "application.h"

extern AsyncEventSource events;

void EventLogger::log(char *msg, ...) {

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

void EventLogger::log(const char *msg, ...) {

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
