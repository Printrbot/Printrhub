/*
 * Shows an error dialog to give the user some feedback about errors
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/behind-the-scenes-printrbot-simple-2016/
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Phillip Schuster
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

#include "ErrorScene.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "font_LiberationSans.h"
#include "../projects/ProjectsScene.h"

extern UIBitmaps uiBitmaps;

ErrorScene::ErrorScene(const String &errorMessage, bool showButton) : SidebarSceneController::SidebarSceneController() {
  _errorMessage = errorMessage;
  _showButton = showButton;
}

ErrorScene::~ErrorScene() {
}

String ErrorScene::getName() {
  return "ErrorScene";
}

UIBitmap *ErrorScene::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap *ErrorScene::getSidebarBitmap() {
  return NULL;
}

uint16_t ErrorScene::getBackgroundColor() {
  return Application.getTheme()->getColor(BackgroundColor);
}

void ErrorScene::onWillAppear() {

  BitmapView *icon = new BitmapView(Rect(100, 24, uiBitmaps.icon_alert.width, uiBitmaps.icon_alert.height));
  icon->setBitmap(&uiBitmaps.icon_alert);
  addView(icon);

  LabelView *labelView = new LabelView(_errorMessage, Rect(34, 118, 197, 20));
  labelView->setFont(&LiberationSans_14);
  labelView->setTextAlign(TEXTALIGN_CENTERED);
  addView(labelView);

  if (_showButton) {
	_okBtn = new LabelButton("OK", Rect(50, 178, 170, 38));
	_okBtn->setBackgroundColor(ILI9341_DARKGREEN);
	_okBtn->setTextColor(ILI9341_WHITE);
	_okBtn->setDelegate(this);
	addView(_okBtn);
  }

  SidebarSceneController::onWillAppear();
}

void ErrorScene::onSidebarButtonTouchUp() {
  ProjectsScene *scene = new ProjectsScene();
  Application.pushScene(scene);
}

void ErrorScene::buttonPressed(void *button) {
  if (button == _okBtn) {
	ProjectsScene *scene = new ProjectsScene();
	Application.pushScene(scene);
  }
}
