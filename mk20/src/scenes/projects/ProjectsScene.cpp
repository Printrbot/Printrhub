/*
 * Shows a user interface with all downloaded projects
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

#include "ProjectsScene.h"
#include "framework/views/BitmapButton.h"
#include "ImageView.h"
#include "SD.h"
#include "../settings/SettingsScene.h"
//#include "../print/PrintStatusSceneController.h"
//#include "../DownloadFileController.h"
#include "../SidebarSceneController.h"
#include "ConfirmDeleteProject.h"
#include "JobsScene.h"
#include "NoProjects.h"
#include "IndexDb.h"

extern UIBitmaps uiBitmaps;
extern int lastProjectIndex;
extern int lastJobIndex;
extern int totalProjects;

ProjectsScene::ProjectsScene() : SidebarSceneController::SidebarSceneController() {
}

ProjectsScene::~ProjectsScene() {
  delete projectIndexDb;
}

String ProjectsScene::getName() {
  return "ProjectsScene";
}

UIBitmap *ProjectsScene::getSidebarBitmap() {
  return &uiBitmaps.sidebar_project;
}

UIBitmap *ProjectsScene::getSidebarIcon() {
  return &uiBitmaps.btn_settings;
}

void ProjectsScene::onWillAppear() {

  setScrollSnap(Display.getLayoutWidth(), SnapMode::Flick);

  projectIndexDb = new IndexDb();
  //As the model views are distributed as opaque, seamless tiles we don't need auto layout as we don't have spaces where
  //background shines through
  Display.disableAutoLayout();

  for (uint8_t i = 0; i < projectIndexDb->getTotalProjects(); i++) {
	Project *p = projectIndexDb->getProjectAt(i);
	ImageView *imageView;
	imageView = new ImageView(Rect(270 * i, 0, 270, 240), 73);
	imageView->setImageTitle(String(p->title));
	imageView->setIndexFileName(String(projectIndexDb->projectFolderName) + p->index);
	addView(imageView);
  }

  _openBtn = new BitmapButton(Rect(10, 180, uiBitmaps.btn_open.width, uiBitmaps.btn_open.height));
  _deleteBtn = new BitmapButton(Rect(220, 190, 50, 50));

  _openBtn->setBitmap(&uiBitmaps.btn_open);
  _openBtn->setVisible(false);
  _openBtn->setDelegate(this);
  addView(_openBtn);

  _deleteBtn->setBitmap(&uiBitmaps.btn_trash);
  _deleteBtn->setVisible(false);
  _deleteBtn->setDelegate(this);
  addView(_deleteBtn);

  SidebarSceneController::onWillAppear();
}

void ProjectsScene::onDidAppear() {

  if (projectIndexDb->getTotalProjects() == 0) {
	NoProjects *scene = new NoProjects();
	Application.pushScene(scene);
	return;
  }

  // return to the last viewed project index
  if (lastProjectIndex > 0) {
	float x = Display.getLayoutWidth() * lastProjectIndex;
	setScrollOffset(-x);
  }

  updateButtons();

  SidebarSceneController::onDidAppear();
}

void ProjectsScene::onSidebarButtonTouchUp() {
  SettingsScene *scene = new SettingsScene();
  Application.pushScene(scene);
}

void ProjectsScene::handleTouchMoved(TS_Point point, TS_Point oldPoint) {
  //_openBtn->setVisible(false);
  //_deleteBtn->setVisible(false);
  SceneController::handleTouchMoved(point, oldPoint);
}

void ProjectsScene::updateButtons() {
  float x = Display.getLayoutWidth() * lastProjectIndex;

  _openBtn->setFrame(Rect(x + 10, 180, uiBitmaps.btn_open.width, uiBitmaps.btn_open.height));
  _openBtn->setVisible(true);
  _openBtn->setDelegate(this);

  _deleteBtn->setFrame(Rect(x + 220, 190, 50, 50));
  _deleteBtn->setVisible(true);
  _deleteBtn->setDelegate(this);
}

void ProjectsScene::animationFinished(Animation *animation) {
  SceneController::animationFinished(animation);

  //We should have stopped at a defined slot index, use that to position the button
  lastProjectIndex = getPageIndex();
  lastJobIndex = 0;

  updateButtons();
}

void ProjectsScene::buttonPressed(void *button) {
  if (button == _openBtn) {
	JobsScene *js = new JobsScene(*projectIndexDb->getProjectAt(getPageIndex()));
	Application.pushScene(js);
  } else if (button == _deleteBtn) {
	ConfirmDeleteProject *scene = new ConfirmDeleteProject(*projectIndexDb->getProjectAt(getPageIndex()));
	Application.pushScene(scene);
  }
  SidebarSceneController::buttonPressed(button);
}
