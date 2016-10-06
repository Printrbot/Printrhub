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

MK20FirmwareUpdate::MK20FirmwareUpdate():
Mode(),
target(MK20_SWD_CLK, MK20_SWD_IO, ARMDebug::LOG_NORMAL),
_state(StateFlashFirmware)
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

void MK20FirmwareUpdate::flashFirmware() {

  _mk20->updateFirmware();

  _state = StateSuccess;
}

void MK20FirmwareUpdate::downloadFirmware() {
  EventLogger::log("IN MK20 FIRMWARE UPDATE");
	EventLogger::log("DOWNLOADING FIRMWARE");
	HTTPClient http;
  //http.begin(server.arg("location"));
	http.begin(_url);
  int httpCode = http.GET();
  //DEBUG(server.arg("location"));
  if (httpCode > 0) {
    if (httpCode == 200) {
			//request->send(200, "text/html", "updating mk20 firmware, please wait");
			EventLogger::log("GOT THE REPLY FROM SERVER...");
      uint8_t buff[128] = { 0 };
      WiFiClient * stream = http.getStreamPtr();
			int len = http.getSize();
			EventLogger::log("GOT THE FILE INFO, downloading now");
			File firmware_file = SPIFFS.open("/mk20.bin", "w");

			if (!firmware_file) {
				EventLogger::log("ERROR WRITING FIRMWARE FILE");
        _state = StateError;
				return;
			}

			while(http.connected() && (len > 0 || len == -1)) {
        // get available data size
        size_t size = stream->available();
        if(size) {
          // read up to 128 byte
          int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));
          firmware_file.write(buff, c);
          if(len > 0) {
            len -= c;
          }
        }
        delay(1);
      }
      EventLogger::log("WRITING DONE");
			firmware_file.close();
      _state = StateFlashFirmware;
      return;
    } else {
      EventLogger::log("error downloading gcode file");
      _state = StateError;
    }
  } else {
		EventLogger::log("GOT BAD HEADER FROM SERVER");
		//request->send(500, "text/html", "unable to download file");
	}
}

void MK20FirmwareUpdate::loop() {

  if (_state == StateDownloadFirmware) {
    downloadFirmware();
    return;
  }

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

    Mode* mode = new Idle();
    Application.pushMode(mode);

    return;
  }
}
