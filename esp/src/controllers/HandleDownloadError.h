//
// Created by Phillip Schuster on 05.10.16.
//

#ifndef ESP_DOWNLOADERROR_H
#define ESP_DOWNLOADERROR_H

#include "../errors.h"
#include "core/Mode.h"

class HandleDownloadError: public Mode
{
#pragma mark Constructor
public:
    HandleDownloadError(DownloadError error);
    virtual ~HandleDownloadError();

#pragma mark Application Flow
    virtual void loop();
    virtual void onWillStart();
    virtual void onWillEnd();
#pragma mark Mode
    virtual String getName();

private:
    DownloadError _error;
};


#endif //ESP_DOWNLOADERROR_H
