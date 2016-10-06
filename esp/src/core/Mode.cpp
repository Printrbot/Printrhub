#include "Mode.h"

Mode::Mode()
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