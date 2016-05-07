//
// Created by Phillip Schuster on 29.04.16.
//

#ifndef TEENSYCMAKE_SIDEBARSCENECONTROLLER_H
#define TEENSYCMAKE_SIDEBARSCENECONTROLLER_H

#include "SceneController.h"
#include "VerticalTextLayer.h"
#include "BitmapButton.h"

class SidebarSceneController: public SceneController
{
#pragma mark Constructor
public:
    SidebarSceneController();
    virtual ~SidebarSceneController();

#pragma mark Getter/Setter
	virtual String getSidebarTitle() const = 0;
	virtual const uint8_t* getSidebarIcon() = 0;

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
