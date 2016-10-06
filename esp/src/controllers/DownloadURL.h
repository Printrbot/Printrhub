//
// Created by Phillip Schuster on 05.10.16.
//

#ifndef ESP_DOWNLOADURL_H
#define ESP_DOWNLOADURL_H

#include "core/Mode.h"
#include <HttpClient.h>
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

#pragma mark private member functions
    bool parseUrl();

#pragma mark DownloadURL prototcol
    virtual bool onBeginDownload(uint32_t expectedSize) = 0;
    virtual bool onDataReceived(uint8_t* data, uint16_t size) = 0;
    virtual void onError(DownloadError errorCode) = 0;
    virtual void onFinished() = 0;

#pragma mark private member variables
private:
    State mode;
    DownloadError _error;
    FasterWiFiClient client;
    HttpClient httpClient;
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
