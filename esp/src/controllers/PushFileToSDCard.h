/*
 * Pushes a file from SPIFFS to SD card via CommStack. Sends data packages to MK20
 * that finally writes the data to SD card.
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

#ifndef ESP_PUSHFILETOSDCARD_H
#define ESP_PUSHFILETOSDCARD_H

#include "core/Mode.h"

class PushFileToSDCard : public Mode {
 public:
  PushFileToSDCard(const String &localFilePath, const String &targetFilePath, bool showUI = false, Compression compression = Compression::None);
  ~PushFileToSDCard();

  void loop();
  void onWillStart();

  //bool handlesTask(TaskID taskID);
  bool runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool *success);
  virtual bool handlesTask(TaskID taskID);
  String getName();

 private:
  String _localFilePath;
  String _targetFilePath;
  bool _showUI;
  bool _waitForResponse;
  bool _fileOpen;
  Compression _compression;
  File _localFile;
  size_t _bytesLeft;
  unsigned long _requestTime;
};

#endif //ESP_PUSHFILETOSDCARD_H
