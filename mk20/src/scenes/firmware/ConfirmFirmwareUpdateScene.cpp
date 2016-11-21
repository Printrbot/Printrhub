/*
 * Shows user interface that informs the user that a firmware update is available.
 * Starts over the air update procedure if user confirms it
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/behind-the-scenes-printrbot-simple-2016/
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Mick Balaban
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

#include "ConfirmFirmwareUpdateScene.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "font_LiberationSans.h"
#include "scenes/projects/ProjectsScene.h"
#include "FirmwareInProgressScene.h"

extern UIBitmaps uiBitmaps;

ConfirmFirmwareUpdateScene::ConfirmFirmwareUpdateScene() : SidebarSceneController::SidebarSceneController() {
}

ConfirmFirmwareUpdateScene::~ConfirmFirmwareUpdateScene() {
}

String ConfirmFirmwareUpdateScene::getName() {
  return "ConfirmFirmwareUpdateScene";
}

UIBitmap *ConfirmFirmwareUpdateScene::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap *ConfirmFirmwareUpdateScene::getSidebarBitmap() {
  return &uiBitmaps.sidebar_update;
}

uint16_t ConfirmFirmwareUpdateScene::getBackgroundColor() {
  return Application.getTheme()->getColor(BackgroundColor);
}

bool ConfirmFirmwareUpdateScene::isModal() {
  return true;
}

void ConfirmFirmwareUpdateScene::onWillAppear() {

  BitmapView *icon = new BitmapView(Rect(35, 20, uiBitmaps.upgrade_scene.width, uiBitmaps.upgrade_scene.height));
  icon->setBitmap(&uiBitmaps.upgrade_scene);
  addView(icon);

  _upgradeButton = new LabelButton("OK", Rect(16, 175, 235, 50));
  _upgradeButton->setBackgroundColor(RGB565(52, 52, 52));
  _upgradeButton->setTextColor(ILI9341_WHITE);
  _upgradeButton->setDelegate(this);
  addView(_upgradeButton);

  SidebarSceneController::onWillAppear();
}

void ConfirmFirmwareUpdateScene::onSidebarButtonTouchUp() {
  ProjectsScene *scene = new ProjectsScene();
  Application.pushScene(scene, true);
}

void ConfirmFirmwareUpdateScene::handleTouchMoved(TS_Point point, TS_Point oldPoint) {
  SceneController::handleTouchMoved(point, oldPoint);
}

void ConfirmFirmwareUpdateScene::animationFinished(Animation *animation) {
  SceneController::animationFinished(animation);
}

void ConfirmFirmwareUpdateScene::buttonPressed(void *button) {
  if (button == _upgradeButton) {

	//Show firmware in progress scene and initiate firmware update by requesting it to ESP
	FirmwareInProgressScene *scene = new FirmwareInProgressScene();
	Application.pushScene(scene, true);

	Application.getESPStack()->requestTask(TaskID::StartFirmwareUpdate);
  }

  SidebarSceneController::buttonPressed(button);
}
