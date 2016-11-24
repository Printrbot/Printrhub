/*
 * Downloads a file from the web via CommStack and ESP and handles request and
 * responses and UI with progress bar
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

#ifndef TEENSY_PAUSEPRINTSCENECONTROLLER_H
#define TEENSY_PAUSEPRINTSCENECONTROLLER_H

#include "SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/LabelButton.h"
#include "framework/views/ProgressBar.h"
#include "projects/ProjectsScene.h"
#include "projects/JobsScene.h"

typedef enum NextScene {
  StartPrint = 0,
  NewProject = 1,
  Materials = 2
};

class DownloadFileController : public SidebarSceneController {

 public:

  virtual void onSidebarButtonTouchUp() override;

  DownloadFileController(String url);
  DownloadFileController(String url, String localFilePath);
  DownloadFileController(String url, String localFilePath, String jobFilePath, Project project, Job job);
  DownloadFileController();
  virtual ~DownloadFileController();

  ProgressBar *getProgressBar() { return _progressBar; };
  virtual bool runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool *success);
  virtual bool handlesTask(TaskID taskID);

  virtual UIBitmap *getSidebarBitmap() override;
  virtual UIBitmap *getSidebarIcon() override;

 private:
  String getName();
  virtual void onWillAppear() override;
  virtual uint16_t getBackgroundColor() override;

  virtual void buttonPressed(void *button) override;

 protected:
  ProgressBar *_progressBar;
  File _file;
  uint32_t _fileSize;
  String _fileName;
  uint32_t _bytesRead;
  int _previousPercent;
  String _url;
  String _localFilePath;
  NextScene _nextScene;
  String _jobFilePath;
  Project _project;
  Job _job;
};

#endif //TEENSY_PAUSEPRINTSCENECONTROLLER_H
