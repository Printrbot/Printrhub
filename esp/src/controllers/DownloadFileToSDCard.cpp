/*
 * Downloads a file from a given URL and sends it via CommStack to MK20 that
 * finally stores the file on the SD card.
 *
 * Copyright (c) 2016 Printrbot Inc.
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

#include "DownloadFileToSDCard.h"
#include "Application.h"
#include "Idle.h"
#include "../core/CommStack.h"
#include "HandleDownloadError.h"

DownloadFileToSDCard::DownloadFileToSDCard(String url) :
	DownloadURL(url),
	_waitForResponse(false),
	_errorTime(0) {}

DownloadFileToSDCard::~DownloadFileToSDCard() {
}

String DownloadFileToSDCard::getName() {
  return "DownloadFileToSDCard";
}

bool DownloadFileToSDCard::onBeginDownload(uint32_t expectedSize) {
  Application.getMK20Stack()->responseTask(TaskID::DownloadFile, sizeof(uint32_t), (uint8_t *) &expectedSize, true);
  _errorTime = 0;
}

bool DownloadFileToSDCard::onDataReceived(uint8_t *data, uint16_t size) {
  //Save last data
  memcpy(_lastData, data, size);
  _lastDataSize = size;

  //We send the data to MK20 and wait for the response
  _waitForResponse = true;
  Application.getMK20Stack()->requestTask(TaskID::FileSaveData, size, data);

  return true;
}

bool DownloadFileToSDCard::readNextData() {
  if (_waitForResponse) return false;
  return true;
}

void DownloadFileToSDCard::onError(DownloadError errorCode) {
  HandleDownloadError *error = new HandleDownloadError(errorCode);
  Application.pushMode(error);
}

void DownloadFileToSDCard::onFinished() {
  Application.getMK20Stack()->requestTask(TaskID::FileClose);

  exit();
}

void DownloadFileToSDCard::onCancelled() {
  exit();
}

bool DownloadFileToSDCard::handlesTask(TaskID taskID) {
  if (taskID == TaskID::FileSaveData) {
	return true;
  } else if (taskID == TaskID::CancelDownload) {
	return true;
  }

  return false;
}

bool DownloadFileToSDCard::runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool *success) {
  if (header.getCurrentTask() == TaskID::FileSaveData) {
	if (header.commType == ResponseSuccess) {
	  _waitForResponse = false;
	  _errorTime = 0;
	} else if (header.commType == ResponseFailed) {
	  if (_errorTime <= 0) {
		_errorTime = millis();
	  }

	  //Try 10 seconds to test
	  if ((millis() - _errorTime) < 10000) {
		EventLogger::log("Response failed sending data, sending data again. Time spent retrying so far: %d", (millis()
			- _errorTime));

		//Sending data has failed, just send the packet again
		_waitForResponse = true;
		Application.getMK20Stack()->requestTask(TaskID::FileSaveData, _lastDataSize, _lastData);
	  } else {
		EventLogger::log("Response failed timeout, canceling download");

		uint8_t errorCode = (uint8_t) DownloadError::UnknownError;
		Application.getMK20Stack()->requestTask(TaskID::DownloadError, sizeof(uint8_t), &errorCode);

		Mode *mode = new Idle();
		Application.pushMode(mode);
		return false;
	  }
	}
  } else if (header.getCurrentTask() == TaskID::CancelDownload) {
	if (header.commType == Request) {
	  cancelDownload();
	  *sendResponse = false;
	}
  }

  return true;
}

