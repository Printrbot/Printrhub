//
// Created by Phillip Schuster on 11.10.16.
//

#ifndef MK20_RECEIVESDCARDFILE_H
#define MK20_RECEIVESDCARDFILE_H

#include "../framework/core/BackgroundJob.h"
#include "SD.h"

class ReceiveSDCardFile: public BackgroundJob {
public:
  ReceiveSDCardFile(String localFilePath, size_t fileSize, Compression compression);
  ~ReceiveSDCardFile();

  virtual bool RLE16Deflate(const uint8_t* data, size_t size);
  virtual bool writeToFile(const uint8_t* data, size_t size);
  virtual bool onDataReceived(const uint8_t* data, size_t size);

  virtual bool isIndeterminate(); //Default = true, i.e. no progress available
  virtual float fractionCompleted();
  virtual bool runTask(CommHeader& header, const uint8_t* data, size_t dataSize, uint8_t* responseData, uint16_t* responseDataSize, bool* sendResponse, bool* success);
  virtual bool handlesTask(TaskID taskID);
  virtual String getName();
  virtual void onWillStart();

private:
    File _localFile;
    Compression _compression;
    size_t _fileSize;
    size_t _bytesLeft;
    String _localFilePath;
};


#endif //MK20_RECEIVESDCARDFILE_H
