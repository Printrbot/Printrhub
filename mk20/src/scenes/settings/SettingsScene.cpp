/*
 * Shows a settings screen with various options
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

#include "SettingsScene.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "../projects/ProjectsScene.h"
#include "../filament/SelectFilamentAction.h"
#include "../calibrate/CalibrateScene.h"
#include "../settings/SystemInfoScene.h"
#include "../materials/MaterialsScene.h"
#include "../settings/VirtualKeyboardSceneController.h"
#include "SetPasswordHandler.h"

extern UIBitmaps uiBitmaps;

SettingsScene::SettingsScene() : SidebarSceneController::SidebarSceneController() {
}

SettingsScene::~SettingsScene() {
}

String SettingsScene::getName() {
  return "SettingsScene";
}

UIBitmap *SettingsScene::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap *SettingsScene::getSidebarBitmap() {
  return &uiBitmaps.sidebar_settings;
}

uint16_t SettingsScene::getBackgroundColor() {
  return Application.getTheme()->getColor(BackgroundColor);
}

void SettingsScene::onWillAppear() {

  setScrollSnap(Display.getLayoutWidth(), SnapMode::Flick);

  _filament = new BitmapButton(Rect(22, 30, uiBitmaps.btn_filament.width, uiBitmaps.btn_filament.height));
  _filament->setBitmap(&uiBitmaps.btn_filament);
  _filament->setDelegate(this);
  addView(_filament);

  _calibrate = new BitmapButton(Rect(102, 30, uiBitmaps.btn_calibrate.width, uiBitmaps.btn_calibrate.height));
  _calibrate->setBitmap(&uiBitmaps.btn_calibrate);
  _calibrate->setDelegate(this);
  addView(_calibrate);

  _wifi = new BitmapButton(Rect(182, 30, uiBitmaps.btn_wifi.width, uiBitmaps.btn_wifi.height));
  _wifi->setBitmap(&uiBitmaps.btn_wifi);
  _wifi->setDelegate(this);
  addView(_wifi);

  _materials = new BitmapButton(Rect(22, 130, uiBitmaps.btn_materials.width, uiBitmaps.btn_materials.height));
  _materials->setBitmap(&uiBitmaps.btn_materials);
  _materials->setDelegate(this);
  addView(_materials);

  _update = new BitmapButton(Rect(102, 130, uiBitmaps.btn_update.width, uiBitmaps.btn_update.height));
  _update->setBitmap(&uiBitmaps.btn_update);
  _update->setDelegate(this);
  addView(_update);

  _password = new BitmapButton(Rect(182, 130, uiBitmaps.btn_password.width, uiBitmaps.btn_password.height));
  _password->setBitmap(&uiBitmaps.btn_password);
  _password->setDelegate(this);
  addView(_password);

  SidebarSceneController::onWillAppear();
}

void SettingsScene::onSidebarButtonTouchUp() {
  ProjectsScene *scene = new ProjectsScene();
  Application.pushScene(scene);
}

void SettingsScene::buttonPressed(void *button) {
  if (button == _filament) {
	SelectFilamentAction *scene = new SelectFilamentAction();
	Application.pushScene(scene);
  } else if (button == _calibrate) {
	CalibrateScene *scene = new CalibrateScene();
	Application.pushScene(scene);
  } else if (button == _materials) {
	MaterialsScene *scene = new MaterialsScene();
	Application.pushScene(scene);
  } else if (button == _update) {
	Application.getESPStack()->requestTask(TaskID::StartFirmwareUpdate);
  } else if (button == _calibrate) {

  } else if (button == _password) {
	SetPasswordHandler *handler = new SetPasswordHandler();
	VirtualKeyboardSceneController *scene = new VirtualKeyboardSceneController(handler, "Password:");
	Application.pushScene(scene);
  } else if (button == _wifi) {
	SystemInfoScene *scene = new SystemInfoScene();
	Application.pushScene(scene);
  }
  SidebarSceneController::buttonPressed(button);
}
