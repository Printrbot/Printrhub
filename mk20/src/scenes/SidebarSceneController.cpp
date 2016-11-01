//
// Created by Phillip Schuster on 29.04.16.
//

#include "SidebarSceneController.h"
#include "UIBitmaps.h"
#include "framework/views/BitmapView.h"
#include "framework/core/Application.h"

extern UIBitmaps uiBitmaps;

SidebarSceneController::SidebarSceneController():
SceneController(),
_actionButton(NULL),
_sidebarImage(NULL)
{

}

SidebarSceneController::~SidebarSceneController() {

}

void SidebarSceneController::setupSidebar() {
	// setup action button
	 Rect bottomButtonRect(0,190,50,50);
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
		_sidebarImage = new BitmapView(Rect(0,0,50, 190));
		_sidebarImage->setContext(DisplayContext::Fixed);
		_sidebarImage->setBitmap(getSidebarBitmap());
		addView(_sidebarImage);
	} else {
		Display.fillRect(0,0,50,240, Application.getTheme()->getColor(SidebarBackgroundColor));
	}

}

void SidebarSceneController::setupDisplay() {
	Display.setScrollInsets(50,0);
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
