//
// Created by Phillip Schuster on 11.10.16.
//

#ifndef MK20_RECEIVESDCARDFILE_H
#define MK20_RECEIVESDCARDFILE_H

#include "../framework/core/BackgroundJob.h"

class ReceiveSDCardFile: public BackgroundJob {
public:
  ReceiveSDCardFile(String localFilePath, size_t fileSize);
  ~ReceiveSDCardFile();

  virtual void loop();
  virtual bool runTask(CommHeader& header, const uint8_t* data, size_t dataSize, uint8_t* responseData, uint16_t* responseDataSize, bool* sendResponse, bool* success);
  virtual bool handlesTask(TaskID taskID);

private:
    File _localFile;
};


#endif //MK20_RECEIVESDCARDFILE_H
