/*
 * Downloads a file from a given URL and stores it in local flash memory (SPIFFS)
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

#ifndef ESP_DOWNLOADFILETOSPIFFS_H
#define ESP_DOWNLOADFILETOSPIFFS_H

#include "DownloadURL.h"

class DownloadFileToSPIFFs : public DownloadURL {
 public:
  DownloadFileToSPIFFs(String url, String localFilePath);
  ~DownloadFileToSPIFFs();

#pragma mark DownloadURL prototcol
  virtual bool onBeginDownload(uint32_t expectedSize);
  virtual bool onDataReceived(uint8_t *data, uint16_t size);
  virtual void onError(DownloadError errorCode);
  virtual void onFinished();
  virtual void onCancelled();

#pragma mark Mode
  virtual String getName();

 private:
  String _localFilePath;
  File _file;
};

#endif //ESP_DOWNLOADFILETOSPIFFS_H
