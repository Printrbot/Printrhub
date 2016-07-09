//
// Created by Phillip Schuster on 24.06.16.
//

#ifndef ESP8266_ARM_SWD_MK20FIRMWAREUPDATE_H
#define ESP8266_ARM_SWD_MK20FIRMWAREUPDATE_H

#include "core/Mode.h"
#include "arm_kinetis_debug.h"
#include <HttpClient.h>

class MK20FirmwareUpdate: public Mode
{
private:
    typedef enum State {
        StateDownloadFirmware = 0,
        StateFlashFirmware = 1,
        StateTestFirmware = 2,
        StateSuccess = 3,
        StateError = 4
    };

#pragma mark Constructor
public:
    MK20FirmwareUpdate();
    virtual ~MK20FirmwareUpdate();

#pragma mark Application Flow
    virtual void loop();
    virtual void onWillStart();
    virtual void onWillEnd();

#pragma mark private member functions
    void setupWiFi();
    void flash();

#pragma mark Mode
    virtual String getName();

#pragma mark private member variables
private:
    State mode;
    ARMKinetisDebug target;
    File firmware_file;
    WiFiClient client;
    HttpClient httpClient;
};


#endif //ESP8266_ARM_SWD_MK20FIRMWAREUPDATE_H
