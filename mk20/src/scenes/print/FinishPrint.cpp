/*
 * Informs user that print has been finished
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

#include "FinishPrint.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "Printr.h"
#include "UIBitmaps.h"
#include "scenes/projects/ProjectsScene.h"
#include "scenes/projects/JobsScene.h"
#include "scenes/print/PrintStatusScene.h"

extern UIBitmaps uiBitmaps;
extern Printr printr;

FinishPrint::FinishPrint(String jobFilePath, Project project, Job job) :
	SidebarSceneController::SidebarSceneController(),
	_jobFilePath(jobFilePath),
	_project(project),
	_job(job) {
}

FinishPrint::~FinishPrint() {
}

String FinishPrint::getName() {
  return "FinishPrint";
}

UIBitmap *FinishPrint::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap *FinishPrint::getSidebarBitmap() {
  return &uiBitmaps.sidebar_printing;
}

void FinishPrint::animationFinished(Animation *animation) {
  SceneController::animationFinished(animation);
}

uint16_t FinishPrint::getBackgroundColor() {
  return Application.getTheme()->getColor(BackgroundColor);
}

void FinishPrint::onWillAppear() {
  BitmapView *icon = new BitmapView(Rect(26, 18, uiBitmaps.job_finish_scene.width, uiBitmaps.job_finish_scene.height));
  icon->setBitmap(&uiBitmaps.job_finish_scene);
  addView(icon);

  _btnDone = new BitmapButton(Rect(16, 176, uiBitmaps.btn_done.width, uiBitmaps.btn_done.height));
  _btnDone->setBitmap(&uiBitmaps.btn_done);
  _btnDone->setDelegate(this);
  addView(_btnDone);

  SidebarSceneController::onWillAppear();
}

void FinishPrint::onSidebarButtonTouchUp() {
  PrintStatusScene *scene = new PrintStatusScene(_jobFilePath, _project, _job);
  Application.pushScene(scene);
}

void FinishPrint::handleTouchMoved(TS_Point point, TS_Point oldPoint) {
  SceneController::handleTouchMoved(point, oldPoint);
}

void FinishPrint::buttonPressed(void *button) {
  // move print head out of the way
  printr.sendLine("G0 Y150"); // move head out of the way
  // set the light to blue
  printr.sendLine("M100({_leds:4})"); //switch to blue light

  JobsScene *js = new JobsScene(_project);
  Application.pushScene(js);

  //SidebarSceneController::buttonPressed(button);
}
