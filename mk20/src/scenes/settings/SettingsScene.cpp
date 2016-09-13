#include "SettingsScene.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "../projects/ProjectsScene.h"
#include "../filament/SelectFilamentAction.h"

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
  addView(_calibrate);

  _wifi = new BitmapButton(Rect(182,30,uiBitmaps.btn_wifi.width,uiBitmaps.btn_wifi.height));
  _wifi->setBitmap(&uiBitmaps.btn_wifi);
  addView(_wifi);

  _jobs = new BitmapButton(Rect(22,130,uiBitmaps.btn_jobs.width,uiBitmaps.btn_jobs.height));
  _jobs->setBitmap(&uiBitmaps.btn_jobs);
  addView(_jobs);

  _update = new BitmapButton(Rect(102,130,uiBitmaps.btn_update.width,uiBitmaps.btn_update.height));
  _update->setBitmap(&uiBitmaps.btn_update);
  addView(_update);

  _password = new BitmapButton(Rect(182,130,uiBitmaps.btn_password.width,uiBitmaps.btn_password.height));
  _password->setBitmap(&uiBitmaps.btn_password);
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
  SidebarSceneController::buttonPressed(button);
}
