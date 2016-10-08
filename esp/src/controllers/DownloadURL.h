//
// Created by Phillip Schuster on 05.10.16.
//

#ifndef ESP_DOWNLOADURL_H
#define ESP_DOWNLOADURL_H

#include "core/Mode.h"
#include <ESP8266HTTPClient.h>
#include "../core/FasterWiFiClient.h"
#include "../errors.h"

class DownloadURL: public Mode
{
private:
    typedef enum State {
        StateRequest = 0,
        StateDownload = 1,
        StateSuccess = 2,
        StateError = 3
    };

#pragma mark Constructor
public:
    DownloadURL(String url);
    virtual ~DownloadURL();

#pragma mark Application Flow
    virtual void loop();

#pragma mark Communication with MK20
    virtual bool handlesTask(TaskID taskID);
    virtual bool runTask(CommHeader& header, const uint8_t* data, size_t dataSize, uint8_t* responseData, uint16_t* responseDataSize, bool* sendResponse, bool* success);

#pragma mark private member functions
    bool parseUrl();

#pragma mark DownloadURL prototcol
    virtual bool onBeginDownload(uint32_t expectedSize) = 0;
    virtual bool onDataReceived(uint8_t* data, uint16_t size) = 0;
    virtual void onError(DownloadError errorCode) = 0;
    virtual void onFinished() = 0;
    virtual bool readNextData();

#pragma mark Getter and Setter
    uint8_t* getBuffer() { return _buffer; };
    int getBufferIndex() { return _bufferIndex; };

#pragma mark private member variables
private:
    State mode;
    DownloadError _error;
    WiFiClient* _stream;
    HTTPClient _httpClient;
    static const int _bufferSize = 128;
    uint8_t _buffer[_bufferSize];
    int _bufferIndex;
    int _numChunks;
    int _bytesToDownload;
    unsigned long _lastBytesReadTimeStamp;
    uint8_t _retries;

    int _port;
    String _url;
    String _protocol;
    String _host;
    String _path;
};


#endif //ESP_DOWNLOADURL_H
