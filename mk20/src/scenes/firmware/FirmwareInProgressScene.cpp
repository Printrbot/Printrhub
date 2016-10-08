//
// Created by Phillip Schuster on 05.10.16.
//

#include "FirmwareInProgressScene.h"

#include "FirmwareInProgressScene.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "font_LiberationSans.h"
#include "scenes/projects/ProjectsScene.h"

extern UIBitmaps uiBitmaps;

FirmwareInProgressScene::FirmwareInProgressScene(): SidebarSceneController::SidebarSceneController() {
}

FirmwareInProgressScene::~FirmwareInProgressScene() {
}


String FirmwareInProgressScene::getName() {
  return "FirmwareInProgressScene";
}


UIBitmap * FirmwareInProgressScene::getSidebarIcon() {
  return NULL;
}

UIBitmap * FirmwareInProgressScene::getSidebarBitmap() {
  return &uiBitmaps.sidebar_update;
}


uint16_t FirmwareInProgressScene::getBackgroundColor()
{
  return Application.getTheme()->getColor(BackgroundColor);
}

void FirmwareInProgressScene::onWillAppear() {

  BitmapView* icon = new BitmapView(Rect(0,0,uiBitmaps.upgrading_scene.width, uiBitmaps.upgrading_scene.height));
  icon->setBitmap(&uiBitmaps.upgrading_scene);
  addView(icon);

  SidebarSceneController::onWillAppear();
}

bool FirmwareInProgressScene::isModal() {
  return true;
}


void FirmwareInProgressScene::onSidebarButtonTouchUp() {
}

void FirmwareInProgressScene::handleTouchMoved(TS_Point point, TS_Point oldPoint) {
  SceneController::handleTouchMoved(point, oldPoint);
}

void FirmwareInProgressScene::animationFinished(Animation *animation) {
  SceneController::animationFinished(animation);
}

void FirmwareInProgressScene::buttonPressed(void *button)
{
  SidebarSceneController::buttonPressed(button);
}
