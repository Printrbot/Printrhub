/*
 * A mode is a module that implements a specific piece of work. Only one mode
 * can run at a time. The application holds a mode stack. You can push new modes
 * to this application stack (see Application class) that will cause the current
 * mode to end and start the new mode. Each mode has a loop function that is called
 * in each application loop.
 *
 * Copyright (c) 2016 Printrbot Inc.
 *
 * Developed in cooperation by Phillip Schuster (@appfruits) from appfruits.com
 * http://www.appfruits.com
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

#ifndef __MODE_H_
#define __MODE_H_

#include "Arduino.h"
#include "Application.h"
#include "CommStack.h"

class Mode {
#pragma mark Constructor
 public:

  Mode();
  virtual ~Mode();

#pragma mark Application Flow
  virtual void loop();
  virtual void onWillStart();
  virtual void onWillEnd();

#pragma mark Communication with MK20
  virtual bool handlesTask(TaskID taskID);
  virtual bool runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool *success);

#pragma mark Misc
  virtual String getName() = 0;
  virtual void setNextMode(Mode *mode);

#pragma mark Internally used
 protected:
  virtual void exit();
  virtual void exitWithError(DownloadError error);

#pragma mark Member Variables
 private:
  Mode *_nextMode;
};

#endif //__MODE_H_
