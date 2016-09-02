#ifndef DOWNLOADFILE_H
#define DOWNLOADFILE_H

#include "core/Mode.h"
#include <HttpClient.h>
#include <WiFiClient.h>

class DownloadFile: public Mode
{
private:
  typedef enum State {
    StateSaveFile = 0,
    StateStartDownload = 1,
    StateDownloading = 2,
    StateSuccess = 3,
    StateError = 4,
    StateWaiting = 5
  };

public:
  DownloadFile(TaskID task_id, String file_id, String url);
  DownloadFile(TaskID task_id, String url);
  virtual ~DownloadFile();

  virtual void loop();
  virtual void onWillStart();
  virtual void onWillEnd();

  virtual String getName();
  virtual bool handlesTask(TaskID taskID);
  virtual bool runTask(CommHeader& header, const uint8_t* data, size_t dataSize, uint8_t* responseData, uint16_t* responseDataSize, bool* sendResponse, bool* success);

private:

  void addByteToBuffer(uint8_t byte);

  State _state;
  String _url;
  int _port;
  String _file_id;
  String _protocol;
  String _host;
  String _path;
  static const int _bufferSize = 128;
  char _buffer[_bufferSize];
  int _bufferIndex;
  int _numChunks;
  bool _waitForResponse;
  bool parseUrl();
  uint32_t _bytesToDownload;
  unsigned long _lastBytesReadTimeStamp;
  TaskID _currentTask;
  HttpClient _http;
  WiFiClient _wclient;
};


#endif
