/*
 * Background job that handles receiving data via CommStack and writes them to the
 * SD card. Supports run length encoding for faster data transmission
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

#include "ReceiveSDCardFile.h"
#include "SD.h"
#include "../framework/core/Application.h"

ReceiveSDCardFile::ReceiveSDCardFile(String localFilePath, size_t fileSize, Compression compression) :
	BackgroundJob(),
	_compression(compression),
	_fileSize(fileSize),
	_bytesLeft(fileSize),
	_localFilePath(localFilePath) {

}

ReceiveSDCardFile::~ReceiveSDCardFile() {
  if (_localFile) {
	_localFile.close();
  }
}

void ReceiveSDCardFile::onWillStart() {
  _localFile = SD.open(_localFilePath.c_str(), O_WRITE | O_CREAT | O_TRUNC);
  if (!_localFile) {
	FLOW_ERROR("ReceiveSDCardFile: Could not open file file for writing: %s", _localFilePath.c_str());
	exit();
  } else {
	FLOW_NOTICE("ReceiveSDCardFile: Opened file for writing: %s", _localFilePath.c_str());
  }
}

bool ReceiveSDCardFile::handlesTask(TaskID taskID) {
  if (taskID == TaskID::FileSaveData) {
	return true;
  } else if (taskID == TaskID::FileClose) {
	return true;
  }

  return false;
}

String ReceiveSDCardFile::getName() {
  return "ReceiveSDCardFile";
}

bool ReceiveSDCardFile::isIndeterminate() {
  if (_fileSize <= 0) return true;
  return false;
}

float ReceiveSDCardFile::fractionCompleted() {
  if (_fileSize <= 0) return 0;
  return 1.0f - ((float) _bytesLeft / (float) _fileSize);
}

bool ReceiveSDCardFile::writeToFile(const uint8_t *data, size_t size) {
  int numBytesWritten = _localFile.write(data, size);
  _bytesLeft -= numBytesWritten;

  COMMSTACK_SPAM("ReceiveSDCardFile: Writing data to file, bytes left: %d", _bytesLeft);

  return true;
}

bool ReceiveSDCardFile::RLE16Deflate(const uint8_t *data, size_t size) {
  //Make sure size is dividable by 3 as run length encoding 16 bit consists of 24 bit chunks
  if (size % 3 != 0) {
	FLOW_ERROR("ReceiveSDCardFile: RLE data packet is not dividable by 3, size: %d", size);
	return false;
  }

  uint8_t counter;
  uint16_t value;
  uint8_t buffer[512];

  //Run length encoding consists of a byte with the counter, i.e. the number of times the next 16-bit value should be written to the file
  for (int i = 0; i < size; i = i + 3) {
	counter = data[i];
	memcpy(&value, &data[i + 1], sizeof(uint16_t));
	//Counter read, now write the following 16-bit values counter times
	for (int c = 0; c < counter; c++) {
	  memcpy(&buffer[c * sizeof(uint16_t)], &value, sizeof(uint16_t));
	}

	COMMSTACK_SPAM("ReceiveSDCardFile: Decompressed %d 16-bit values (%d) and writing them to the file", counter, value);

/*    for (int s=0;s<(sizeof(uint16_t)*counter);s++) {
      DebugSerial.print(s);
      DebugSerial.print(":");
      DebugSerial.println(buffer[s],HEX);
    }*/

	//Write buffer to file
	_localFile.write(buffer, sizeof(uint16_t) * counter);
  }
  _bytesLeft -= size;

  return true;
}

bool ReceiveSDCardFile::onDataReceived(const uint8_t *data, size_t size) {
  if (_compression == Compression::None) {
	return writeToFile(data, size);
  } else if (_compression == Compression::RLE16) {
	return RLE16Deflate(data, size);
  }

  return true;
}

bool ReceiveSDCardFile::runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool *success) {
  COMMSTACK_SPAM("ReceiveSDCardFile: Run task %d", header.getCurrentTask());

  if (header.getCurrentTask() == TaskID::FileSaveData) {
	if (header.commType == Request) {
	  COMMSTACK_SPAM("ReceiveSDCardFile: Received data with size: %d", dataSize);
	  if (onDataReceived(data, dataSize)) {
		//Send a response success
		*sendResponse = true;
		*responseDataSize = 0;
		*success = true;
	  } else {
		FLOW_ERROR("ReceiveSDCardFile: Could not write data to file, sending failed response");

		//Send a response failed
		*sendResponse = true;
		*responseDataSize = 0;
		*success = false;
	  }
	}
  } else if (header.getCurrentTask() == TaskID::FileClose) {
	if (header.commType == Request) {
	  FLOW_NOTICE("ReceiveSDCardFile: Closed file: %s", _localFilePath.c_str());
	  //Close local file
	  _localFile.close();
	  //Don't send a response
	  *sendResponse = false;

	  //Exit this job, we are done
	  exit();
	}
  }
}