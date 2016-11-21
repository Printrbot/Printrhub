/*
 * Shows a user interface to the user that allows him zu select the material
 * loaded in the printer. Adjusts speed and temperature settings.
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

#include "SelectFilamentAction.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "font_LiberationSans.h"
#include "../settings/SettingsScene.h"
#include "PreheatExtruder.h"
#include "UnloadFilament.h"
#include "framework/layers/TransparentTextLayer.h"

extern UIBitmaps uiBitmaps;

SelectFilamentAction::SelectFilamentAction() :
	SidebarSceneController::SidebarSceneController() {
}

SelectFilamentAction::~SelectFilamentAction() {
}

String SelectFilamentAction::getName() {
  return "SelectFilamentAction";
}

UIBitmap *SelectFilamentAction::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap *SelectFilamentAction::getSidebarBitmap() {
  return &uiBitmaps.sidebar_filament;
}

uint16_t SelectFilamentAction::getBackgroundColor() {
  return Application.getTheme()->getColor(BackgroundColor);
}

void SelectFilamentAction::onWillAppear() {

  BitmapView *icon = new BitmapView(Rect(85, 10, uiBitmaps.icon_filament.width, uiBitmaps.icon_filament.height));
  icon->setBitmap(&uiBitmaps.icon_filament);
  addView(icon);

/*
  TextLayer* textLayer = new TextLayer(Rect(10,136, 250, 20));
  textLayer->setFont(&LiberationSans_14);
  textLayer->setTextAlign(TEXTALIGN_CENTERED);
  textLayer->setForegroundColor(ILI9341_WHITE);
  textLayer->setBackgroundColor(getBackgroundColor());
  textLayer->setText("Please select below");
  Display.addLayer(textLayer);
*/



  _loadBtn = new BitmapButton(Rect(15, 112, uiBitmaps.btn_load.width, uiBitmaps.btn_load.height));
  _loadBtn->setBitmap(&uiBitmaps.btn_load);
  _loadBtn->setDelegate(this);
  addView(_loadBtn);

  _unloadBtn = new BitmapButton(Rect(15, 172, uiBitmaps.btn_unload.width, uiBitmaps.btn_unload.height));
  _unloadBtn->setBitmap(&uiBitmaps.btn_unload);
  _unloadBtn->setDelegate(this);
  addView(_unloadBtn);

  SidebarSceneController::onWillAppear();
}

void SelectFilamentAction::onSidebarButtonTouchUp() {
  SettingsScene *scene = new SettingsScene();
  Application.pushScene(scene);
}

void SelectFilamentAction::buttonPressed(void *button) {
  if (button == _loadBtn) {
	PreheatExtruder *scene = new PreheatExtruder();
	scene->setNextScene(2);
	Application.pushScene(scene);
  } else if (button == _unloadBtn) {
	PreheatExtruder *scene = new PreheatExtruder();
	scene->setNextScene(1);
	Application.pushScene(scene);
  }

  SidebarSceneController::buttonPressed(button);
}
