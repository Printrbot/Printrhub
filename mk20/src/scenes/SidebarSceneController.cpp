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

#include "SidebarSceneController.h"
#include "UIBitmaps.h"
#include "framework/views/BitmapView.h"
#include "framework/core/Application.h"

extern UIBitmaps uiBitmaps;

SidebarSceneController::SidebarSceneController() :
	SceneController(),
	_actionButton(NULL),
	_sidebarImage(NULL) {

}

SidebarSceneController::~SidebarSceneController() {

}

void SidebarSceneController::setupSidebar() {
  // setup action button
  Rect bottomButtonRect(0, 190, 50, 50);
  _actionButton = new BitmapButton(bottomButtonRect);
  _actionButton->setContext(DisplayContext::Fixed);
  //_actionButton->setBackgroundColor(Application.getTheme()->getColor(HighlightBackgroundColor));
  //_actionButton->setColor(Application.getTheme()->getColor(HighlighTextColor));
  //_actionButton->setAlternateBackgroundColor(Application.getTheme()->getColor(HighlightAlternateBackgroundColor));
  //_actionButton->setAlternateTextColor(Application.getTheme()->getColor(HighlightAlternateTextColor));
  _actionButton->setBitmap(getSidebarIcon());
  _actionButton->setDelegate(this);
  addView(_actionButton);

  if (getSidebarBitmap()) {
	_sidebarImage = new BitmapView(Rect(0, 0, 50, 190));
	_sidebarImage->setContext(DisplayContext::Fixed);
	_sidebarImage->setBitmap(getSidebarBitmap());
	addView(_sidebarImage);
  } else {
	Display.fillRect(0, 0, 50, 240, Application.getTheme()->getColor(SidebarBackgroundColor));
  }

}

void SidebarSceneController::setupDisplay() {
  Display.setScrollInsets(50, 0);
  Display.setScroll(0);
}

void SidebarSceneController::onWillAppear() {
  setupSidebar();
  SceneController::onWillAppear();
}

void SidebarSceneController::onDidAppear() {
  if (_sidebarImage != NULL) {
	_sidebarImage->setNeedsDisplay();
  }
}

void SidebarSceneController::onSidebarButtonTouchUp() {

}

void SidebarSceneController::buttonPressed(void *button) {
  if (button == _actionButton) {
	onSidebarButtonTouchUp();
  }
}
