/*
 * Shows a user interface that allows the user to calibrate the printers z
 * position.
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

#include "CalibrateScene.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "font_LiberationSans.h"
#include "../settings/SettingsScene.h"
#include "Printr.h"
#include "scenes/settings/DataStore.h"

extern UIBitmaps uiBitmaps;
extern Printr printr;
extern DataStore dataStore;

CalibrateScene::CalibrateScene() :
	SidebarSceneController::SidebarSceneController() {
}

CalibrateScene::~CalibrateScene() {
}

String CalibrateScene::getName() {
  return "Calibrate";
}

UIBitmap *CalibrateScene::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap *CalibrateScene::getSidebarBitmap() {
  return &uiBitmaps.sidebar_calibrate;
}

uint16_t CalibrateScene::getBackgroundColor() {
  return Application.getTheme()->getColor(BackgroundColor);
}

void CalibrateScene::onWillAppear() {

  /* BitmapView* hb = new BitmapView(Rect(16,10,uiBitmaps.hotend.width, uiBitmaps.hotend.height));
   hb->setBitmap(&uiBitmaps.hotend);
   addView(hb);*/

  SDBitmapLayer *hotendLayer = new SDBitmapLayer(Rect(16, 10, 121, uiBitmaps.hotend.height));
  hotendLayer->setBitmap("ui.min", 121, uiBitmaps.hotend.height, uiBitmaps.hotend.offset);
  Display.addLayer(hotendLayer);

  SDBitmapLayer
	  *offsetLayer = new SDBitmapLayer(Rect(137, 75, uiBitmaps.hotend_offset.width, uiBitmaps.hotend_offset.height));
  offsetLayer->setBitmap("ui.min", uiBitmaps.hotend_offset.width, uiBitmaps.hotend_offset.height, uiBitmaps.hotend_offset.offset);
  Display.addLayer(offsetLayer);

  _offset = dataStore.getHeadOffset();
  _offsetText = new TextLayer(Rect(137, 13, 113, 62));
  _offsetText->setFont(&LiberationSans_32);
  _offsetText->setTextAlign(TEXTALIGN_CENTERED);
  _offsetText->setForegroundColor(ILI9341_BLACK);
  _offsetText->setBackgroundColor(RGB565(200, 200, 200));

  char buffer[10];
  dtostrf(_offset, 3, 1, buffer);
  _offsetText->setText(String(buffer));

  Display.addLayer(_offsetText);

  _minusBtn = new BitmapButton(Rect(15, 112, uiBitmaps.btn_down.width, uiBitmaps.btn_down.height));
  _minusBtn->setBitmap(&uiBitmaps.btn_down);
  _minusBtn->setDelegate(this);
  addView(_minusBtn);

  _plusBtn = new BitmapButton(Rect(138, 112, uiBitmaps.btn_up.width, uiBitmaps.btn_up.height));
  _plusBtn->setBitmap(&uiBitmaps.btn_up);
  _plusBtn->setDelegate(this);
  addView(_plusBtn);

  _saveBtn = new BitmapButton(Rect(15, 173, uiBitmaps.btn_save.width, uiBitmaps.btn_save.height));
  _saveBtn->setBitmap(&uiBitmaps.btn_save);
  _saveBtn->setDelegate(this);
  addView(_saveBtn);

  SidebarSceneController::onWillAppear();
}

void CalibrateScene::onSidebarButtonTouchUp() {
  SettingsScene *scene = new SettingsScene();
  Application.pushScene(scene);
}

void CalibrateScene::buttonPressed(void *button) {
  if (button == _plusBtn) {
	_offset += 0.1f;
  }
  if (button == _minusBtn) {
	_offset -= 0.1f;
  }

  if (button == _saveBtn) {
	dataStore.setHeadOffset(_offset);
	dataStore.save();

	SettingsScene *scene = new SettingsScene();
	Application.pushScene(scene);

	return;
  }

  //Convert float to string and set to text field
  char buffer[4];
  dtostrf(_offset, 3, 1, buffer);
  _offsetText->setText(String(buffer));

  SidebarSceneController::buttonPressed(button);
}
