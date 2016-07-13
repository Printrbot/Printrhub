//
// Created by Phillip Schuster on 12.07.16.
//

#ifndef ESP8266_ARM_SWD_DOWNLOADFILETOSDCARD_H
#define ESP8266_ARM_SWD_DOWNLOADFILETOSDCARD_H

#include "core/Mode.h"
#include <HttpClient.h>

class DownloadFileToSDCard: public Mode
{
private:
    typedef enum State {
        StateWorking = 0,
        StateSuccess = 1,
        StateError = 2
    };

#pragma mark Constructor
public:
    DownloadFileToSDCard(String filePath);
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

#pragma mark Mode
    virtual String getName();

#pragma mark private member variables
private:
    State mode;
    WiFiClient client;
    HttpClient httpClient;
    String _filePath;
    static const int _bufferSize = 128;
    char _buffer[_bufferSize];
    int _bufferIndex;
};


#endif //ESP8266_ARM_SWD_DOWNLOADFILETOSDCARD_H
