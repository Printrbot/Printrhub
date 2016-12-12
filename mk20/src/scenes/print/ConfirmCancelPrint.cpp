/*
 * Asks the user to confirm cancellation of a print
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

#include "ConfirmCancelPrint.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "Printr.h"
#include "UIBitmaps.h"
#include "scenes/projects/ProjectsScene.h"
#include "scenes/projects/JobsScene.h"
#include "scenes/print/PrintStatusScene.h"

extern UIBitmaps uiBitmaps;
extern Printr printr;

ConfirmCancelPrint::ConfirmCancelPrint(String *jobFilePath, Project *project, Job *job) :
	SidebarSceneController::SidebarSceneController(),
	_jobFilePath(*jobFilePath),
	_project(*project),
	_job(*job) {
}

ConfirmCancelPrint::~ConfirmCancelPrint() {
}

String ConfirmCancelPrint::getName() {
  return "PausePrintScene";
}

UIBitmap *ConfirmCancelPrint::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap *ConfirmCancelPrint::getSidebarBitmap() {
  return &uiBitmaps.sidebar_printing;
}

bool ConfirmCancelPrint::isModal() {
  return true;
}

uint16_t ConfirmCancelPrint::getBackgroundColor() {
  return Application.getTheme()->getColor(BackgroundColor);
}

void ConfirmCancelPrint::onWillAppear() {

  BitmapView* icon = new BitmapView(Rect(95,14,uiBitmaps.icon_alert.width, uiBitmaps.icon_alert.height));
  icon->setBitmap(&uiBitmaps.icon_alert);
  addView(icon);

  _btnCancelPrint =
	  new BitmapButton(Rect(17, 112, uiBitmaps.btn_cancel_print.width, uiBitmaps.btn_cancel_print.height));
  _btnCancelPrint->setBitmap(&uiBitmaps.btn_cancel_print);
  _btnCancelPrint->setDelegate(this);
  addView(_btnCancelPrint);

  _btnBack = new BitmapButton(Rect(17, 172, uiBitmaps.btn_back.width, uiBitmaps.btn_back.height));
  _btnBack->setDelegate(this);
  _btnBack->setBitmap(&uiBitmaps.btn_back);
  addView(_btnBack);

  SidebarSceneController::onWillAppear();

}

void ConfirmCancelPrint::onSidebarButtonTouchUp() {
  PrintStatusScene *scene = new PrintStatusScene(_jobFilePath, _project, _job);
  Application.pushScene(scene, true);
}

void ConfirmCancelPrint::onDidAppear() {
  SidebarSceneController::onDidAppear();
}

void ConfirmCancelPrint::buttonPressed(void *button) {
  if (button == _btnBack) {
	PrintStatusScene *scene = new PrintStatusScene(_jobFilePath, _project, _job);
	Application.pushScene(scene, true);
  } else if (button == _btnCancelPrint) {
	printr.cancelCurrentJob();
	JobsScene *js = new JobsScene(_project);
	Application.pushScene(js, true);
  }

  SidebarSceneController::buttonPressed(button);
}
