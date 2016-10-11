//
// Created by Phillip Schuster on 11.10.16.
//

#include "ReceiveSDCardFile.h"
#include "SD.h"

ReceiveSDCardFile::ReceiveSDCardFile(String localFilePath, size_t fileSize):
BackgroundJob()
{
  _localFile = SD.open(localFilePath.c_str(),FILE_WRITE);
  if (!_localFile) {
    exit();
  }
}

ReceiveSDCardFile::~ReceiveSDCardFile() {
  if (_localFile) {
    _localFile.close();
  }
}

bool ReceiveSDCardFile::handlesTask(TaskID taskID) {
  if (taskID == TaskID::FileOpenForWrite) {
    return true;
  }
  else if (taskID == TaskID::FileSaveData) {
    return true;
  }
  else if (taskID == TaskID::FileClose) {
    return true;
  }

  return false;
}

bool ReceiveSDCardFile::runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool *success) {
  if (header.getCurrentTask() == TaskID::FileSaveData) {
    if (header.commType == Request) {

      int numBytesWritten = _localFile.write(data, dataSize);
      LOG_VALUE("Written number of bytes to file",numBytesWritten);

      //Send a response
      *sendResponse = true;
      *responseDataSize = 0;
    }
  } else if (header.getCurrentTask() == TaskID::FileClose) {
    if (header.commType == Request) {
      //Close local file
      _localeFile.close();
      //Don't send a response
      *sendResponse = false;

      //Exit this job, we are done
      exit();
    }
  }
}