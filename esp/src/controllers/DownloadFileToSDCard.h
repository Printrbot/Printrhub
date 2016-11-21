/*
 * Downloads a file from a given URL and sends it via CommStack to MK20 that
 * finally stores the file on the SD card.
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

#ifndef ESP8266_ARM_SWD_DOWNLOADFILETOSDCARD_H
#define ESP8266_ARM_SWD_DOWNLOADFILETOSDCARD_H

#include "core/Mode.h"
#include <HttpClient.h>
#include "../core/FasterWiFiClient.h"
#include "../errors.h"
#include "DownloadURL.h"

class DownloadFileToSDCard : public DownloadURL {
 public:
  DownloadFileToSDCard(String url);
  ~DownloadFileToSDCard();

#pragma mark DownloadURL prototcol
  virtual bool onBeginDownload(uint32_t expectedSize);
  virtual bool onDataReceived(uint8_t *data, uint16_t size);
  virtual void onError(DownloadError errorCode);
  virtual void onFinished();
  virtual void onCancelled();
  virtual bool readNextData();

#pragma mark Communication with MK20
  virtual bool handlesTask(TaskID taskID);
  virtual bool runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool *success);

#pragma mark Mode
  virtual String getName();

#pragma mark Member Variables
 private:
  bool _waitForResponse;
  unsigned long _errorTime;
  uint8_t _lastData[DOWNLOADURL_BUFFER_SIZE];
  size_t _lastDataSize;

};

#endif //ESP8266_ARM_SWD_DOWNLOADFILETOSDCARD_H
