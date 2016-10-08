#include "ESPFirmwareUpdate.h"
#include "Idle.h"
#include "Application.h"
#include "../event_logger.h"
#include <ESP8266httpUpdate.h>
#include "MK20FirmwareUpdate.h"

ESPFirmwareUpdate::ESPFirmwareUpdate(String url):
Mode(),
_url(url)
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
  ESPhttpUpdate.rebootOnUpdate(false);
  t_httpUpdate_return ret = ESPhttpUpdate.update(_url);

  if (ret == HTTP_UPDATE_NO_UPDATES)
  {
      EventLogger::log("ESP Update failed, no updates");
      delay(100);
      exitWithError(DownloadError::ESPUpdateNoUpdates);
      return;
  }
  else if (ret == HTTP_UPDATE_FAILED)
  {
      EventLogger::log("ESP Update failed: %s",ESPhttpUpdate.getLastErrorString().c_str());
      delay(100);
      exitWithError(DownloadError::ESPUpdateFailed);
      return;
  }
  else if (ret == HTTP_UPDATE_OK)
  {
      EventLogger::log("ESP Update successful, restarting ESP");
      delay(100);

      //Just restart ESP
      Application.getMK20Stack()->requestTask(TaskID::FirmwareUpdateComplete);

      //Exit and wait for MK20 to reset ESP
      exit();
  }
}

void ESPFirmwareUpdate::loop() {
    flashFirmware();
}
