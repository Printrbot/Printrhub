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
        _url(url),
        _stream(NULL)
{
    memset(_buffer,0,_bufferSize);
    _bufferIndex = 0;
    _numChunks = 0;
    _retries = 0;
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
        if (!_httpClient.begin(_url))
        {
            EventLogger::log("Download failed, could not parse URL: %s",_url.c_str());
            mode = StateError;
            _error = DownloadError::ConnectionFailed;
        }
        else
        {
            err = _httpClient.GET();
            if (err >= 200 && err <= 299)
            {
                _stream = _httpClient.getStreamPtr();
                _bytesToDownload = _httpClient.getSize();

                EventLogger::log("Begin Download from %s, size: %d",_url.c_str(),_bytesToDownload);

                if (_stream == NULL || _bytesToDownload == 0)
                {
                    mode = StateError;
                    _error = DownloadError::UnknownError;
                }
                else
                {
                    onBeginDownload(_bytesToDownload);

                    mode = StateDownload;
                    _lastBytesReadTimeStamp = millis();
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
        while(_httpClient.connected() && (_bytesToDownload > 0 || _bytesToDownload == -1)) {
            // get available data size
            size_t size = _stream->available();
            if(size) {
                // read up to 128 byte
                int c = _stream->readBytes(_buffer, ((size > _bufferSize) ? _bufferSize : size));

                //EventLogger::log("Data received, size: %d, bytes left: %d",c,_bytesToDownload);

                onDataReceived(_buffer,c);
                _bytesToDownload -= c;

                _lastBytesReadTimeStamp = millis();
            }
            else
            {
                //Check for timeout
                if ((millis() - _lastBytesReadTimeStamp) > kNetworkTimeout)
                {
                    EventLogger::log("Download failed, timeout");

                    LOG("Connection timeout");
                    mode = StateError;
                    _error = DownloadError::Timeout;
                    return;
                }

                delay(1);
            }
        }

        if (_bytesToDownload == 0)
        {
            mode = StateSuccess;
        }
        else
        {
            mode = StateError;
            _error = DownloadError::Timeout;
        }
    }

    if (mode == StateError)
    {
        //Send the error to MK20
        EventLogger::log("Download failed, Error-Code: %d",_error);
        onError(_error);
        return;
    }

    if (mode == StateSuccess)
    {
        EventLogger::log("Download successful");
        onFinished();
        return;
    }
}