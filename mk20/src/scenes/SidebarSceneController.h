//
// Created by Phillip Schuster on 29.04.16.
//

#ifndef TEENSYCMAKE_SIDEBARSCENECONTROLLER_H
#define TEENSYCMAKE_SIDEBARSCENECONTROLLER_H

#include "framework/core/SceneController.h"
#include "framework/layers/VerticalTextLayer.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/BitmapView.h"

class SidebarSceneController: public SceneController, public ButtonDelegate
{
public:
	virtual void buttonPressed(void *button) override;
	virtual void setupDisplay() override;
	SidebarSceneController();
  virtual ~SidebarSceneController();
	virtual UIBitmap * getSidebarIcon() = 0;
	virtual UIBitmap * getSidebarBitmap() = 0;
	virtual BitmapButton* getSidebarButton() const { return _actionButton; };
	virtual void onSidebarButtonTouchUp();
	virtual void onWillAppear() override;
	virtual void onDidAppear() override;

private:
	void setupSidebar();
	BitmapButton* _actionButton;
	BitmapView* _sidebarImage;
};


#endif
