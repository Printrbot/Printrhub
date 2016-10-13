//
// Created by Phillip Schuster on 11.10.16.
//

#ifndef MK20_BACKGROUNDJOB_H
#define MK20_BACKGROUNDJOB_H

#include "CommStack.h"

class BackgroundJob {
public:
    BackgroundJob();
    virtual ~BackgroundJob();

public:
    virtual void loop();
    virtual void onWillStart();
    virtual void onWillEnd();
    virtual void exit();
    virtual bool isFinished() { return _finished; };
    virtual String getName() = 0;

    virtual bool isIndeterminate(); //Default = true, i.e. no progress available
    virtual float fractionCompleted();

    virtual bool handlesTask(TaskID taskID);
    virtual bool runTask(CommHeader& header, const uint8_t* data, size_t dataSize, uint8_t* responseData, uint16_t* responseDataSize, bool* sendResponse, bool* success);

private:
    bool _finished;
};


#endif //MK20_BACKGROUNDJOB_H
