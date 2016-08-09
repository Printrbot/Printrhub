//
// Created by Phillip Schuster on 24.06.16.
//

#ifndef MK20FIRMWAREUPDATE_H
#define MK20FIRMWAREUPDATE_H

#include "core/Mode.h"
#include "arm_kinetis_debug.h"

class MK20FirmwareUpdate: public Mode
{
private:
  typedef enum State {
    StateDownloadFirmware = 0,
    StateFlashFirmware = 1,
    StateTestFirmware = 2,
    StateSuccess = 3,
    StateError = 4
  };

public:
  MK20FirmwareUpdate(String url);
  virtual ~MK20FirmwareUpdate();

  virtual void loop();
  virtual void onWillStart();
  virtual void onWillEnd();
  virtual bool handlesTask(TaskID taskID);
  virtual String getName();

private:
  void downloadFirmware();
  void flashFirmware();
  State _state;
  String _url;
  ARMKinetisDebug target;
  File firmware_file;
};


#endif //MK20FIRMWAREUPDATE_H
