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

#ifndef MK20_EVENTLOGGER_H
#define MK20_EVENTLOGGER_H

#include <Arduino.h>

#define LOG_SPAM 0
#define LOG_NOTICE 1
#define LOG_WARNING 2
#define LOG_ERROR 3
#define LOG_ALWAYS 4

#define LOG_FLOW 1
#define LOG_TOUCH 2
#define LOG_DISPLAY 4
#define LOG_COMMSTACK 8
#define LOG_PRINTER 16

#define COMMSTACK_NOTICE(X, ...) EventLogger.log(LOG_COMMSTACK,LOG_NOTICE,X,##__VA_ARGS__)
#define COMMSTACK_WARNING(X, ...) EventLogger.log(LOG_COMMSTACK,LOG_WARNING,X,##__VA_ARGS__)
#define COMMSTACK_ERROR(X, ...) EventLogger.log(LOG_COMMSTACK,LOG_ERROR,X,##__VA_ARGS__)
#define COMMSTACK_SPAM(X, ...) EventLogger.log(LOG_COMMSTACK,LOG_SPAM,X,##__VA_ARGS__)

#define FLOW_NOTICE(X, ...) EventLogger.log(LOG_FLOW,LOG_NOTICE,X,##__VA_ARGS__)
#define FLOW_ERROR(X, ...) EventLogger.log(LOG_FLOW,LOG_ERROR,X,##__VA_ARGS__)
#define FLOW_SPAM(X, ...) EventLogger.log(LOG_FLOW,LOG_SPAM,X,##__VA_ARGS__)
#define FLOW_ALWAYS(X, ...) EventLogger.log(LOG_FLOW,LOG_ALWAYS,X,##__VA_ARGS__)

#define PRINTER_NOTICE(X, ...) EventLogger.log(LOG_PRINTER,LOG_NOTICE,X,##__VA_ARGS__)
#define PRINTER_WARNING(X, ...) EventLogger.log(LOG_PRINTER,LOG_WARNING,X,##__VA_ARGS__)
#define PRINTER_ERROR(X, ...) EventLogger.log(LOG_PRINTER,LOG_ERROR,X,##__VA_ARGS__)
#define PRINTER_SPAM(X, ...) EventLogger.log(LOG_PRINTER,LOG_SPAM,X,##__VA_ARGS__)

class EventLoggerClass {
 public:
  EventLoggerClass();
  void log(uint8_t logContext, uint8_t logLevel, char *msg, ...);
  void log(uint8_t logContext, uint8_t logLevel, const char *msg, ...);
  void setLogLevel(uint8_t logLevel) { _logLevel = logLevel; };
  void setLogContexts(uint8_t contextFlags) { _contexts = contextFlags; };

 private:
  uint8_t _logLevel;
  uint8_t _contexts;
};

#endif //MK20_EVENTLOGGER_H
