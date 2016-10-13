//
// Created by Phillip Schuster on 05.10.16.
//

#include "ConfirmFirmwareUpdateScene.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "font_LiberationSans.h"
#include "scenes/projects/ProjectsScene.h"
#include "FirmwareInProgressScene.h"

extern UIBitmaps uiBitmaps;

ConfirmFirmwareUpdateScene::ConfirmFirmwareUpdateScene(): SidebarSceneController::SidebarSceneController() {
}

ConfirmFirmwareUpdateScene::~ConfirmFirmwareUpdateScene() {
}


String ConfirmFirmwareUpdateScene::getName() {
  return "ConfirmFirmwareUpdateScene";
}


UIBitmap * ConfirmFirmwareUpdateScene::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap * ConfirmFirmwareUpdateScene::getSidebarBitmap() {
  return &uiBitmaps.sidebar_update;
}


uint16_t ConfirmFirmwareUpdateScene::getBackgroundColor()
{
  return Application.getTheme()->getColor(BackgroundColor);
}

bool ConfirmFirmwareUpdateScene::isModal()
{
  return true;
}

void ConfirmFirmwareUpdateScene::onWillAppear() {

  BitmapView* icon = new BitmapView(Rect(35,20,uiBitmaps.upgrade_scene.width, uiBitmaps.upgrade_scene.height));
  icon->setBitmap(&uiBitmaps.upgrade_scene);
  addView(icon);

  _upgradeButton = new LabelButton("OK",Rect(16,175,235,50));
  _upgradeButton->setBackgroundColor(RGB565(52,52,52));
  _upgradeButton->setTextColor(ILI9341_WHITE);
  _upgradeButton->setDelegate(this);
  addView(_upgradeButton);

  SidebarSceneController::onWillAppear();
}

void ConfirmFirmwareUpdateScene::onSidebarButtonTouchUp() {
  ProjectsScene* scene = new ProjectsScene();
  Application.pushScene(scene,true);
}

void ConfirmFirmwareUpdateScene::handleTouchMoved(TS_Point point, TS_Point oldPoint) {
  SceneController::handleTouchMoved(point, oldPoint);
}

void ConfirmFirmwareUpdateScene::animationFinished(Animation *animation) {
  SceneController::animationFinished(animation);
}

void ConfirmFirmwareUpdateScene::buttonPressed(void *button)
{
  if (button == _upgradeButton) {

    //Show firmware in progress scene and initiate firmware update by requesting it to ESP
    FirmwareInProgressScene* scene = new FirmwareInProgressScene();
    Application.pushScene(scene,true);

    Application.getESPStack()->requestTask(TaskID::StartFirmwareUpdate);
  }

  SidebarSceneController::buttonPressed(button);
}
