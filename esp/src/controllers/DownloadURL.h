/*
 * This is the base class for all URL downloading classes. It handles all the
 * request and response details and calls a few functions that must be
 * overridden by the subclass to handle incoming data and errors.
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

#ifndef ESP_DOWNLOADURL_H
#define ESP_DOWNLOADURL_H

#include "core/Mode.h"
#include <ESP8266HTTPClient.h>
#include "../core/FasterWiFiClient.h"
#include "../errors.h"

//This is the size of the download buffer. Typically this buffer is filled before onDataReceived is called (but not necessarily)
//and as we often need to send data using UART (CommStack) we want to keep packets so small that they fit in Serial buffers which
//are typically 64 bytes.
#define DOWNLOADURL_BUFFER_SIZE 60

class DownloadURL : public Mode {
 private:
  typedef enum State {
	StateRequest = 0,
	StateDownload = 1,
	StateSuccess = 2,
	StateError = 3,
	StateCancelled = 4
  };

#pragma mark Constructor
 public:
  DownloadURL(String url);
  virtual ~DownloadURL();

#pragma mark Application Flow
  virtual void loop();

#pragma mark Communication with MK20
  virtual bool handlesTask(TaskID taskID);
  virtual bool runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool *success);

#pragma mark private member functions
  bool parseUrl();

#pragma mark DownloadURL prototcol
  virtual bool onBeginDownload(uint32_t expectedSize) = 0;
  virtual bool onDataReceived(uint8_t *data, uint16_t size) = 0;
  virtual void onError(DownloadError errorCode) = 0;
  virtual void onFinished() = 0;
  virtual void onCancelled() = 0;
  virtual bool readNextData();
  virtual void cancelDownload();

#pragma mark Getter and Setter
  uint8_t *getBuffer() { return _buffer; };

#pragma mark private member variables
 private:
  State mode;
  DownloadError _error;
  WiFiClient *_stream;
  HTTPClient _httpClient;
  static const int _bufferSize = DOWNLOADURL_BUFFER_SIZE;
  uint8_t _buffer[_bufferSize];
  int _numChunks;
  int _bytesToDownload;
  unsigned long _lastBytesReadTimeStamp;

  int _port;
  String _url;
  String _protocol;
  String _host;
  String _path;
};

#endif //ESP_DOWNLOADURL_H
