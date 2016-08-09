//
// Created by Phillip Schuster on 29.04.16.
//

#ifndef TEENSYCMAKE_SIDEBARSCENECONTROLLER_H
#define TEENSYCMAKE_SIDEBARSCENECONTROLLER_H

#include "framework/core/SceneController.h"
#include "framework/layers/VerticalTextLayer.h"
#include "framework/views/BitmapButton.h"

class SidebarSceneController: public SceneController, public ButtonDelegate
{
#pragma mark Constructor
public:
	virtual void buttonPressed(void *button) override;

	virtual void setupDisplay() override;

	SidebarSceneController();
    virtual ~SidebarSceneController();

#pragma mark Getter/Setter
	virtual String getSidebarTitle() const = 0;
	virtual const uint8_t* getSidebarIcon() = 0;
	virtual BitmapButton* getSidebarButton() const { return _actionButton; };

#pragma mark Events
	virtual void onSidebarButtonTouchUp();

#pragma mark Scene Controller
	virtual void onWillAppear() override;

#pragma mark Member Functions
private:
	void setupSidebar();

#pragma mark Member Variables
private:
	VerticalTextLayer* _textLayer;
	BitmapButton* _actionButton;
};


#endif //TEENSYCMAKE_SIDEBARSCENECONTROLLER_H
