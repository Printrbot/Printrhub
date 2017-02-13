/*
 * Shows user details about the print and a progress bar, handles main printer
 * communication
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

#include "PrintStatusScene.h"
#include "framework/views/ProgressBar.h"
#include "ConfirmCancelPrint.h"
#include "Printr.h"
#include "UIBitmaps.h"
#include "framework/layers/TransparentTextLayer.h"
#include "font_LiberationSans.h"
#include "FinishPrint.h";

#include "scenes/print/PrintStatusScene.h"

extern UIBitmaps uiBitmaps;
extern Printr printr;
extern int lastJobIndex;

PrintStatusScene::PrintStatusScene(String jobFilePath, Project project, Job job) :
	SidebarSceneController::SidebarSceneController(),
	_jobFilePath(jobFilePath),
	_project(project),
	_job(job) {
  printr.setListener(this);
}

PrintStatusScene::~PrintStatusScene() {
  printr.setListener(nullptr);
}

UIBitmap *PrintStatusScene::getSidebarBitmap() {
  return &uiBitmaps.sidebar_printing;
}

UIBitmap *PrintStatusScene::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

String PrintStatusScene::getName() {
  return "PrintStatusScene";
}

bool PrintStatusScene::isModal() {
  return true;
}

void PrintStatusScene::onWillAppear() {

  // start the print only if not running already (in case we are returning from CancelPrint scene)
  if (!printr.isPrinting()) {
	_totalJobLines = printr.startJob(_jobFilePath);
  } else {
	_totalJobLines = printr.getTotalJobLines();
  }

  // display job image
  String _projectFilePath = String("/projects/" + String(_project.index));
  _imageLayer = new SDBitmapLayer(Rect(0, 0, 270, 240));
  _imageLayer->setBitmap(_projectFilePath.c_str(), 270, 240, 129675 + (129899 * lastJobIndex) + 299);

  Display.setFixedBackgroundLayer(_imageLayer);

  // job title
  _nameLayer = new TransparentTextLayer(Rect(15, 10, Display.getLayoutWidth() - 30, 25));
  _nameLayer->setTextAlign(TEXTALIGN_LEFT);
  _nameLayer->setFont(&LiberationSans_14);
  _nameLayer->setText(String(_job.title));
  _nameLayer->setForegroundColor(ILI9341_BLACK);
  Display.addLayer(_nameLayer);

  // Resolution
  _resolution = new TransparentTextLayer(Rect(10, 120, Display.getLayoutWidth() - 30, 60));
  _resolution->setTextAlign(TEXTALIGN_LEFT);
  _resolution->setFont(&LiberationSans_12);
  _resolution->setText(String("Resolution: ") + printr.getResolution());
  _resolution->setForegroundColor(ILI9341_BLACK);
  Display.addLayer(_resolution);

  // Infill
  _infill = new TransparentTextLayer(Rect(10, 140, Display.getLayoutWidth() - 30, 60));
  _infill->setTextAlign(TEXTALIGN_LEFT);
  _infill->setFont(&LiberationSans_12);
  _infill->setText(String("Infill: ") + printr.getInfill());
  _infill->setForegroundColor(ILI9341_BLACK);
  Display.addLayer(_infill);

  // Support
  _support = new TransparentTextLayer(Rect(10, 160, Display.getLayoutWidth() - 30, 60));
  _support->setTextAlign(TEXTALIGN_LEFT);
  _support->setFont(&LiberationSans_12);
  _support->setText(String("Print Support: ") + printr.getSupport());
  _support->setForegroundColor(ILI9341_BLACK);
  Display.addLayer(_support);

  // Filament required
  _filament = new TransparentTextLayer(Rect(10, 180, Display.getLayoutWidth() - 30, 60));
  _filament->setTextAlign(TEXTALIGN_LEFT);
  _filament->setFont(&LiberationSans_12);
  _filament->setText(String("Filament required: ") + printr.getFilamentLength());
  _filament->setForegroundColor(ILI9341_BLACK);
  Display.addLayer(_filament);


  // progress bar
  _progressBar = new ProgressBar(Rect(0, 228, Display.getLayoutWidth(), 12));
  _progressBar->setValue(0.0f);
  _progressBar->setBackgroundColor(RGB565(233, 154, 36));
  addView(_progressBar);

  SidebarSceneController::onWillAppear();
}

void PrintStatusScene::onNewNozzleTemperature(float temp) {

}

void PrintStatusScene::onPrintProgress(float progress) {
  _progressBar->setValue(progress);
}

void PrintStatusScene::onPrintComplete(bool success) {
  printr.setListener(NULL);
  FinishPrint *scene = new FinishPrint(_jobFilePath, _project, _job);
  Application.pushScene(scene, true);
}

void PrintStatusScene::loop() {
  SceneController::loop();
}

void PrintStatusScene::buttonPressed(void *button) {
  printr.setListener(NULL);
  ConfirmCancelPrint *scene = new ConfirmCancelPrint(&_jobFilePath, &_project, &_job);
  Application.pushScene(scene, true);
}
