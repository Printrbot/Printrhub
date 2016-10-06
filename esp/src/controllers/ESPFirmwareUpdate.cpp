#include "ESPFirmwareUpdate.h"
#include "Idle.h"
#include "Application.h"
#include "../event_logger.h"
#include <ESP8266httpUpdate.h>
#include "MK20FirmwareUpdate.h"

ESPFirmwareUpdate::ESPFirmwareUpdate(String url):
Mode(),
_url(url),
_state(StateFlashFirmware)
{
}

ESPFirmwareUpdate::~ESPFirmwareUpdate() {
}

String ESPFirmwareUpdate::getName() {
  return "ESPFirmwareUpdate";
}

void ESPFirmwareUpdate::onWillStart() {
}

void ESPFirmwareUpdate::onWillEnd() {
}

bool ESPFirmwareUpdate::handlesTask(TaskID taskID) {
  switch(taskID) {
    case TaskID::UpdateESPFirmware:
      return true;
      break;
    default:
      return false;
  }
}

void ESPFirmwareUpdate::flashFirmware() {
  EventLogger::log("IN SERVER UPDATE LOOP");
  t_httpUpdate_return ret = ESPhttpUpdate.update(_url);
  switch(ret) {
    case HTTP_UPDATE_FAILED: {
      String s = ESPhttpUpdate.getLastErrorString().c_str();
      char sb[128];
      s.toCharArray(sb, 128, 0);
      EventLogger::log(sb);
      }
      _state = StateError;
      break;

    case HTTP_UPDATE_NO_UPDATES:
      EventLogger::log("NO UPDATES\n\n");
      _state = StateError;
      break;

    case HTTP_UPDATE_OK:
      _state = StateSuccess;
      EventLogger::log("UPDATE OK");
      break;
  }
}

void ESPFirmwareUpdate::loop() {

  if (_state == StateFlashFirmware) {
    flashFirmware();
    return;
  }

  if (_state == StateError) {
    Mode* mode = new Idle();
    Application.pushMode(mode);
    return;
  }

  if (_state == StateSuccess) {

    MK20FirmwareUpdate* mode = new MK20FirmwareUpdate(FIRMWARE_URL_MK20);
    Application.pushMode(mode);

    return;
  }
}
