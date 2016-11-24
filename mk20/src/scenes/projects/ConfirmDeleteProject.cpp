/*
 * Asks user to comfirm project deletion. Just deletes project from local printer
 * storage but not from the cloud
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

#include "ConfirmDeleteProject.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "font_LiberationSans.h"
#include "ProjectsScene.h"

extern UIBitmaps uiBitmaps;
extern int lastProjectIndex;
extern int lastJobIndex;

ConfirmDeleteProject::ConfirmDeleteProject(Project project) :
	SidebarSceneController::SidebarSceneController(),
	_project(project) {
}

ConfirmDeleteProject::~ConfirmDeleteProject() {
}

String ConfirmDeleteProject::getName() {
  return "ConfirmDeleteProject";
}

UIBitmap *ConfirmDeleteProject::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap *ConfirmDeleteProject::getSidebarBitmap() {
  return &uiBitmaps.sidebar_project;
}

uint16_t ConfirmDeleteProject::getBackgroundColor() {
  return Application.getTheme()->getColor(BackgroundColor);
}

void ConfirmDeleteProject::onWillAppear() {
  setScrollSnap(Display.getLayoutWidth(), SnapMode::Flick);

  BitmapView *icon = new BitmapView(Rect(90, 20, uiBitmaps.icon_alert.width, uiBitmaps.icon_alert.height));
  icon->setBitmap(&uiBitmaps.icon_alert);
  addView(icon);

  _yesBtn = new BitmapButton(Rect(17, 112, uiBitmaps.btn_delete_project.width, uiBitmaps.btn_delete_project.height));
  _yesBtn->setBitmap(&uiBitmaps.btn_delete_project);
  _yesBtn->setDelegate(this);
  addView(_yesBtn);

  _noBtn = new BitmapButton(Rect(17, 172, uiBitmaps.btn_cancel.width, uiBitmaps.btn_cancel.height));
  _noBtn->setDelegate(this);
  _noBtn->setBitmap(&uiBitmaps.btn_cancel);
  addView(_noBtn);

  SidebarSceneController::onWillAppear();
}

void ConfirmDeleteProject::onDidAppear() {
  SidebarSceneController::onDidAppear();
}

void ConfirmDeleteProject::onSidebarButtonTouchUp() {
  ProjectsScene *scene = new ProjectsScene();
  Application.pushScene(scene);
}

void ConfirmDeleteProject::buttonPressed(void *button) {
  if (button == _noBtn) {
	ProjectsScene *scene = new ProjectsScene();
	Application.pushScene(scene);
  } else if (button == _yesBtn) {
	IndexDb *idb = new IndexDb();
	idb->deleteProject(_project);
	lastJobIndex = 0;
	lastProjectIndex = lastProjectIndex > 0 ? lastProjectIndex - 1 : 0;

	ProjectsScene *scene = new ProjectsScene();
	Application.pushScene(scene);
  }

  SidebarSceneController::buttonPressed(button);
}
