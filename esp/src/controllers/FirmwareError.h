//
// Created by Phillip Schuster on 05.10.16.
//

#ifndef ESP_FIRMWAREERROR_H
#define ESP_FIRMWAREERROR_H

#include "core/Mode.h"
#include "../errors.h"

class FirmwareError : public Mode
{
#pragma mark Constructor
public:
    FirmwareError(FirmwareUpdateError);
    virtual ~FirmwareError();

#pragma mark Application Flow
    virtual void loop();
    virtual void onWillStart();
    virtual void onWillEnd();
#pragma mark Mode
    virtual String getName();

private:
    FirmwareUpdateError _error;
};


#endif //ESP_FIRMWAREERROR_H
