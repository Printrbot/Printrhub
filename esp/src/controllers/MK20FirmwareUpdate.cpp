/*
 * Updated the MK20 firmware update via Serial Wire Debug protocol. Uses previously
 * stored MK20 firmware binary downloaded to SPIFFS (see DownloadFileToSPIFFS)
 *
 * Copyright (c) 2016 Printrbot Inc.
 *
 * Developed in cooperation by Phillip Schuster (@appfruits) from appfruits.com
 * http://www.appfruits.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "MK20FirmwareUpdate.h"
#include "Idle.h"
#include "../hal.h"
#include "../event_logger.h"
#include <ESP8266HTTPClient.h>
#include "Application.h"

MK20FirmwareUpdate::MK20FirmwareUpdate(String localFilePath) :
	Mode(),
	target(MK20_SWD_CLK, MK20_SWD_IO, ARMDebug::LOG_NORMAL),
	_localFilePath(localFilePath) {

}

MK20FirmwareUpdate::~MK20FirmwareUpdate() {

}

bool MK20FirmwareUpdate::handlesTask(TaskID taskID) {
  switch (taskID) {
	case TaskID::UpdateMK20Firmware:break;
	default:return false;
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

  if (_mk20->updateFirmware(_localFilePath)) {
	Application.sendPulse(5, 4);
	EventLogger::log("MK20 successfully flashed");
	exit();
  } else {
	Application.sendPulse(5, 5);
	EventLogger::log("Failed to flash MK20");
	exitWithError(DownloadError::MK20UpdateError);
  }
}
