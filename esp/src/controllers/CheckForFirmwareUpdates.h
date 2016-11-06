//
// Created by Phillip Schuster on 05.10.16.
//

#ifndef ESP_CHECKFORFIRMWAREUPDATES_H
#define ESP_CHECKFORFIRMWAREUPDATES_H

#include "DownloadURL.h"

#define FIRMWAREUPDATE_URL "http://static.printrbot.cloud/firmware/simple/firmware.json"

class CheckForFirmwareUpdates: public DownloadURL
{
public:
    CheckForFirmwareUpdates();
    ~CheckForFirmwareUpdates();

#pragma mark DownloadURL prototcol
    virtual bool onBeginDownload(uint32_t expectedSize);
    virtual bool onDataReceived(uint8_t* data, uint16_t size);
    virtual void onError(DownloadError errorCode);
    virtual void onFinished();
    virtual void onCancelled();

#pragma mark Mode
    virtual String getName();

#pragma mark member variables
    uint8_t* _content;
    uint32_t _size;
    uint32_t _pointer;
};


#endif //ESP_CHECKFORFIRMWAREUPDATES_H
