//
// Created by Phillip Schuster on 12.07.16.
//

#ifndef ESP8266_ARM_SWD_DOWNLOADFILETOSDCARD_H
#define ESP8266_ARM_SWD_DOWNLOADFILETOSDCARD_H

#include "core/Mode.h"
#include <HttpClient.h>
#include "../core/FasterWiFiClient.h"
#include "../errors.h"

class DownloadFileToSDCard: public Mode
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
    DownloadFileToSDCard(String url);
    virtual ~DownloadFileToSDCard();

#pragma mark Application Flow
    virtual void loop();
    virtual void onWillStart();
    virtual void onWillEnd();

#pragma mark private member functions
    void setupWiFi();
    void sendError(String errorMessage);
    void addByteToBuffer(uint8_t byte);
    void sendBuffer();
    void sendResponse(uint32_t contentLength);
    bool parseUrl();

#pragma mark Communication
    virtual bool handlesTask(TaskID taskID);
    virtual bool runTask(CommHeader& header, const uint8_t* data, size_t dataSize, uint8_t* responseData, uint16_t* responseDataSize, bool* sendResponse, bool* success);

#pragma mark Mode
    virtual String getName();

#pragma mark private member variables
private:
    State mode;
    DownloadError _error;
    FasterWiFiClient client;
    HttpClient httpClient;
    static const int _bufferSize = 128;
    char _buffer[_bufferSize];
    int _bufferIndex;
    int _numChunks;
    bool _waitForResponse;
    int _bytesToDownload;
    unsigned long _lastBytesReadTimeStamp;
    uint8_t _retries;

    int _port;
    String _url;
    String _protocol;
    String _host;
    String _path;
};


#endif //ESP8266_ARM_SWD_DOWNLOADFILETOSDCARD_H
