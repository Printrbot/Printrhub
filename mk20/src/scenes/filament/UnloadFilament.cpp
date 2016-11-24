/*
 * Shows user interface with description on how to unload filament and rotates
 * drive gear to unload the filament
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

#include "UnloadFilament.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "font_LiberationSans.h"
#include "../settings/SettingsScene.h"
#include "PreheatExtruder.h"
#include "Printr.h"
#include "framework/layers/TransparentTextLayer.h"

extern UIBitmaps uiBitmaps;
extern Printr printr;

UnloadFilament::UnloadFilament() :
	SidebarSceneController::SidebarSceneController() {
}

UnloadFilament::~UnloadFilament() {
}

String UnloadFilament::getName() {
  return "UnloadFilament";
}

UIBitmap *UnloadFilament::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap *UnloadFilament::getSidebarBitmap() {
  return &uiBitmaps.sidebar_filament;
}

uint16_t UnloadFilament::getBackgroundColor() {
  return Application.getTheme()->getColor(BackgroundColor);
}

void UnloadFilament::onWillAppear() {

  BitmapView *icon = new BitmapView(Rect(0, 0, uiBitmaps.unload_scene.width, uiBitmaps.unload_scene.height));
  icon->setBitmap(&uiBitmaps.unload_scene);
  addView(icon);

  _doneBtn = new BitmapButton(Rect(15, 178, uiBitmaps.btn_done.width, uiBitmaps.btn_done.height));
  _doneBtn->setBitmap(&uiBitmaps.btn_done);
  _doneBtn->setDelegate(this);
  addView(_doneBtn);
  // increase max feed on A to be able to retract
  // filament quickly
  printr.sendLine("M100({afr:6480})");
  printr.sendLine("G92 A0");
  printr.sendLine("G1 A5 F6000");
  printr.sendLine("G1 A-200 F9000");

  SidebarSceneController::onWillAppear();
}

void UnloadFilament::onSidebarButtonTouchUp() {

}

void UnloadFilament::buttonPressed(void *button) {
  printr.stopAndFlush();
  printr.sendWaitCommand(1000);
  printr.sendLine("M100({_leds:4})");
  printr.sendLine("G0 Y150");
  printr.sendLine("G0 Z30");
  printr.turnOffHotend();

  SettingsScene *scene = new SettingsScene();
  Application.pushScene(scene);
}
