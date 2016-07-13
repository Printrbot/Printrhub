//
// Created by Phillip Schuster on 13.07.16.
//

#ifndef ESP8266_ARM_SWD_PUBNUBLOGGER_H
#define ESP8266_ARM_SWD_PUBNUBLOGGER_H

#include "Print.h"

#define PUBNUB_LOGGER_BUFFER_SIZE 100

class PubNubLogger: public Print
{
public:
    PubNubLogger(){
        _byteIndex = 0;
        memset(_buffer,0,PUBNUB_LOGGER_BUFFER_SIZE);
    };
    ~PubNubLogger(){};

public:
    virtual size_t write(uint8_t byte);

private:
    char _buffer[PUBNUB_LOGGER_BUFFER_SIZE];
    uint16_t _byteIndex;
};


#endif //ESP8266_ARM_SWD_PUBNUBLOGGER_H
