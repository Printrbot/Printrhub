//
// Created by Phillip Schuster on 14.07.16.
//

#include "FileLogger.h"
#include <FS.h>

size_t FileLogger::write(uint8_t byte)
{
    _buffer[_byteIndex] = byte;
    _byteIndex++;

    if (byte == '\n')
    {
        File file = SPIFFS.open("/log.txt","a");
        file.write(_buffer,_byteIndex);
        file.close();

        memset(_buffer,0,FILE_LOGGER_BUFFER_SIZE);
        _byteIndex = 0;
    }
    else
    {
        if (_byteIndex >= FILE_LOGGER_BUFFER_SIZE)
        {
            File file = SPIFFS.open("/log.txt","a");
            file.write(_buffer,_byteIndex);
            file.close();

            memset(_buffer,0,FILE_LOGGER_BUFFER_SIZE);
            _byteIndex = 0;
        }
    }

    return 1;
}