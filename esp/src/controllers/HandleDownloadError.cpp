//
// Created by Phillip Schuster on 05.10.16.
//

#include "HandleDownloadError.h"
#include "Idle.h"

HandleDownloadError::HandleDownloadError(DownloadError error):
Mode(),
_error(error)
{

}

HandleDownloadError::~HandleDownloadError()
{

}

void HandleDownloadError::loop()
{

}

void HandleDownloadError::onWillStart()
{
    EventLogger::log("Starting Download Error mode...");

    uint8_t errorCode = (uint8_t)_error;
    Application.getMK20Stack()->requestTask(TaskID::DownloadError,sizeof(uint8_t),&errorCode);

    Idle* idle = new Idle();
    Application.pushMode(idle);
}

void HandleDownloadError::onWillEnd()
{

}

String HandleDownloadError::getName()
{
    return "DownloadError";
}

