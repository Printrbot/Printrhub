#include "SettingsScene.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "../projects/ProjectsScene.h"

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

  _filament = new BitmapButton(Rect(22,26,uiBitmaps.btn_filament.width,uiBitmaps.btn_filament.height));
  _filament->setBitmap(&uiBitmaps.btn_filament);
  addView(_filament);

  _calibrate = new BitmapButton(Rect(102,26,uiBitmaps.btn_calibrate.width,uiBitmaps.btn_calibrate.height));
  _calibrate->setBitmap(&uiBitmaps.btn_calibrate);
  addView(_calibrate);

  _wifi = new BitmapButton(Rect(182,26,uiBitmaps.btn_wifi.width,uiBitmaps.btn_wifi.height));
  _wifi->setBitmap(&uiBitmaps.btn_wifi);
  addView(_wifi);

  _cloud = new BitmapButton(Rect(22,126,uiBitmaps.btn_cloud.width,uiBitmaps.btn_cloud.height));
  _cloud->setBitmap(&uiBitmaps.btn_cloud);
  addView(_cloud);

  _update = new BitmapButton(Rect(102,126,uiBitmaps.btn_update.width,uiBitmaps.btn_update.height));
  _update->setBitmap(&uiBitmaps.btn_update);
  addView(_update);

  _password = new BitmapButton(Rect(182,126,uiBitmaps.btn_password.width,uiBitmaps.btn_password.height));
  _password->setBitmap(&uiBitmaps.btn_password);
  addView(_password);

  SidebarSceneController::onWillAppear();
}


void SettingsScene::onSidebarButtonTouchUp() {
  ProjectsScene * scene = new ProjectsScene();
  Application.pushScene(scene);
}

void SettingsScene::handleTouchMoved(TS_Point point, TS_Point oldPoint) {
  //if (_openBtn != NULL) {
  //  _openBtn->setVisible(false);
  //}
  SceneController::handleTouchMoved(point, oldPoint);
}

void SettingsScene::animationFinished(Animation *animation) {
  SceneController::animationFinished(animation);

}

void SettingsScene::buttonPressed(void *button)
{
  SidebarSceneController::buttonPressed(button);
}
