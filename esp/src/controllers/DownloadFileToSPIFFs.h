//
// Created by Phillip Schuster on 05.10.16.
//

#ifndef ESP_DOWNLOADFILETOSPIFFS_H
#define ESP_DOWNLOADFILETOSPIFFS_H

#include "DownloadURL.h"

class DownloadFileToSPIFFs : public DownloadURL
{
public:
    DownloadFileToSPIFFs(String url, String localFilePath);
    ~DownloadFileToSPIFFs();

#pragma mark DownloadURL prototcol
    virtual bool onBeginDownload(uint32_t expectedSize);
    virtual bool onDataReceived(uint8_t* data, uint16_t size);
    virtual void onError(DownloadError errorCode);
    virtual void onFinished();

#pragma mark Mode
    virtual String getName();

private:
    String _localFilePath;
    File _file;
};


#endif //ESP_DOWNLOADFILETOSPIFFS_H
