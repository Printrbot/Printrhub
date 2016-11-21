/*
 * Pushes a file from SPIFFS to SD card via CommStack. Sends data packages to MK20
 * that finally writes the data to SD card.
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

#include "PushFileToSDCard.h"
#include "../event_logger.h"

PushFileToSDCard::PushFileToSDCard(const String &localFilePath, const String &targetFilePath, bool showUI, Compression compression)
	:
	Mode(),
	_localFilePath(localFilePath),
	_targetFilePath(targetFilePath),
	_showUI(showUI),
	_waitForResponse(false),
	_fileOpen(false),
	_compression(compression) {

}

PushFileToSDCard::~PushFileToSDCard() {
  _localFile.close();
}

String PushFileToSDCard::getName() {
  return "PushFileToSDCard";
}

void PushFileToSDCard::onWillStart() {
  EventLogger::log("Request SD file write with path: %s", _targetFilePath.c_str());

  if (!SPIFFS.exists(_localFilePath)) {
	EventLogger::log("Local file does not exist: %s", _localFilePath.c_str());
	exitWithError(DownloadError::LocalFileNotFound);
  } else {
	_localFile = SPIFFS.open(_localFilePath, "r");
	if (!_localFile) {
	  EventLogger::log("Local file could not be opened for read: %s", _localFilePath.c_str());
	  exitWithError(DownloadError::LocalFileOpenForReadFailed);
	} else {
	  _bytesLeft = _localFile.size();
	  EventLogger::log("File opened successfully: %s, Bytes left to send: %d", _localFilePath.c_str(), _bytesLeft);

	  //Request MK20 to store a file on SD card
	  _requestTime = millis();
	  _waitForResponse = true;
	  Application.getMK20Stack()->openSDFileForWrite(_targetFilePath, _bytesLeft, _showUI, _compression);
	}
  }
}

void PushFileToSDCard::loop() {
  if (millis() - _requestTime > 20000) {
	//Timeouted
	//exitWithError(DownloadError::Timeout);
  }

  //Just send data once the file is open
  if (!_fileOpen) {
	return;
  }

  //We wait for a response of MK20
  if (_waitForResponse) {
	return;
  }

  //Send 128 bytes with each chunk of data if compression is None
  uint8_t chunkSize = 128;
  if (_compression == Compression::RLE16) {
	//Run length encoding 16 bit consists of 24 bit chunks, 8 bit for the counter and 16-bit for the value, so we choose a packet size dividable by 3
	chunkSize = 48;
  }

  uint8_t buffer[chunkSize];
  int numReadBytes = _localFile.read(buffer, _bytesLeft > chunkSize ? chunkSize : _bytesLeft);
  _bytesLeft -= numReadBytes;

  EventLogger::log("Sending %d bytes to SD card, bytes left: %d", numReadBytes, _bytesLeft);

  //Send bytes
  _requestTime = millis();
  _waitForResponse = true;
  Application.getMK20Stack()->sendSDFileData(buffer, numReadBytes);

  if (_bytesLeft <= 0) {
	EventLogger::log("Sending file complete");

	//File is completely transferred
	Application.getMK20Stack()->closeSDFile();

	//Close local file
	_localFile.close();

	//Exit this mode
	exit();
  }
}

bool PushFileToSDCard::handlesTask(TaskID taskID) {
  if (taskID == TaskID::FileOpenForWrite) {
	return true;
  } else if (taskID == TaskID::FileSaveData) {
	return true;
  }

  return false;
}

bool PushFileToSDCard::runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool *success) {
  if (header.getCurrentTask() == TaskID::FileOpenForWrite) {
	if (header.commType == ResponseSuccess) {
	  _waitForResponse = false;
	  _fileOpen = true;
	} else if (header.commType == ResponseFailed) {
	  _waitForResponse = false;
	  _fileOpen = false;
	  exitWithError(DownloadError::TargetFileOpenForWriteFailed);
	}

  } else if (header.getCurrentTask() == TaskID::FileSaveData) {
	_waitForResponse = false;
  }
}
