/*
 * Background job that handles receiving data via CommStack and writes them to the
 * SD card. Supports run length encoding for faster data transmission
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

#ifndef MK20_RECEIVESDCARDFILE_H
#define MK20_RECEIVESDCARDFILE_H

#include "../framework/core/BackgroundJob.h"
#include "SD.h"

class ReceiveSDCardFile : public BackgroundJob {
 public:
  ReceiveSDCardFile(String localFilePath, size_t fileSize, Compression compression);
  ~ReceiveSDCardFile();

  virtual bool RLE16Deflate(const uint8_t *data, size_t size);
  virtual bool writeToFile(const uint8_t *data, size_t size);
  virtual bool onDataReceived(const uint8_t *data, size_t size);

  virtual bool isIndeterminate(); //Default = true, i.e. no progress available
  virtual float fractionCompleted();
  virtual bool runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool *success);
  virtual bool handlesTask(TaskID taskID);
  virtual String getName();
  virtual void onWillStart();

 private:
  File _localFile;
  Compression _compression;
  size_t _fileSize;
  size_t _bytesLeft;
  String _localFilePath;
};

#endif //MK20_RECEIVESDCARDFILE_H
