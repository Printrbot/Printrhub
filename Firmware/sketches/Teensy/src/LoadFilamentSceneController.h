//
// Created by Phillip Schuster on 04.05.16.
//

#ifndef TEENSY_LOADFILAMENTSCENECONTROLLER_H
#define TEENSY_LOADFILAMENTSCENECONTROLLER_H

#include "SidebarSceneController.h"

class LoadFilamentSceneController: public SidebarSceneController
{
public:
	LoadFilamentSceneController();
	virtual ~LoadFilamentSceneController();

	String getName();

protected:
	virtual String getSidebarTitle() override;
	virtual String getSidebarIcon() override;

private:
	virtual void onWillAppear() override;

public:
	virtual uint16_t getBackgroundColor() override;
};


#endif //TEENSY_LOADFILAMENTSCENECONTROLLER_H
