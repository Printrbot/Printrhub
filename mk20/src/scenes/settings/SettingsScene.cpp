#include "SettingsScene.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "../projects/ProjectsScene.h"
#include "../filament/SelectFilamentAction.h"
#include "../calibrate/CalibrateScene.h"
#include "../settings/SystemInfoScene.h"
#include "../settings/VirtualKeyboardSceneController.h"
#include "SetPasswordHandler.h"

extern UIBitmaps uiBitmaps;

SettingsScene::SettingsScene(): SidebarSceneController::SidebarSceneController() {
}

SettingsScene::~SettingsScene() {
}


String SettingsScene::getName() {
  return "SettingsScene";
}

UIBitmap * SettingsScene::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap * SettingsScene::getSidebarBitmap() {
  return &uiBitmaps.sidebar_settings;
}

uint16_t SettingsScene::getBackgroundColor()
{
  return Application.getTheme()->getColor(BackgroundColor);
}

void SettingsScene::onWillAppear() {

  setScrollSnap(Display.getLayoutWidth(), SnapMode::Flick);

  _filament = new BitmapButton(Rect(22,30,uiBitmaps.btn_filament.width,uiBitmaps.btn_filament.height));
  _filament->setBitmap(&uiBitmaps.btn_filament);
  _filament->setDelegate(this);
  addView(_filament);

  _calibrate = new BitmapButton(Rect(102,30,uiBitmaps.btn_calibrate.width,uiBitmaps.btn_calibrate.height));
  _calibrate->setBitmap(&uiBitmaps.btn_calibrate);
  _calibrate->setDelegate(this);
  addView(_calibrate);

  _wifi = new BitmapButton(Rect(182,30,uiBitmaps.btn_wifi.width,uiBitmaps.btn_wifi.height));
  _wifi->setBitmap(&uiBitmaps.btn_wifi);
  _wifi->setDelegate(this);
  addView(_wifi);

  _materials = new BitmapButton(Rect(22,130,uiBitmaps.btn_materials.width,uiBitmaps.btn_materials.height));
  _materials->setBitmap(&uiBitmaps.btn_materials);
  _materials->setDelegate(this);
  addView(_materials);

  _update = new BitmapButton(Rect(102,130,uiBitmaps.btn_update.width,uiBitmaps.btn_update.height));
  _update->setBitmap(&uiBitmaps.btn_update);
  _update->setDelegate(this);
  addView(_update);

  _password = new BitmapButton(Rect(182,130,uiBitmaps.btn_password.width,uiBitmaps.btn_password.height));
  _password->setBitmap(&uiBitmaps.btn_password);
  _password->setDelegate(this);
  addView(_password);

  SidebarSceneController::onWillAppear();
}


void SettingsScene::onSidebarButtonTouchUp() {
  ProjectsScene * scene = new ProjectsScene();
  Application.pushScene(scene);
}

void SettingsScene::buttonPressed(void *button)
{
  if (button == _filament) {
    SelectFilamentAction * scene = new SelectFilamentAction();
    Application.pushScene(scene);
  }
  else if (button == _calibrate) {
    CalibrateScene * scene = new CalibrateScene();
    Application.pushScene(scene);
  }
  else if (button == _materials) {

  }
  else if (button == _update) {
    Application.getESPStack()->requestTask(TaskID::StartFirmwareUpdate);
  }
  else if (button == _calibrate) {

  }
  else if (button == _password) {
    SetPasswordHandler* handler = new SetPasswordHandler();
    VirtualKeyboardSceneController* scene = new VirtualKeyboardSceneController(handler,"Password:");
    Application.pushScene(scene);
  }
  else if (button == _wifi) {
    SystemInfoScene* scene = new SystemInfoScene();
    Application.pushScene(scene);
  }
  SidebarSceneController::buttonPressed(button);
}
