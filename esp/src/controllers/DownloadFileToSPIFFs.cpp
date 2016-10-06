//
// Created by Phillip Schuster on 05.10.16.
//

#include "DownloadFileToSPIFFs.h"
#include "Idle.h"
#include "HandleDownloadError.h"

DownloadFileToSPIFFs::DownloadFileToSPIFFs(String url, String localFilePath, Mode* nextMode):
        DownloadURL(url),
        _nextMode(nextMode),
        _localFilePath(localFilePath)
{}

DownloadFileToSPIFFs::~DownloadFileToSPIFFs()
{
}

String DownloadFileToSPIFFs::getName()
{
    return "DownloadFileToSPIFFs";
}

bool DownloadFileToSPIFFs::onBeginDownload(uint32_t expectedSize)
{
    _file = SPIFFS.open("/download.tmp", "w");

    Serial.println("Download began");

    return true;
}

bool DownloadFileToSPIFFs::onDataReceived(uint8_t *data, uint16_t size)
{
    Serial.println("Data received");
    _file.write(data,size);

    return true;
}

void DownloadFileToSPIFFs::onError(DownloadError errorCode)
{
    Serial.println("Error");
    HandleDownloadError* error = new HandleDownloadError(errorCode);
    Application.pushMode(error);
}

void DownloadFileToSPIFFs::onFinished()
{
    _file.close();
    Serial.println("Finished");

    //Delete existing file and rename downloaded file
    if (SPIFFS.exists(_localFilePath)) {
        Serial.println("File exists");
        if (!SPIFFS.remove(_localFilePath)) {
            Serial.println("Remove failed");
            HandleDownloadError* error = new HandleDownloadError(DownloadError::RemoveOldFilesFailed);
            Application.pushMode(error);
            return;
        }
    }

    if (!SPIFFS.rename("/download.tmp",_localFilePath)) {
        Serial.println("Rename failed");
        HandleDownloadError* error = new HandleDownloadError(DownloadError::PrepareDownloadedFileFailed);
        Application.pushMode(error);
        return;
    }

    if (_nextMode != NULL)
    {
        Serial.println("Update firmware");
        Application.pushMode(_nextMode);
    }
    else
    {
        Idle* idle = new Idle();
        Application.pushMode(idle);
    }
}
