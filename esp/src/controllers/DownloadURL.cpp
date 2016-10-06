//
// Created by Phillip Schuster on 05.10.16.
//

#include "DownloadURL.h"
#include "Application.h"
#include "Idle.h"
#include "../core/CommStack.h"

// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 10*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 100;

DownloadURL::DownloadURL(String url):
        Mode(),
        mode(StateRequest),
        httpClient(client),
        _url(url)
{
    memset(_buffer,0,_bufferSize);
    _bufferIndex = 0;
    _numChunks = 0;
    _retries = 0;

    parseUrl();

    client.setNoDelay(true);
}

DownloadURL::~DownloadURL()
{
}

bool DownloadURL::parseUrl()
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

void DownloadURL::loop()
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

                    onBeginDownload(_bytesToDownload);

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
                    onDataReceived(_buffer,_bufferIndex);
                    _bufferIndex = 0;
                }

                // We read something, reset the timeout counter
                _lastBytesReadTimeStamp = millis();
            }
            else
            {
                return;
            }
        }

        //All data read from the net, make sure we send the rest of the buffer
        if (_bytesToDownload <= 0)
        {
            onDataReceived(_buffer,_bufferIndex);
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
        //Send the error to MK20
        onError(_error);
        return;
    }

    if (mode == StateSuccess)
    {
        onFinished();
        return;
    }
}