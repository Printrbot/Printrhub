//
// Created by Phillip Schuster on 12.07.16.
//

#include "DownloadFileToSDCard.h"
#include "core/Application.h"
#include "Idle.h"

// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 100;

DownloadFileToSDCard::DownloadFileToSDCard(String filePath):
        Mode(),
        mode(StateRequest),
        httpClient(client),
        _filePath(filePath)
{
    memset(_buffer,0,_bufferSize);
    _bufferIndex = 0;
    _numChunks = 0;
    _waitForResponse = false;
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
    Application.getMK20Stack()->responseTask(GetJobWithID,sizeof(uint32_t),(uint8_t*)&contentLength,true);
}

void DownloadFileToSDCard::loop()
{
    int err = 0;

    //In this mode ESP will try to initiate the download of the requested file and will send the response with number of bytes to download
    if (mode == StateRequest)
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
                    _bytesToDownload = httpClient.contentLength();

                    //Send the response with the size of the file
                    sendResponse(_bytesToDownload);

                    //Switch to download mode
                    _waitForResponse = false;
                    mode = StateDownload;
                }
                else
                {
                    mode = StateError;
                }
            }
            else
            {
                mode = StateError;
            }
        }
    }

    //In this mode ESP will download the file by chunks of _bufferSize (32 bytes) and will then leave the loop to allow for responses
    digitalWrite(13,LOW);
    if (mode == StateDownload)
    {
        // Now we've got to the body, so we can print it out
        unsigned long timeoutStart = millis();
        char c;

        //If we are still waiting for the response of the last sent package do nothing
        if (_waitForResponse)
        {
            digitalWrite(13,HIGH);
            return;
        }

        while ((httpClient.connected() || httpClient.available()) &&
               ((millis() - timeoutStart) < kNetworkTimeout))
        {
            ESP.wdtFeed();

            if (httpClient.available())
            {
                c = httpClient.read();
                _bytesToDownload--;

                //Store the byte read from the web in our buffer
                _buffer[_bufferIndex] = c;

                _bufferIndex++;
                if (_bufferIndex >= _bufferSize)
                {
                    //Buffer is full, send it to MK20 and leave the loop
                    _waitForResponse = true;
                    digitalWrite(13,HIGH);
                    delayMicroseconds(1);
                    digitalWrite(13,LOW);
                    sendBuffer();
                    memset(_buffer,0,_bufferSize);
                    _bufferIndex = 0;

                    digitalWrite(13,HIGH);
                    delayMicroseconds(1);
                    digitalWrite(13,LOW);
                    delayMicroseconds(1);
                    digitalWrite(13,HIGH);
                    delayMicroseconds(1);
                    digitalWrite(13,LOW);

                    //Close this run loop now to keep up the rest of the application loop and to wait for the response
                    break;
                }


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
        if (_bytesToDownload <= 0)
        {
            _waitForResponse = true;
            sendBuffer();
            mode = StateSuccess;
        }
        digitalWrite(13,HIGH);
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

bool DownloadFileToSDCard::handlesTask(TaskID taskID)
{
    if (taskID == FileSendData)
    {
        return true;
    }

    return false;
}

bool DownloadFileToSDCard::runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool* success)
{
    if (header.getCurrentTask() == FileSendData)
    {
        if (header.commType == ResponseSuccess)
        {
            _waitForResponse = false;
        }
    }

    return true;
}