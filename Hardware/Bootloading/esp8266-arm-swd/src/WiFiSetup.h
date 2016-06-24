//
// Created by Phillip Schuster on 24.06.16.
//

#ifndef ESP8266_ARM_SWD_WIFISETUP_H
#define ESP8266_ARM_SWD_WIFISETUP_H

#include "core/Mode.h"

class WiFiSetup: public Mode
{
#pragma mark Constructor
public:
    WiFiSetup();
    virtual ~WiFiSetup();

#pragma mark Application Flow
    virtual void loop();
    virtual void onWillStart();
    virtual void onWillEnd();

#pragma mark private member functions
    void setupWiFi();

#pragma mark Mode
    virtual String getName();

private:
    WiFiServer _server;
};


#endif //ESP8266_ARM_SWD_WIFISETUP_H
