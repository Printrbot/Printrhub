//
// Created by Phillip Schuster on 12.07.16.
//

#include "DownloadFileToSDCard.h"
#include "Application.h"
#include "Idle.h"
#include "../core/CommStack.h"

// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 10*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 100;

DownloadFileToSDCard::DownloadFileToSDCard(String url):
        Mode(),
        mode(StateRequest),
        httpClient(client),
        _url(url)
{
    memset(_buffer,0,_bufferSize);
    _bufferIndex = 0;
    _numChunks = 0;
    _waitForResponse = false;

    parseUrl();

    client.setNoDelay(true);
}

DownloadFileToSDCard::~DownloadFileToSDCard()
{
}

bool DownloadFileToSDCard::parseUrl()
{
    // parse protocol
    int i = _url.indexOf(":");
    if (i < 0) return false;

    _protocol = _url.substring(0, i);
    _url.remove(0, (i + 3));

    // parse host
    i = _url.indexOf('/');
    _host = _url.substring(0, i);
    _url.remove(0, i);

    // parse port
    i = _host.indexOf(':');
    if (i > 0)
    {
        EventLogger::log("EXTRACTING PORT");
        EventLogger::log(_host.substring(i + 1).c_str());
        _port = _host.substring(i + 1).toInt();
        _host.remove(i);
    } else
    {
        _port = 80;
    }
    // parse path
    _path = _url;

    EventLogger::log(_protocol.c_str());
    EventLogger::log(_host.c_str());
    EventLogger::log(_url.c_str());

    char _p[6];
    sprintf(_p, "%05i", _port);
    EventLogger::log(_p);

    return true;
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
    Application.getMK20Stack()->requestTask(TaskID::Error);
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
    Application.getMK20Stack()->requestTask(TaskID::FileSaveData,_bufferIndex, (uint8_t*)_buffer);

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
    Application.getMK20Stack()->responseTask(TaskID::DownloadFile,sizeof(uint32_t),(uint8_t*)&contentLength,true);
}

void DownloadFileToSDCard::loop()
{
    int err = 0;

    //In this mode ESP will try to initiate the download of the requested file and will send the response with number of bytes to download
    if (mode == StateRequest)
    {
        if (httpClient.get(_host.c_str(), _port, _path.c_str()) != 0)
        {
            mode = StateError;
            _error = DownloadError::ConnectionFailed;
        }
        else
        {
            err = httpClient.responseStatusCode();
            if (err >= 200 && err <= 299)
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
                    _lastBytesReadTimeStamp = millis();
                }
                else
                {
                    mode = StateError;
                    _error = DownloadError::UnknownError;
                }
            }
            else if (err >= 300 && err <= 399)
            {
                mode = StateError;
                _error = DownloadError::Forbidden;
            }
            else if (err >= 400 && err <= 499)
            {
                mode = StateError;
                _error = DownloadError::FileNotFound;
            }
            else if (err >= 500 && err <= 599)
            {
                mode = StateError;
                _error = DownloadError::InternalServerError;
            }
            else
            {
                mode = StateError;
                _error = DownloadError::UnknownError;
            }
        }
    }

    //In this mode ESP will download the file by chunks of _bufferSize (32 bytes) and will then leave the loop to allow for responses
    if (mode == StateDownload)
    {
        // Now we've got to the body, so we can print it out
        char c;

        //If we are still waiting for the response of the last sent package do nothing
        if (_waitForResponse)
        {
            return;
        }

        if (!httpClient.connected())
        {
            LOG("HTTPClient lost connection");
            LOG_VALUE("Bytes left to download",_bytesToDownload);
            mode = StateError;
            _error = DownloadError::Timeout;
            return;
        }

        if (!httpClient.available())
        {
            digitalWrite(COMMSTACK_INFO_MARKER_PIN,LOW);
        }
        else
        {
            digitalWrite(COMMSTACK_INFO_MARKER_PIN,HIGH);
        }

        LOG("Downloading data from the net");
        while (httpClient.available())
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
                    sendBuffer();
                    memset(_buffer,0,_bufferSize);
                    _bufferIndex = 0;

                    //Close this run loop now to keep up the rest of the application loop and to wait for the response
                    return;
                }


                // We read something, reset the timeout counter
                _lastBytesReadTimeStamp = millis();
            }
            else
            {
                // We haven't got any data, so let's pause to allow some to
                // arrive
                digitalWrite(COMMSTACK_WORKING_MARKER_PIN,LOW);
                delay(kNetworkDelay);
                digitalWrite(COMMSTACK_WORKING_MARKER_PIN,HIGH);
            }
        }

        //All data read from the net, make sure we send the rest of the buffer
        if (_bytesToDownload <= 0)
        {
            _waitForResponse = true;
            sendBuffer();
            mode = StateSuccess;
        }
        else
        {
            //Check for timeout
            if ((millis() - _lastBytesReadTimeStamp) > kNetworkTimeout)
            {
                LOG("Connection timeout");
                mode = StateError;
                _error = DownloadError::Timeout;
            }
        }
    }

    if (mode == StateError)
    {
        sendError("Failed to download file");

        //Send the error to MK20
        uint8_t errorCode = (uint8_t)_error;
        Application.getMK20Stack()->requestTask(TaskID::DownloadError,sizeof(uint8_t),&errorCode);

        Mode* mode = new Idle();
        Application.pushMode(mode);
        return;
    }

    if (mode == StateSuccess)
    {
        //Send File Close message
        //Send Request for sending data
        Application.getMK20Stack()->requestTask(TaskID::FileClose);

        Mode* mode = new Idle();
        Application.pushMode(mode);
        return;
    }
}

bool DownloadFileToSDCard::handlesTask(TaskID taskID)
{
    if (taskID == TaskID::FileSaveData)
    {
        return true;
    }

    return false;
}

bool DownloadFileToSDCard::runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool* success)
{
    if (header.getCurrentTask() == TaskID::FileSaveData)
    {
        if (header.commType == ResponseSuccess)
        {
            _waitForResponse = false;

            digitalWrite(COMMSTACK_WORKING_MARKER_PIN,LOW);
            delayMicroseconds(2);
            digitalWrite(COMMSTACK_WORKING_MARKER_PIN,HIGH);
        }
    }

    return true;
}