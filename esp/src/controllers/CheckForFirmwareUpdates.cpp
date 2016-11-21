/*
 * Downloads a JSON file from the printrbot cloud and stores firmware update
 * info in the application. If a new firmware is available the user is prompted
 * to install it.
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/behind-the-scenes-printrbot-simple-2016/
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Phillip Schuster
 * https://github.com/Printrbot/Printrhub
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

#include "CheckForFirmwareUpdates.h"
#include "Idle.h"

CheckForFirmwareUpdates::CheckForFirmwareUpdates() :
	DownloadURL(FIRMWAREUPDATE_URL),
	_content(NULL),
	_pointer(0),
	_size(0) {}

CheckForFirmwareUpdates::~CheckForFirmwareUpdates() {
  if (_content != NULL) {
	free(_content);
	_content = NULL;
  }
}

String CheckForFirmwareUpdates::getName() {
  return "CheckForFirmwareUpdates";
}

bool CheckForFirmwareUpdates::onBeginDownload(uint32_t expectedSize) {
  digitalWrite(COMMSTACK_INFO_MARKER_PIN, LOW);
  _size = expectedSize;
  _content = (uint8_t *) calloc(sizeof(uint8_t), expectedSize + 1);
  if (!_content) {
	return false;
  }

  _pointer = 0;

  return true;
}

bool CheckForFirmwareUpdates::onDataReceived(uint8_t *data, uint16_t size) {
  memcpy(&_content[_pointer], data, size);
  _pointer += size;

  return true;
}

void CheckForFirmwareUpdates::onError(DownloadError errorCode) {
  //TODO: Decide if and what to show the user in this case, today we just do nothing at all
  Idle *idle = new Idle();
  Application.pushMode(idle);
}

void CheckForFirmwareUpdates::onFinished() {
  digitalWrite(COMMSTACK_INFO_MARKER_PIN, HIGH);
  if (_content != NULL) {
	StaticJsonBuffer<1000> jsonBuffer;
	String jsonObject((const char *) _content);
	JsonObject &root = jsonBuffer.parseObject(jsonObject);

	if (root.success()) {
	  //We have a newer firmware available, save update infos like urls in Application
	  FirmwareUpdateInfo *info = new FirmwareUpdateInfo();
	  int buildNumber = root["version"];
	  const char *esp_url = root["esp_url"];
	  const char *mk20_url = root["mk20_url"];
	  const char *mk20_ui_url = root["mk20_ui_url"];

	  EventLogger::log("Firmware.json downloaded, setting firmware data");

	  info->buildnr = buildNumber;
	  info->esp_url = String(esp_url);
	  info->mk20_url = String(mk20_url);
	  info->mk20_ui_url = String(mk20_ui_url);
	  Application.setFirmwareUpdateInfo(info);

	  EventLogger::log("Firmware Infos registered");
	}
  }

  //Whatever is, we get in idle state as now either MK20 runs the project scene or is showing an update notification to the user
  Idle *idle = new Idle();
  Application.pushMode(idle);
}

void CheckForFirmwareUpdates::onCancelled() {
  //TODO: Decide if and what to show the user in this case, today we just do nothing at all
  Idle *idle = new Idle();
  Application.pushMode(idle);
}