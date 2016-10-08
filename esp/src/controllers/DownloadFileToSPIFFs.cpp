//
// Created by Phillip Schuster on 05.10.16.
//

#include "DownloadFileToSPIFFs.h"
#include "Idle.h"
#include "HandleDownloadError.h"

DownloadFileToSPIFFs::DownloadFileToSPIFFs(String url, String localFilePath):
        DownloadURL(url),
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
    //_file = SPIFFS.open("/download.tmp", "w");
    EventLogger::log("Saving downloaded file in path %s",_localFilePath.c_str());
    _file = SPIFFS.open(_localFilePath, "w");
    if (!_file)
    {
        EventLogger::log("Failed to open file %s",_localFilePath.c_str());
        return false;
    }

    return true;
}

bool DownloadFileToSPIFFs::onDataReceived(uint8_t *data, uint16_t size)
{
    _file.write(data,size);

    return true;
}

void DownloadFileToSPIFFs::onError(DownloadError errorCode)
{
    HandleDownloadError* error = new HandleDownloadError(errorCode);
    Application.pushMode(error);
}

void DownloadFileToSPIFFs::onFinished()
{
    _file.close();

    //Delete existing file and rename downloaded file
/*    if (SPIFFS.exists(_localFilePath)) {
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
    }*/

    exit();
}
