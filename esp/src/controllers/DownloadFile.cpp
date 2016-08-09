#include "DownloadFile.h"
#include "Idle.h"
#include "Application.h"
#include "../event_logger.h"
#include "../hal.h"
#include "../core/CommStack.h"


const int _networkTimeout = 10*1000;
// Number of milliseconds to wait if no data is available before trying again
const int _networkDelay = 100;

DownloadFile::DownloadFile(TaskID task_id, String file_id, String url):
Mode(),
_url(url),
_file_id(file_id),
_state(StateSaveFile),
_http(_wclient),
_currentTask(task_id)
{
  memset(_buffer,0,_bufferSize);
  _bufferIndex = 0;
  _numChunks = 0;
  _waitForResponse = false;

  if (!parseUrl()) {
    EventLogger::log("could not parse url");
    _state = StateError;
  }
}

DownloadFile::DownloadFile(TaskID task_id, String url):
Mode(),
_url(url),
_state(StateStartDownload),
_http(_wclient),
_currentTask(task_id)
{
  memset(_buffer,0,_bufferSize);
  _bufferIndex = 0;
  _numChunks = 0;
  _waitForResponse = false;

  if (!parseUrl()) {
    EventLogger::log("could not parse url");
    _state = StateError;
  }
}


DownloadFile::~DownloadFile() {
}

String DownloadFile::getName() {
  return "DownloadFile";
}

bool DownloadFile::parseUrl() {

  // parse protocol
	int i = _url.indexOf(":");
	if (i<0) return false;

	_protocol = _url.substring(0, i);
  _url.remove(0, (i + 3));

  // parse host
  i = _url.indexOf('/');
  _host = _url.substring(0, i);
  _url.remove(0, i);

  // parse port
  i = _host.indexOf(':');
  if (i>0) {
    EventLogger::log("EXTRACTING PORT");
    EventLogger::log(_host.substring(i+1).c_str());
    _port = _host.substring(i+1).toInt();
    _host.remove(i);
  } else {
    _port = 80;
  }
  // parse path
  _path = _url;

  EventLogger::log(_protocol.c_str());
  EventLogger::log(_host.c_str());
  EventLogger::log(_url.c_str());

  char _p[6];
  sprintf (_p, "%05i", _port);
  EventLogger::log(_p);

  return true;
}

void DownloadFile::onWillStart() {
}

void DownloadFile::onWillEnd() {
}

bool DownloadFile::handlesTask(TaskID taskID) {
  switch(taskID) {
    case GetProjectWithID:
    case GetJobWithID:
    case GetProjectItemWithID:
    case SaveProjectWithID:
    case FileSaveData:
    case FileSetSize:
    case FileClose:
    case Error:
      return true;
      break;
    default:
      return false;
  }

}

