//
// Created by Phillip Schuster on 13.05.16.
//

#ifndef TEENSY_PAUSEPRINTSCENECONTROLLER_H
#define TEENSY_PAUSEPRINTSCENECONTROLLER_H

#include "SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/LabelButton.h"
#include "framework/views/ProgressBar.h"

class DownloadFileController: public SidebarSceneController
{
#pragma mark Constructor
public:
	virtual void onSidebarButtonTouchUp() override;

	DownloadFileController();
	virtual ~DownloadFileController();

#pragma mark Sidebar Scene Controller
private:
	virtual const uint8_t *getSidebarIcon() override;
	virtual String getSidebarTitle() const override;

#pragma mark Scene Controller
private:
	String getName();
	virtual void onWillAppear() override;
	virtual uint16_t getBackgroundColor() override;

#pragma mark Button Delegate
private:
	virtual void buttonPressed(void *button) override;

#pragma mark Member functions
public:
	ProgressBar* getProgressBar() { return _progressBar; };

#pragma mark Member Variables
private:
	ProgressBar* _progressBar;
	TextLayer* _nameLayer;
};


#endif //TEENSY_PAUSEPRINTSCENECONTROLLER_H
