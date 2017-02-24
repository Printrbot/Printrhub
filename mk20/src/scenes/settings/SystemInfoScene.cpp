/*
 * Shows an info screen with various settings and info about the printer
 * like IP address, name and serial number
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/behind-the-scenes-printrbot-simple-2016/
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Phillip Schuster
 * https://github.com/Printrbot/Printrhub
 *
 * Developed in cooperation with Phillip Schuster (@appfruits) from appfruits.com
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

#include "SystemInfoScene.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "../projects/ProjectsScene.h"
#include "../filament/SelectFilamentAction.h"
#include "../calibrate/CalibrateScene.h"
#include "../settings/SettingsScene.h"
#include "font_LiberationSans.h"

extern UIBitmaps uiBitmaps;

SystemInfoScene::SystemInfoScene() : SidebarSceneController::SidebarSceneController() {
}

SystemInfoScene::~SystemInfoScene() {
}

String SystemInfoScene::getName() {
  return "SystemInfoScene";
}

UIBitmap *SystemInfoScene::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap *SystemInfoScene::getSidebarBitmap() {
  return &uiBitmaps.sidebar_settings;
}

uint16_t SystemInfoScene::getBackgroundColor() {
  return Application.getTheme()->getColor(BackgroundColor);
}

void SystemInfoScene::configureLabelView(LabelView *labelView) {
  labelView->setTextAlign(TEXTALIGN_LEFT);
  labelView->setFont(&LiberationSans_11);
  labelView->setBackgroundColor(getBackgroundColor());
  addView(labelView);
}

void SystemInfoScene::onWillAppear() {

  setScrollSnap(Display.getLayoutWidth(), SnapMode::Flick);

  int y = 10;
  int xLabel = 10;
  int labelWidth = 80;
  int labelHeight = 20;
  int yGap = 10;

  //Printer Name
  _printerNameLabel = new LabelView("Name:", Rect(xLabel, y, labelWidth, labelHeight));
  _printerName = new LabelView("Loading Info", Rect(
	  xLabel + labelWidth + 10, y, 270 - (xLabel + labelWidth + 10 + 10), labelHeight));
  configureLabelView(_printerNameLabel);
  configureLabelView(_printerName);

  y += labelHeight + yGap;

  //Wifi-Mode
  _networkModeLabel = new LabelView("WiFi:", Rect(xLabel, y, labelWidth, labelHeight));
  _networkMode = new LabelView("Loading Info", Rect(
	  xLabel + labelWidth + 10, y, 270 - (xLabel + labelWidth + 10 + 10), labelHeight));
  configureLabelView(_networkModeLabel);
  configureLabelView(_networkMode);
  y += labelHeight + yGap;

  //IPAddress
  _ipaddressLabel = new LabelView("IP address:", Rect(xLabel, y, labelWidth, labelHeight));
  _ipaddress = new LabelView("Loading Info", Rect(
	  xLabel + labelWidth + 10, y, 270 - (xLabel + labelWidth + 10 + 10), labelHeight));
  configureLabelView(_ipaddressLabel);
  configureLabelView(_ipaddress);
  y += labelHeight + yGap;

    //IPAddress
  _macAddressLabel = new LabelView("Mac:", Rect(xLabel, y, labelWidth, labelHeight));
  _macAddress = new LabelView("Loading Info", Rect(
      xLabel + labelWidth + 10, y, 270 - (xLabel + labelWidth + 10 + 10), labelHeight));
  configureLabelView(_macAddressLabel);
  configureLabelView(_macAddress);
  y += labelHeight + yGap;

  //SSID
  _ssidLabel = new LabelView("SSID:", Rect(xLabel, y, labelWidth, labelHeight));
  _ssid = new LabelView("Loading Info", Rect(
	  xLabel + labelWidth + 10, y, 270 - (xLabel + labelWidth + 10 + 10), labelHeight));
  configureLabelView(_ssidLabel);
  configureLabelView(_ssid);
  y += labelHeight + yGap;

  //Firmware version
  _firmwareVersionLabel = new LabelView("Firmware:", Rect(xLabel, y, labelWidth, labelHeight));
  _firmwareVersion = new LabelView("Loading Info", Rect(
	  xLabel + labelWidth + 10, y, 270 - (xLabel + labelWidth + 10 + 10), labelHeight));
  configureLabelView(_firmwareVersionLabel);
  configureLabelView(_firmwareVersion);
  y += labelHeight + yGap;

  //Serial Number
  _serialNumberLabel = new LabelView("Serial:", Rect(xLabel, y, labelWidth, labelHeight));
  _serialNumber = new LabelView("Loading Info", Rect(xLabel, y + labelHeight + 4, 260, labelHeight));

  configureLabelView(_serialNumberLabel);
  configureLabelView(_serialNumber);
  _serialNumber->setFont(&LiberationSans_8);
  y += labelHeight + yGap;

  SidebarSceneController::onWillAppear();

  //Prepare requesting data
  _dataReceived = false;
  _lastPing = 0;
}

void SystemInfoScene::queryData() {
  if (_dataReceived == true) {
	return;
  }

  if (_lastPing == 0) {
	_lastPing = millis();
  }

  //Query each second until we got our data
  if ((millis() - _lastPing) > 1000) {
	Application.getESPStack()->requestTask(TaskID::GetSystemInfo);
  }
}

void SystemInfoScene::loop() {
  queryData();
}

bool SystemInfoScene::handlesTask(TaskID taskID) {
  if (taskID == TaskID::GetSystemInfo) {
	return true;
  }
  return false;
}

bool SystemInfoScene::isModal() {
  return false;
}

bool SystemInfoScene::runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool *success) {
  if (header.getCurrentTask() == TaskID::GetSystemInfo) {
	if (header.commType == ResponseSuccess) {

	  //Stop querying data
	  _dataReceived = true;

	  //Read System info struct from data
	  memcpy(&_systemInfo, data, sizeof(SystemInfo));

	  // read serial number from sd card
	  String path = "/serial";

	  if (SD.exists(path.c_str())) {
		File _file = SD.open(path.c_str(), FILE_READ);
		_file.read(_systemInfo.serialNumber, 36);
		_systemInfo.serialNumber[37] = '\0';
	  } else {
		strcpy(_systemInfo.serialNumber, "Missing!");
	  }

	  _printerName->setText(_systemInfo.printerName);
	  if (_systemInfo.networkMode == NetworkMode::AccessPoint) {
		_networkMode->setText("AccessPoint");
	  } else if (_systemInfo.networkMode == NetworkMode::Client) {
		_networkMode->setText("Local network");
	  }
	  _ipaddress->setText(_systemInfo.ipaddress);
      _macAddress->setText(_systemInfo.macAddress);
	  _ssid->setText(_systemInfo.SSID);
	  _serialNumber->setText(_systemInfo.serialNumber);
	  _firmwareVersion->setText(_systemInfo.firmwareVersion);
	}
  }
}

void SystemInfoScene::onSidebarButtonTouchUp() {
  SettingsScene *scene = new SettingsScene();
  Application.pushScene(scene, true);
}

void SystemInfoScene::buttonPressed(void *button) {

  SidebarSceneController::buttonPressed(button);
}
