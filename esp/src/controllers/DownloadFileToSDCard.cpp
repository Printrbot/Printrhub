//
// Created by Phillip Schuster on 12.07.16.
//

#include "DownloadFileToSDCard.h"
#include "Application.h"
#include "Idle.h"
#include "../core/CommStack.h"
#include "HandleDownloadError.h"

DownloadFileToSDCard::DownloadFileToSDCard(String url):
        DownloadURL(url),
        _waitForResponse(false),
        _retries(0)
{}

DownloadFileToSDCard::~DownloadFileToSDCard()
{
}

String DownloadFileToSDCard::getName()
{
    return "DownloadFileToSDCard";
}

bool DownloadFileToSDCard::onBeginDownload(uint32_t expectedSize)
{
    Application.getMK20Stack()->responseTask(TaskID::DownloadFile,sizeof(uint32_t),(uint8_t*)&expectedSize,true);
}

bool DownloadFileToSDCard::onDataReceived(uint8_t *data, uint16_t size)
{
    //We send the data to MK20 and wait for the response
    _waitForResponse = true;
    Application.getMK20Stack()->requestTask(TaskID::FileSaveData,size, data);

    return true;
}

bool DownloadFileToSDCard::readNextData()
{
    if (_waitForResponse) return false;
    return true;
}

void DownloadFileToSDCard::onError(DownloadError errorCode)
{
    HandleDownloadError* error = new HandleDownloadError(errorCode);
    Application.pushMode(error);
}

void DownloadFileToSDCard::onFinished()
{
    Application.getMK20Stack()->requestTask(TaskID::FileClose);

    exit();
}

bool DownloadFileToSDCard::handlesTask(TaskID taskID)
{
    if (taskID == TaskID::FileSaveData)
    {
        return true;
    }

    return false;
}

bool DownloadFileToSDCard::runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool* success)
{
    if (header.getCurrentTask() == TaskID::FileSaveData)
    {
        if (header.commType == ResponseSuccess)
        {
            _waitForResponse = false;
            _retries = 0;
        }
        else if (header.commType == ResponseFailed)
        {
            //Only try sending packet again for a limited time, then just fail
            _retries++;
            if (_retries > 3)
            {
                uint8_t errorCode = (uint8_t)DownloadError::UnknownError;
                Application.getMK20Stack()->requestTask(TaskID::DownloadError,sizeof(uint8_t),&errorCode);

                Mode* mode = new Idle();
                Application.pushMode(mode);
                return false;
            }

            //Sending data has failed, just send the packet again
            onDataReceived(getBuffer(),getBufferIndex());
        }
    }

    return true;
}

