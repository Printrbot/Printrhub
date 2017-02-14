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

#ifndef PRINT_STATUS_SCENE
#define PRINT_STATUS_SCENE

#include "../SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/ProgressBar.h"
#include "framework/views/LabelView.h"
#include "framework/views/LabelButton.h"
#include "scenes/projects/ProjectsScene.h"
#include "scenes/projects/JobsScene.h"
#include "../../Printr.h"

class PrintStatusScene : public SidebarSceneController, public ButtonDelegate, public PrintrListener {

 public:
  virtual void loop() override;
  PrintStatusScene(String jobFilePath, Project project, Job job);
  virtual ~PrintStatusScene();

  virtual void onNewNozzleTemperature(float temp);
  virtual void onPrintProgress(float progress);
  virtual void onPrintComplete(bool success);

 private:
  virtual UIBitmap *getSidebarBitmap() override;
  virtual UIBitmap *getSidebarIcon() override;
  virtual bool isModal() override;
  virtual void onWillAppear() override;
  virtual void buttonPressed(void *button) override;
  String getName();
  LabelButton *_button;
  ProgressBar *_progressBar;
  SDBitmapLayer *_imageLayer;
  float _step;
  Project _project;
  Job _job;

  String _jobFilePath;
  String _projectIndex;
  int _totalJobLines;

  TextLayer *_nameLayer;
  TextLayer *_material;
  TextLayer *_resolution;
  TextLayer *_infill;
  TextLayer *_support;
  TextLayer *_filament;

};

#endif
