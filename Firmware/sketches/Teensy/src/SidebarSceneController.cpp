//
// Created by Phillip Schuster on 29.04.16.
//

#include "SidebarSceneController.h"

SidebarSceneController::SidebarSceneController(): SceneController()
{

}

SidebarSceneController::~SidebarSceneController()
{

}

void SidebarSceneController::onWillAppear()
{
	//Display.setScrollInsets(50,0);

	_sidebar = new Sidebar(getSidebarTitle(),getSidebarIcon(),Rect(0,0,50,240));
	addView(_sidebar);

	SceneController::onWillAppear();
}