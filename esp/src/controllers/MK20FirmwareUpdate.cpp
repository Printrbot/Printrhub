#include "MK20FirmwareUpdate.h"
#include "Idle.h"
#include "../hal.h"
#include "../event_logger.h"
#include <ESP8266HTTPClient.h>
#include "Application.h"

// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;

MK20FirmwareUpdate::MK20FirmwareUpdate(String localFilePath):
Mode(),
target(MK20_SWD_CLK, MK20_SWD_IO, ARMDebug::LOG_NORMAL),
_localFilePath(localFilePath)
{

}

MK20FirmwareUpdate::~MK20FirmwareUpdate() {

}

bool MK20FirmwareUpdate::handlesTask(TaskID taskID) {
  switch(taskID) {
    case TaskID::UpdateMK20Firmware:
      break;
    default:
      return false;
  }
}


String MK20FirmwareUpdate::getName() {
  return "MK20FirmwareUpdate";
}

void MK20FirmwareUpdate::onWillStart() {
}

void MK20FirmwareUpdate::onWillEnd() {
}

void MK20FirmwareUpdate::loop() {

    if (_mk20->updateFirmware(_localFilePath))
    {
        Application.sendPulse(5,4);
        EventLogger::log("MK20 successfully flashed");
        exit();
    } else {
        Application.sendPulse(5,5);
        EventLogger::log("Failed to flash MK20");
        exitWithError(DownloadError::MK20UpdateError);
    }
}
