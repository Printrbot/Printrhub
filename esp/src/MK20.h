/*
 * Custom CommStack implementation acts as a wrapper to separate some CommStack
 * details from the rest of the application code. Mostly holds convenience functions
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/printrbot-simple-2016-commstack-explained
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Phillip Schuster
 * https://github.com/Printrbot/Printrhub
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

#ifndef ESP_MK20_H
#define ESP_MK20_H

#include "core/CommStack.h"
#include "config.h"
#include <FS.h>

class MK20 : public CommStack {
 public:
  MK20(Stream *port, CommStackDelegate *delegate);
  ~MK20();

  void reset();
  bool writeFirmware(File &file);
  bool updateFirmware(String localFilePath);
  void showUpdateFirmwareNotification();
  void showFirmwareInProgressNotification();

  bool isTimedOut() { return !_isAlive && _timeout > 10; };
  void incrementTimeout(float deltaTime) { _timeout += deltaTime; };
  uint8_t getNumTries() { return _numTries; };
  void incrementNumTries() {
	_numTries++;
	_timeout = 0;
  };
  void setBuildNumber(int buildNumber) {
	_buildNumber = buildNumber;
	_isAlive = true;
	_timeout = 0;
	_numTries = 0;
  };
  bool isAlive() { return _isAlive; };
  bool needsUpdate() { return FIRMWARE_BUILDNR > _buildNumber; };
  bool openSDFileForWrite(String targetFilePath, size_t bytesToSend, bool showUI = false, Compression compression = Compression::None);
  bool sendSDFileData(uint8_t *data, size_t size);
  bool closeSDFile();
  void showWiFiInfo();

 private:
  unsigned long _timeout;
  bool _isAlive;
  uint8_t _numTries;
  int _buildNumber;
};

#endif //ESP_MK20_H
