//
// Created by Phillip Schuster on 09.07.16.
//

#include "CommStack.h"
#include "Application.h"

CommStack::CommStack(Stream* port, CommStackDelegate* delegate):
_port(port),
_delegate(delegate)
{ }

CommStack::~CommStack()
{ }

bool CommStack::readHeader(CommHeader* commHeader)
{
    int numBytes = _port->readBytes((uint8_t*)commHeader,sizeof(CommHeader));
    if (numBytes < sizeof(CommHeader))
    {
        return false;
    }

    return true;
}

bool CommStack::prepareResponse(CommHeader *commHeader)
{
    //Flip Request to Response, if this is a Response switch to next task. If all tasks
    //have been processed just return true and do nothing
    if (commHeader->commType == Request)
    {
        commHeader->commType = Response;
    }
    else
    {
        commHeader->commType = Response;
        commHeader->currentTaskIndex++;

        if (commHeader->isFinished())
        {
            return false;
        }
    }

    return true;
}

bool CommStack::writeHeader(CommHeader* commHeader)
{
    int numBytes = _port->write((uint8_t*)commHeader,sizeof(CommHeader));
    if (numBytes < sizeof(CommHeader))
    {
        return false;
    }

    return true;
}

void CommStack::process()
{
    if (_port->available())
    {
        LOG("CommStack: Data Available.");

        CommHeader header;
        if (readHeader(&header))
        {
            LOG("Header sucessfully read");
            LOG_VALUE("Task-ID",header.getCurrentTask());
            LOG_VALUE("Comm-Type",header.commType);
            if (_delegate->canRunTask(header))
            {
                LOG("Task can be processed");
                //Copy request header
                CommHeader responseHeader = header;

                //Prepare header for return
                if (prepareResponse(&responseHeader))
                {
                    LOG("Prepared Header");
                    //Task initiated, answer with the same header that is flipped over and sent back
                    if (writeHeader(&responseHeader))
                    {
                        LOG("Header written, running task");
                        //We got the header, run the task
                        _delegate->runTask(header,_port);
                    }
                }
            }
        }
    }
}

bool CommStack::requestTask(TaskID task)
{
    CommHeader header(task);
    return writeHeader(&header);
}