//
// Created by Phillip Schuster on 12.07.16.
//

#ifndef ESP8266_ARM_SWD_DOWNLOADFILETOSDCARD_H
#define ESP8266_ARM_SWD_DOWNLOADFILETOSDCARD_H

#include "core/Mode.h"
#include <HttpClient.h>
#include "../core/FasterWiFiClient.h"
#include "../errors.h"
#include "DownloadURL.h"

class DownloadFileToSDCard: public DownloadURL
{
public:
    DownloadFileToSDCard(String url);
    ~DownloadFileToSDCard();

#pragma mark DownloadURL prototcol
    virtual bool onBeginDownload(uint32_t expectedSize);
    virtual bool onDataReceived(uint8_t* data, uint16_t size);
    virtual void onError(DownloadError errorCode);
    virtual void onFinished();
    virtual bool readNextData();

#pragma mark Communication with MK20
    virtual bool handlesTask(TaskID taskID);
    virtual bool runTask(CommHeader& header, const uint8_t* data, size_t dataSize, uint8_t* responseData, uint16_t* responseDataSize, bool* sendResponse, bool* success);

#pragma mark Mode
    virtual String getName();

#pragma mark Member Variables
private:
    bool _waitForResponse;
    unsigned long _errorTime;
    uint8_t _lastData[DOWNLOADURL_BUFFER_SIZE];
    size_t _lastDataSize;

};


#endif //ESP8266_ARM_SWD_DOWNLOADFILETOSDCARD_H
