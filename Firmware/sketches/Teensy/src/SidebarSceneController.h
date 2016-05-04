//
// Created by Phillip Schuster on 29.04.16.
//

#ifndef TEENSYCMAKE_SIDEBARSCENECONTROLLER_H
#define TEENSYCMAKE_SIDEBARSCENECONTROLLER_H

#include "SceneController.h"
#include "Sidebar.h"

class SidebarSceneController: public SceneController
{
public:
    SidebarSceneController();
    virtual ~SidebarSceneController();

    //virtual void loop();

	virtual String getSidebarTitle() = 0;
	virtual String getSidebarIcon() = 0;

	virtual void onWillAppear() override;

private:
	Sidebar* _sidebar;


};


#endif //TEENSYCMAKE_SIDEBARSCENECONTROLLER_H
