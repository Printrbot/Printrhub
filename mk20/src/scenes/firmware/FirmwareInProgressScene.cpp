/*
 * Shows user interface that informs the user to not turn off the printer while
 * firmware update is in progress
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

#include "FirmwareInProgressScene.h"
#include "FirmwareInProgressScene.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "font_LiberationSans.h"
#include "scenes/projects/ProjectsScene.h"

extern UIBitmaps uiBitmaps;

FirmwareInProgressScene::FirmwareInProgressScene() : SidebarSceneController::SidebarSceneController() {
}

FirmwareInProgressScene::~FirmwareInProgressScene() {
}

String FirmwareInProgressScene::getName() {
  return "FirmwareInProgressScene";
}

UIBitmap *FirmwareInProgressScene::getSidebarIcon() {
  return &uiBitmaps.sidebar_blank;
}

UIBitmap *FirmwareInProgressScene::getSidebarBitmap() {
  return &uiBitmaps.btn_sidebar_blank;
}

uint16_t FirmwareInProgressScene::getBackgroundColor() {
  return Application.getTheme()->getColor(BackgroundColor);
}

void FirmwareInProgressScene::onWillAppear() {

  BitmapView *icon = new BitmapView(Rect(0, 0, uiBitmaps.upgrading_scene.width, uiBitmaps.upgrading_scene.height));
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

void FirmwareInProgressScene::buttonPressed(void *button) {
  SidebarSceneController::buttonPressed(button);
}
