//
// Created by Phillip Schuster on 05.10.16.
//

#include "FirmwareError.h"
#include "Idle.h"

FirmwareError::FirmwareError(FirmwareUpdateError error):
_error(error)
{

}

FirmwareError::~FirmwareError()
{

}

void FirmwareError::loop()
{

}

void FirmwareError::onWillStart()
{
    EventLogger::log("Starting Firmware Error mode...");

    uint8_t errorCode = (uint8_t)_error;
    Application.getMK20Stack()->requestTask(TaskID::FirmwareUpdateError,sizeof(uint8_t),&errorCode);

    Idle* idle = new Idle();
    Application.pushMode(idle);
}

void FirmwareError::onWillEnd()
{

}

String FirmwareError::getName()
{
    return "FirmwareError";
}
