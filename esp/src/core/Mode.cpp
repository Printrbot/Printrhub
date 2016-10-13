#include "Mode.h"

Mode::Mode():
_nextMode(NULL)
{

}

Mode::~Mode()
{

}

void Mode::loop()
{

}

void Mode::onWillStart()
{

}

void Mode::onWillEnd()
{

}

bool Mode::handlesTask(TaskID taskID)
{
    return false;
}

bool Mode::runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool* success)
{
    return false;
}

void Mode::setNextMode(Mode *mode)
{
    _nextMode = mode;
}

void Mode::exit()
{
    if (_nextMode == NULL)
    {
        EventLogger::log("Exiting mode %s to idle",getName().c_str());
        Application.idle();
    }
    else
    {
        EventLogger::log("Exiting mode %s to %s",getName().c_str(),_nextMode->getName().c_str());
        Application.pushMode(_nextMode);
    }
}

void Mode::exitWithError(DownloadError error)
{
    EventLogger::log("Exiting mode %s with error %d",getName().c_str(),error);
    Application.handleError(error);
}