//
// Created by Phillip Schuster on 13.10.16.
//

#include "SystemInfoScene.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "../projects/ProjectsScene.h"
#include "../filament/SelectFilamentAction.h"
#include "../calibrate/CalibrateScene.h"
#include "../settings/SettingsScene.h"

extern UIBitmaps uiBitmaps;

SystemInfoScene::SystemInfoScene(): SidebarSceneController::SidebarSceneController() {
}

SystemInfoScene::~SystemInfoScene() {
}


String SystemInfoScene::getName() {
  return "SystemInfoScene";
}

UIBitmap * SystemInfoScene::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap * SystemInfoScene::getSidebarBitmap() {
  return &uiBitmaps.sidebar_settings;
}

uint16_t SystemInfoScene::getBackgroundColor()
{
  return Application.getTheme()->getColor(BackgroundColor);
}

void SystemInfoScene::configureLabelView(LabelView* labelView) {
  labelView->setTextAlign(TEXTALIGN_LEFT);
  labelView->setFont(&PTSansNarrow_12);
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
  _printerNameLabel = new LabelView("Name:",Rect(xLabel,y,labelWidth,labelHeight));
  _printerName = new LabelView("Loading Info",Rect(xLabel+labelWidth+10,y,270-(xLabel+labelWidth+10+10),labelHeight));
  configureLabelView(_printerNameLabel);
  configureLabelView(_printerName);

  y += labelHeight + yGap;

  //Wifi-Mode
  _networkModeLabel = new LabelView("WiFi:",Rect(xLabel,y,labelWidth,labelHeight));
  _networkMode = new LabelView("Loading Info",Rect(xLabel+labelWidth+10,y,270-(xLabel+labelWidth+10+10),labelHeight));
  configureLabelView(_networkModeLabel);
  configureLabelView(_networkMode);
  y += labelHeight + yGap;

  //IPAddress
  _ipaddressLabel = new LabelView("IP address:",Rect(xLabel,y,labelWidth,labelHeight));
  _ipaddress = new LabelView("Loading Info",Rect(xLabel+labelWidth+10,y,270-(xLabel+labelWidth+10+10),labelHeight));
  configureLabelView(_ipaddressLabel);
  configureLabelView(_ipaddress);
  y += labelHeight + yGap;

  //SSID
  _ssidLabel = new LabelView("SSID:",Rect(xLabel,y,labelWidth,labelHeight));
  _ssid = new LabelView("Loading Info",Rect(xLabel+labelWidth+10,y,270-(xLabel+labelWidth+10+10),labelHeight));
  configureLabelView(_ssidLabel);
  configureLabelView(_ssid);
  y += labelHeight + yGap;

  //Serial Number
  _serialNumberLabel = new LabelView("Serial:",Rect(xLabel,y,labelWidth,labelHeight));
  _serialNumber = new LabelView("Loading Info",Rect(xLabel+labelWidth+10,y,270-(xLabel+labelWidth+10+10),labelHeight));
  configureLabelView(_serialNumberLabel);
  configureLabelView(_serialNumber);
  y += labelHeight + yGap;

  //Firmware version
  _firmwareVersionLabel = new LabelView("Firmware:",Rect(xLabel,y,labelWidth,labelHeight));
  _firmwareVersion = new LabelView("Loading Info",Rect(xLabel+labelWidth+10,y,270-(xLabel+labelWidth+10+10),labelHeight));
  configureLabelView(_firmwareVersionLabel);
  configureLabelView(_firmwareVersion);
  y += labelHeight + yGap;

  SidebarSceneController::onWillAppear();

  //Request system info
  Application.getESPStack()->requestTask(TaskID::GetSystemInfo);
}

bool SystemInfoScene::handlesTask(TaskID taskID) {
  if (taskID == TaskID::GetSystemInfo) {
    return true;
  }
  return false;
}

bool SystemInfoScene::runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool *success) {
  if (header.getCurrentTask() == TaskID::GetSystemInfo) {
    if (header.commType == ResponseSuccess) {
      //Read System info struct from data
      memcpy(&_systemInfo,data,sizeof(SystemInfo));

      _printerName->setText(_systemInfo.printerName);
      if (_systemInfo.networkMode == NetworkMode::AccessPoint) {
        _networkMode->setText("AccessPoint");
      }
      else if (_systemInfo.networkMode == NetworkMode::Client) {
        _networkMode->setText("Connected to local network");
      }
      _ipaddress->setText(_systemInfo.ipaddress);
      _ssid->setText(_systemInfo.SSID);
      _serialNumber->setText(_systemInfo.serialNumber);
      _firmwareVersion->setText(_systemInfo.firmwareVersion);
    }
  }
}

void SystemInfoScene::onSidebarButtonTouchUp() {
  SettingsScene * scene = new SettingsScene();
  Application.pushScene(scene);
}

void SystemInfoScene::buttonPressed(void *button)
{

  SidebarSceneController::buttonPressed(button);
}
