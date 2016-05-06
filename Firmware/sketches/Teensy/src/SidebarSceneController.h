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
public:
    SidebarSceneController();
    virtual ~SidebarSceneController();

    //virtual void loop();

	virtual String getSidebarTitle() const = 0;
	virtual const uint8_t* getSidebarIcon() = 0;

	virtual void onWillAppear() override;

private:
	VerticalTextLayer* _textLayer;
	BitmapButton* _actionButton;

	void setupSidebar();
};


#endif //TEENSYCMAKE_SIDEBARSCENECONTROLLER_H
