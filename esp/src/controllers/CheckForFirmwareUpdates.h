/*
 * Downloads a JSON file from the printrbot cloud and stores firmware update
 * info in the application. If a new firmware is available the user is prompted
 * to install it.
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/behind-the-scenes-printrbot-simple-2016/
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

#ifndef ESP_CHECKFORFIRMWAREUPDATES_H
#define ESP_CHECKFORFIRMWAREUPDATES_H

#include "DownloadURL.h"

#ifdef BETA
  #define FIRMWAREUPDATE_URL "http://static.printrbot.cloud/firmware-beta/simple/firmware.json"
#else
  #define FIRMWAREUPDATE_URL "http://static.printrbot.cloud/firmware/simple/firmware.json"
#endif

class CheckForFirmwareUpdates : public DownloadURL {
 public:
  CheckForFirmwareUpdates();
  ~CheckForFirmwareUpdates();

#pragma mark DownloadURL prototcol
  virtual bool onBeginDownload(uint32_t expectedSize);
  virtual bool onDataReceived(uint8_t *data, uint16_t size);
  virtual void onError(DownloadError errorCode);
  virtual void onFinished();
  virtual void onCancelled();

#pragma mark Mode
  virtual String getName();

#pragma mark member variables
  uint8_t *_content;
  uint32_t _size;
  uint32_t _pointer;
};

#endif //ESP_CHECKFORFIRMWAREUPDATES_H
