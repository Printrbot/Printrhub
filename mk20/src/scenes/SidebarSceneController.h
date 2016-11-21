/*
 * Implements the base class for the user interface in the hub. Shows a steady
 * sidebar with info text (where you are) and a small button at the bottom that acts
 * as a back button
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

#ifndef TEENSYCMAKE_SIDEBARSCENECONTROLLER_H
#define TEENSYCMAKE_SIDEBARSCENECONTROLLER_H

#include "framework/core/SceneController.h"
#include "framework/layers/VerticalTextLayer.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/BitmapView.h"

class SidebarSceneController : public SceneController, public ButtonDelegate {
 public:
  virtual void buttonPressed(void *button) override;
  virtual void setupDisplay() override;
  SidebarSceneController();
  virtual ~SidebarSceneController();
  virtual UIBitmap *getSidebarIcon() = 0;
  virtual UIBitmap *getSidebarBitmap() = 0;
  virtual BitmapButton *getSidebarButton() const { return _actionButton; };
  virtual void onSidebarButtonTouchUp();
  virtual void onWillAppear() override;
  virtual void onDidAppear() override;

 private:
  void setupSidebar();
  BitmapButton *_actionButton;
  BitmapView *_sidebarImage;
};

#endif
