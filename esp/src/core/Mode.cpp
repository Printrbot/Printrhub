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


#include "Mode.h"

Mode::Mode() :
	_nextMode(NULL) {

}

Mode::~Mode() {

}

void Mode::loop() {

}

void Mode::onWillStart() {

}

void Mode::onWillEnd() {

}

bool Mode::handlesTask(TaskID taskID) {
  return false;
}

bool Mode::runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool *success) {
  return false;
}

void Mode::setNextMode(Mode *mode) {
  _nextMode = mode;
}

void Mode::exit() {
  if (_nextMode == NULL) {
	EventLogger::log("Exiting mode %s to idle", getName().c_str());
	Application.idle();
  } else {
	EventLogger::log("Exiting mode %s to %s", getName().c_str(), _nextMode->getName().c_str());
	Application.pushMode(_nextMode);
  }
}

void Mode::exitWithError(DownloadError error) {
  EventLogger::log("Exiting mode %s with error %d", getName().c_str(), error);
  Application.handleError(error);
}