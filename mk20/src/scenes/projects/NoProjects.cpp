/*
 * Shows a user interface informing the user that no projects have been downloaded
 * to the printer yet
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

#include "NoProjects.h"
#include "../SidebarSceneController.h"
#include "font_LiberationSans.h"
#include "../settings/SettingsScene.h"

extern UIBitmaps uiBitmaps;

NoProjects::NoProjects() : SidebarSceneController::SidebarSceneController() {
}

NoProjects::~NoProjects() {
}

String NoProjects::getName() {
  return "NoProjects";
}

UIBitmap *NoProjects::getSidebarIcon() {
  return &uiBitmaps.btn_settings;
}

UIBitmap *NoProjects::getSidebarBitmap() {
  return &uiBitmaps.sidebar_project;
}

uint16_t NoProjects::getBackgroundColor() {
  return Application.getTheme()->getColor(BackgroundColor);
}

void NoProjects::onWillAppear() {

  BitmapView
	  *icon = new BitmapView(Rect(0, 0, uiBitmaps.scene_empty_project.width, uiBitmaps.scene_empty_project.height));
  icon->setBitmap(&uiBitmaps.scene_empty_project);
  addView(icon);
/*
  TextLayer* textLayer = new TextLayer(Rect(34,158, 197, 20));
  textLayer->setFont(&LiberationSans_14);
  textLayer->setTextAlign(TEXTALIGN_CENTERED);
  textLayer->setForegroundColor(ILI9341_WHITE);
  textLayer->setText("No projects available :(");
  Display.addLayer(textLayer);
*/
  SidebarSceneController::onWillAppear();
}

void NoProjects::onSidebarButtonTouchUp() {
  SettingsScene *scene = new SettingsScene();
  Application.pushScene(scene);
}

void NoProjects::buttonPressed(void *button) {
  SidebarSceneController::buttonPressed(button);
}
