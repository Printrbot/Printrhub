#ifndef ESPFIRMWAREUPDATE_H
#define ESPFIRMWAREUPDATE_H

#include "core/Mode.h"

class ESPFirmwareUpdate: public Mode
{
private:
  typedef enum State {
    StateFlashFirmware = 0,
    StateSuccess = 1,
    StateError = 2
  };

public:
  ESPFirmwareUpdate(String url);
  virtual ~ESPFirmwareUpdate();

  virtual void loop();
  virtual void onWillStart();
  virtual void onWillEnd();
  virtual bool handlesTask(TaskID taskID);
  virtual String getName();

private:
  void flashFirmware();
  State _state;
  String _url;
};


#endif
