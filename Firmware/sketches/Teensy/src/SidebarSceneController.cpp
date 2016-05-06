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

void SidebarSceneController::setupSidebar()
{
	Rect textLayerRect(0,0,50,190);
	_textLayer = new VerticalTextLayer(textLayerRect);
	_textLayer->setText(getSidebarTitle());
	_textLayer->setForegroundColor(Application.getTheme()->getTextColor(ColorTheme::Shade::Light));
	_textLayer->setBackgroundColor(Application.getTheme()->getPrimaryColor(ColorTheme::Shade::Default));
	_textLayer->setVerticalTextAlign(TEXTALIGN_CENTERED);
	_textLayer->setTextAlign(TEXTALIGN_RIGHT);
	_textLayer->setFont(&PTSansNarrow_24);
	_textLayer->setPadding(3);
	Display.addLayer(_textLayer);

	Rect bottomButtonRect(0,190,50,50);
	_actionButton = new BitmapButton(bottomButtonRect);
	_actionButton->setBackgroundColor(Application.getTheme()->getPrimaryColor(ColorTheme::Shade::Darker));
	_actionButton->setColor(Application.getTheme()->getTextColor(ColorTheme::Shade::Light));
	_actionButton->setAlternateBackgroundColor(Application.getTheme()->getBackgroundColor(ColorTheme::Shade::Default));
	_actionButton->setAlternateTextColor(Application.getTheme()->getTextColor(ColorTheme::Shade::Darker));
	_actionButton->setBitmap(getSidebarIcon(),24,24);
	addView(_actionButton);
}

void SidebarSceneController::onWillAppear()
{
	setupSidebar();

	SceneController::onWillAppear();

	//Display.setScrollInsets(50,0);
}