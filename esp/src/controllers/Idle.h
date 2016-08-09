//
// Created by Phillip Schuster on 25.06.16.
//

#ifndef ESP8266_ARM_SWD_IDLE_H
#define ESP8266_ARM_SWD_IDLE_H

#include "core/Mode.h"

class Idle: public Mode
{
#pragma mark Constructor
public:
    Idle();
    virtual ~Idle();

#pragma mark Application Flow
    virtual void loop();
    virtual void onWillStart();
    virtual void onWillEnd();
#pragma mark Mode
    virtual String getName();

private:

};


#endif //ESP8266_ARM_SWD_IDLE_H
