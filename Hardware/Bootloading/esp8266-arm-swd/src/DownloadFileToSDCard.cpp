//
// Created by Phillip Schuster on 12.07.16.
//

#include "DownloadFileToSDCard.h"
#include "core/Application.h"
#include "Idle.h"

// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;

DownloadFileToSDCard::DownloadFileToSDCard(String filePath):
        Mode(),
        mode(StateWorking),
        httpClient(client),
        _filePath(filePath)
{
    memset(_buffer,0,_bufferSize);
    _bufferIndex = 0;
    _numChunks = 0;
}

DownloadFileToSDCard::~DownloadFileToSDCard()
{
}

String DownloadFileToSDCard::getName()
{
    return "DownloadFileToSDCard";
}

void DownloadFileToSDCard::onWillStart()
{

}

void DownloadFileToSDCard::onWillEnd()
{

}

void DownloadFileToSDCard::sendError(String errorMessage)
{
    LOG_VALUE("Failed to send to SD card",errorMessage);
    Application.getMK20Stack()->requestTask(Error);
}

void DownloadFileToSDCard::addByteToBuffer(uint8_t byte)
{
    _buffer[_bufferIndex] = byte;

    _bufferIndex++;
    if (_bufferIndex >= _bufferSize)
    {
        sendBuffer();
        memset(_buffer,0,_bufferSize);
        _bufferIndex = 0;
    }
}

void DownloadFileToSDCard::sendBuffer()
{
    if (_bufferIndex <= 0)
    {
        //No data to send
        return;
    }

    //Send Request for sending data
    Application.getMK20Stack()->requestTask(FileSendData,_bufferIndex, (uint8_t*)_buffer);

/*    _numChunks++;
    if (_numChunks > 3)
    {
        Mode* mode = new Idle();
        Application.pushMode(mode);
        return;
    }*/
}

void DownloadFileToSDCard::sendResponse(uint32_t contentLength)
{
    Application.getMK20Stack()->responseTask(GetJobWithID,sizeof(uint32_t),(uint8_t*)&contentLength);
}

void DownloadFileToSDCard::loop()
{
    int err = 0;

    if (mode == StateWorking)
    {
        if (httpClient.get("www.appfruits.com", 80, _filePath.c_str()) != 0)
        {
            mode = StateError;
        }
        else
        {
            err = httpClient.responseStatusCode();
            if (err >= 0)
            {
                // Usually you'd check that the response code is 200 or a
                // similar "success" code (200-299) before carrying on,
                // but we'll print out whatever response we get

                err = httpClient.skipResponseHeaders();
                if (err >= 0)
                {
                    int bodyLen = httpClient.contentLength();

                    //Send the response with the size of the file
                    sendResponse(bodyLen);

                    // Now we've got to the body, so we can print it out
                    unsigned long timeoutStart = millis();
                    char c;

                    while ((httpClient.connected() || httpClient.available()) &&
                           ((millis() - timeoutStart) < kNetworkTimeout))
                    {
                        ESP.wdtFeed();

                        if (httpClient.available())
                        {
                            c = httpClient.read();
                            // Print out this character

                            addByteToBuffer(c);

                            bodyLen--;
                            // We read something, reset the timeout counter
                            timeoutStart = millis();
                        }
                        else
                        {
                            // We haven't got any data, so let's pause to allow some to
                            // arrive
                            delay(kNetworkDelay);
                        }
                    }

                    //All data read from the net, make sure we send the rest of the buffer
                    sendBuffer();

                    mode = StateSuccess;
                }
                else
                {
                    mode = StateError;
                }
            }
        }
    }

    if (mode == StateError)
    {
        sendError("Failed to download file");

        Mode* mode = new Idle();
        Application.pushMode(mode);
        return;
    }

    if (mode == StateSuccess)
    {
        //Send File Close message
        //Send Request for sending data
        Application.getMK20Stack()->requestTask(FileClose);

        Mode* mode = new Idle();
        Application.pushMode(mode);
        return;
    }
}