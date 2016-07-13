//
// Created by Phillip Schuster on 13.07.16.
//

#include "PubNubLogger.h"
#include "PubNub.h"
#include "Application.h"

size_t PubNubLogger::write(uint8_t byte)
{
    if (byte == '\n')
    {
        //Serial.println(_buffer);
        //pubnub_publish("ESPDebug",_buffer,pubnub_callback);
        memset(_buffer,0,PUBNUB_LOGGER_BUFFER_SIZE);
        _byteIndex = 0;
    }
    else
    {
        _buffer[_byteIndex] = byte;
        _byteIndex++;

        if (_byteIndex >= PUBNUB_LOGGER_BUFFER_SIZE)
        {
//            Serial.println(_buffer);

            //pubnub_publish("ESPDebug",_buffer,pubnub_callback);
            memset(_buffer,0,PUBNUB_LOGGER_BUFFER_SIZE);
            _byteIndex = 0;
        }
    }

    return 1;
}