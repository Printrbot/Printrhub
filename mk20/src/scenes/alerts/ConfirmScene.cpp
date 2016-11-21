/*
 * Shows a yes and no button
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


#include "ConfirmScene.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "font_LiberationSans.h"

extern UIBitmaps uiBitmaps;

ConfirmScene::ConfirmScene() : SidebarSceneController::SidebarSceneController() {
}

ConfirmScene::~ConfirmScene() {
}

String ConfirmScene::getName() {
  return "ConfirmScene";
}

UIBitmap *ConfirmScene::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap *ConfirmScene::getSidebarBitmap() {
  return NULL;
}

uint16_t ConfirmScene::getBackgroundColor() {
  return Application.getTheme()->getColor(BackgroundColor);
}

void ConfirmScene::onWillAppear() {

  BitmapView *icon = new BitmapView(Rect(100, 24, uiBitmaps.icon_alert.width, uiBitmaps.icon_alert.height));
  icon->setBitmap(&uiBitmaps.icon_alert);
  addView(icon);

  TextLayer *textLayer = new TextLayer(Rect(34, 118, 197, 20));
  textLayer->setFont(&LiberationSans_14);
  textLayer->setTextAlign(TEXTALIGN_CENTERED);
  textLayer->setForegroundColor(ILI9341_WHITE);
  textLayer->setText("Are you sure");
  Display.addLayer(textLayer);

  _yesBtn = new BitmapButton(Rect(50, 178, uiBitmaps.btn_yes.width, uiBitmaps.btn_yes.height));
  _yesBtn->setBitmap(&uiBitmaps.btn_yes);
  addView(_yesBtn);

  _noBtn = new BitmapButton(Rect(137, 178, uiBitmaps.btn_no.width, uiBitmaps.btn_no.height));
  _noBtn->setBitmap(&uiBitmaps.btn_no);
  addView(_noBtn);

  SidebarSceneController::onWillAppear();
}

void ConfirmScene::onSidebarButtonTouchUp() {
  // todo
}

void ConfirmScene::handleTouchMoved(TS_Point point, TS_Point oldPoint) {
  SceneController::handleTouchMoved(point, oldPoint);
}

void ConfirmScene::animationFinished(Animation *animation) {
  SceneController::animationFinished(animation);
}

void ConfirmScene::buttonPressed(void *button) {
  SidebarSceneController::buttonPressed(button);
}
