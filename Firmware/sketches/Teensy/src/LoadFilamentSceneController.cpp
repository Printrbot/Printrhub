//
// Created by Phillip Schuster on 04.05.16.
//

#include "LoadFilamentSceneController.h"

LoadFilamentSceneController::LoadFilamentSceneController():
SidebarSceneController::SidebarSceneController()
{

}

LoadFilamentSceneController::~LoadFilamentSceneController()
{

}

uint16_t LoadFilamentSceneController::getBackgroundColor()
{
	return Application.getTheme()->getBackgroundColor();
}

String LoadFilamentSceneController::getName()
{
	return "LoadFilamentSceneController";
}


String LoadFilamentSceneController::getSidebarTitle()
{
	return "FILAMENT";
}

String LoadFilamentSceneController::getSidebarIcon()
{
	return "exchangeIcon";
}


void LoadFilamentSceneController::onWillAppear()
{
	SidebarSceneController::onWillAppear();


}

