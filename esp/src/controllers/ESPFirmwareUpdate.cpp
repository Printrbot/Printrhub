/*
 * Updates the ESP firmware over the air (using WiFi)
 *
 * Copyright (c) 2016 Printrbot Inc.
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

#include "ESPFirmwareUpdate.h"
#include "Idle.h"
#include "Application.h"
#include "../event_logger.h"
#include <ESP8266httpUpdate.h>
#include "MK20FirmwareUpdate.h"

ESPFirmwareUpdate::ESPFirmwareUpdate(String url) :
	Mode(),
	_url(url) {
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
  switch (taskID) {
	case TaskID::UpdateESPFirmware:return true;
	  break;
	default:return false;
  }
}

void ESPFirmwareUpdate::flashFirmware() {

  EventLogger::log("IN SERVER UPDATE LOOP");
  ESPhttpUpdate.rebootOnUpdate(false);
  t_httpUpdate_return ret = ESPhttpUpdate.update(_url);

  if (ret == HTTP_UPDATE_NO_UPDATES) {
	EventLogger::log("ESP Update failed, no updates");
	delay(100);
	exitWithError(DownloadError::ESPUpdateNoUpdates);
	return;
  } else if (ret == HTTP_UPDATE_FAILED) {
	EventLogger::log("ESP Update failed: %s", ESPhttpUpdate.getLastErrorString().c_str());
	delay(100);
	exitWithError(DownloadError::ESPUpdateFailed);
	return;
  } else if (ret == HTTP_UPDATE_OK) {
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
