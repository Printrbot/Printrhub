//
// Created by Phillip Schuster on 09.07.16.
//

#ifndef ESP8266_ARM_SWD_TASK_H
#define ESP8266_ARM_SWD_TASK_H


class Task
{
#pragma mark Constructor
public:
    Task();
    virtual ~Task();

#pragma mark Member Functions
public:
    bool processRequest();
    bool processResponse();
};


#endif //ESP8266_ARM_SWD_TASK_H