void DownloadFile::loop() {

  int httpCode = 0;

  if (_state == StateSaveFile) {
    EventLogger::log("IN StateSaveFile, sending request to save the file");
    const char* f = _file_id.c_str();
    Application.getMK20Stack()->requestTask(_currentTask, strlen(f), (uint8_t*) f);
    EventLogger::log("Sent request to save file, now waiting for response");
    _state = StateWaiting;
    return;
  }
  else if (_state == StateStartDownload) {

    EventLogger::log("IN STATE DOWNLOAD, TRYING TO FETCH THE FILE");
    EventLogger::log(_url.c_str());

    if (_http.get(_host.c_str(), _port, _path.c_str()) != 0) {
      _state = StateError;
      EventLogger::log("COULD NOT GET THIS BEYOTCH");
    } else {
      httpCode = _http.responseStatusCode();
      if (httpCode >= 0) {
        httpCode = _http.skipResponseHeaders();
        if (httpCode >= 0) {
          _bytesToDownload = _http.contentLength();
          EventLogger::log("Bytes to download:");
          Application.getMK20Stack()->requestTask(FileSetSize, sizeof(uint32_t), (uint8_t*)&_bytesToDownload);
          char _t[16];
          sprintf(_t, "%lu", _bytesToDownload);
          EventLogger::log(_t);
          _state = StateWaiting;
        } else {
          _state = StateError;
          EventLogger::log("ERROR HEADER CODE IS 0");
        }
      } else {
        EventLogger::log("ERROR STATUS CODE IS 0");
        _state = StateError;
      }
    }
  }
  else if (_state == StateDownloading) {

    if (_waitForResponse) return;

    char c;
    if (!_http.connected()) {
      EventLogger::log("Lost Connection");
      _state = StateError;
      return;
    }

    //EventLogger::log("Downloading data from the net");
    while (_http.available()) {
      ESP.wdtFeed();

      if (_http.available()) {
        c = _http.read();
        _bytesToDownload--;

        //Store the byte read from the web in our buffer
        _buffer[_bufferIndex] = c;
        _bufferIndex++;
        if (_bufferIndex >= _bufferSize) {
          //Buffer is full, send it to MK20 and leave the loop
          _waitForResponse = true;
          Application.getMK20Stack()->requestTask(FileSaveData,_bufferIndex, (uint8_t*)_buffer);
          memset(_buffer,0,_bufferSize);
          _bufferIndex = 0;
          //EventLogger::log("Sending SHIT>...");
          //Close this run loop now to keep up the rest of the application loop and to wait for the response
          return;
        }
        // We read something, reset the timeout counter
        _lastBytesReadTimeStamp = millis();
      }
      else {
        // We haven't got any data, so let's pause to allow some to
        // arrive
        EventLogger::log("DELAYING...");
        delay(_networkDelay);
      }
    }

    //All data read from the net, make sure we send the rest of the buffer
    if (_bytesToDownload <= 0) {
      _waitForResponse = true;
      Application.getMK20Stack()->requestTask(FileSaveData,_bufferIndex, (uint8_t*)_buffer);
      EventLogger::log("FINISHED SENDING FILE");
      //_state = StateSuccess;
    }
    else {
      //Check for timeout
      if ((millis() - _lastBytesReadTimeStamp) > _networkTimeout) {
        EventLogger::log("Connection timeout");
        _state = StateError;
      }
    }
  }

  if (_state == StateError) {
    Application.getMK20Stack()->requestTask(Error);
    Mode* mode = new Idle();
    Application.pushMode(mode);
    return;
  }

  if (_state == StateSuccess) {
    Application.getMK20Stack()->requestTask(FileClose);
    _state = StateWaiting;
    return;
  }

}

void DownloadFile::addByteToBuffer(uint8_t byte) {
  _buffer[_bufferIndex] = byte;
  _bufferIndex++;
  if (_bufferIndex >= _bufferSize) {
      Application.getMK20Stack()->requestTask(FileSaveData, _bufferIndex, (uint8_t*)_buffer);
      memset(_buffer,0,_bufferSize);
      _bufferIndex = 0;
  }
}


bool DownloadFile::runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool* success)
{
  if (header.commType == ResponseFailed) {
    // stop on error??
  }
  if (header.getCurrentTask() == FileClose) {
    // all good, exit the task
    EventLogger::log("File is closed, back to idle");
    Mode* mode = new Idle();
    Application.pushMode(mode);
  }
  if (header.getCurrentTask() == FileSaveData) {
    if (header.commType == ResponseSuccess) {
      _waitForResponse = false;
      // check if we are done
      if (_bytesToDownload <= 0) {
        EventLogger::log("OK ALL DONE, tell mk20 to close the file!");
        _state = StateSuccess;
      }
    }
  }
  else if (header.getCurrentTask() == SaveProjectWithID) {
    if (header.commType == ResponseSuccess) {
      _currentTask = FileSetSize;
      _state = StateStartDownload;
    }
  }
  else if (header.getCurrentTask() == FileSetSize) {
    if (header.commType == ResponseSuccess) {
      _currentTask = FileSaveData;
      _state = StateDownloading;
    }
  }

  return true;
}
