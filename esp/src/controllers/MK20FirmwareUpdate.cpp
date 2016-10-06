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

MK20FirmwareUpdate::MK20FirmwareUpdate(String url):
Mode(),
target(MK20_SWD_CLK, MK20_SWD_IO, ARMDebug::LOG_NORMAL),
_state(StateDownloadFirmware),
_url(url)
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
  File firmware_file = SPIFFS.open("/mk20.bin", "r");
  if (!firmware_file) {
    Serial.println("Failed to open firmware file for reading");
    _state = StateError;
  }

  EventLogger::log("Reseting Teensy - the hard way");
  pinMode(MK20_RESET_PIN, OUTPUT);
  digitalWrite(MK20_RESET_PIN, LOW);
  delay(500);
  digitalWrite(MK20_RESET_PIN, HIGH);

  EventLogger::log("Resetting Teensy done");
  delay(200);

  EventLogger::log("Flashing");
  ESP.wdtDisable();

  // flash here
  ARMKinetisDebug target(MK20_SWD_CLK, MK20_SWD_IO, ARMDebug::LOG_NORMAL);
  uint32_t idcode;
  if (target.begin() && target.getIDCODE(idcode)) {
    char result[128];
    EventLogger::log("Debug Port Detected:");
  }
  delay(200);

  if (!target.detect()) {
    EventLogger::log("Detect failed:");
    _state = StateError;
  }
  delay(200);

  if (!target.reset()) {
    EventLogger::log("Reset failed:");
    EventLogger::log("Trying to unlock device:");
    digitalWrite(MK20_RESET_PIN,LOW);
    target.flashMassErase();
    digitalWrite(MK20_RESET_PIN,HIGH);
    EventLogger::log("Unlock successful. Powercycle device and try again");
    _state = StateError;
  } else {
    EventLogger::log("Device reset complete");
  }
  delay(200);

  if (!target.debugHalt()) {
    EventLogger::log("Debug Halt failed");
  } else {
    EventLogger::log("Debug halt complete");
  }
  delay(200);

  if (!target.initK20()) {
    EventLogger::log("MK20 Init faile");
  }
  else {
    EventLogger::log("MK20 initialized");
  }
  delay(200);

  ARMKinetisDebug::Flasher programmer(target);
  if (!programmer.installFirmware(&firmware_file)) {
    EventLogger::log("Failed to flash");
    _state = StateError;
  }

  firmware_file.close();
  target.dumpSector(0);
  target.dumpSector(1024);
  target.dumpSector(2048);

  EventLogger::log("Resetting Chip");
  digitalWrite(MK20_RESET_PIN,LOW);
  delay(200);
  digitalWrite(MK20_RESET_PIN,HIGH);

  // end flash
  ESP.wdtEnable(10000);
  EventLogger::log("Flashing MK20 complete");

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
