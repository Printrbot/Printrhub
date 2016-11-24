/*
 * Downloads a file from a given URL and stores it in local flash memory (SPIFFS)
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/behind-the-scenes-printrbot-simple-2016/
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Phillip Schuster
 * https://github.com/Printrbot/Printrhub
 *
 * Developed in cooperation by Phillip Schuster (@appfruits) from appfruits.com
 * http://www.appfruits.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "DownloadFileToSPIFFs.h"
#include "Idle.h"
#include "HandleDownloadError.h"

DownloadFileToSPIFFs::DownloadFileToSPIFFs(String url, String localFilePath) :
	DownloadURL(url),
	_localFilePath(localFilePath) {}

DownloadFileToSPIFFs::~DownloadFileToSPIFFs() {
}

String DownloadFileToSPIFFs::getName() {
  return "DownloadFileToSPIFFs";
}

bool DownloadFileToSPIFFs::onBeginDownload(uint32_t expectedSize) {
  //_file = SPIFFS.open("/download.tmp", "w");
  EventLogger::log("Saving downloaded file in path %s", _localFilePath.c_str());
  _file = SPIFFS.open(_localFilePath, "w");
  if (!_file) {
	EventLogger::log("Failed to open file %s", _localFilePath.c_str());
	return false;
  }

  return true;
}

bool DownloadFileToSPIFFs::onDataReceived(uint8_t *data, uint16_t size) {
  _file.write(data, size);

  return true;
}

void DownloadFileToSPIFFs::onError(DownloadError errorCode) {
  HandleDownloadError *error = new HandleDownloadError(errorCode);
  Application.pushMode(error);
}

void DownloadFileToSPIFFs::onCancelled() {
  _file.close();
  exit();
}

void DownloadFileToSPIFFs::onFinished() {
  _file.close();

  //Delete existing file and rename downloaded file
/*    if (SPIFFS.exists(_localFilePath)) {
        Serial.println("File exists");
        if (!SPIFFS.remove(_localFilePath)) {
            Serial.println("Remove failed");
            HandleDownloadError* error = new HandleDownloadError(DownloadError::RemoveOldFilesFailed);
            Application.pushMode(error);
            return;
        }
    }

    if (!SPIFFS.rename("/download.tmp",_localFilePath)) {
        Serial.println("Rename failed");
        HandleDownloadError* error = new HandleDownloadError(DownloadError::PrepareDownloadedFileFailed);
        Application.pushMode(error);
        return;
    }*/

  exit();
}
