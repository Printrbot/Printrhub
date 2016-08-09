#ifndef RUNGCODE_H
#define RUNGCODE_H

#include "core/Mode.h"

class Template: public Mode
{
private:
  typedef enum State {
    StateSuccess = 1,
    StateError = 2,
    StateWaiting = 3
  };

TaskID AvailableTasks[0] {  };

public:
  Template();


  virtual void loop();
  virtual void onWillStart();
  virtual void onWillEnd();

  virtual String getName();

  bool runTask(CommHeader& header, const uint8_t* data, size_t dataSize, uint8_t* responseData, uint16_t* responseDataSize, bool* sendResponse, bool* success);


private:
  State _state;
};


#endif
