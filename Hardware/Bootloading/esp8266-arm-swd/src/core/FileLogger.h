//
// Created by Phillip Schuster on 14.07.16.
//

#ifndef ESP8266_ARM_SWD_FILELOGGER_H
#define ESP8266_ARM_SWD_FILELOGGER_H

#include "Print.h"

#define FILE_LOGGER_BUFFER_SIZE 1024

class FileLogger: public Print
{
public:
    FileLogger(){
        _byteIndex = 0;
        memset(_buffer,0,FILE_LOGGER_BUFFER_SIZE);
    };
    ~FileLogger(){};

public:
    virtual size_t write(uint8_t byte);

private:
    uint8_t _buffer[FILE_LOGGER_BUFFER_SIZE];
    uint16_t _byteIndex;
};

#endif //ESP8266_ARM_SWD_FILELOGGER_H
