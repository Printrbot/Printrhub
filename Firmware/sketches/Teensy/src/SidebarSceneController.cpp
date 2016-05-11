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
	_textLayer->setForegroundColor(Application.getTheme()->getColor(SidebarTextColor));
	_textLayer->setBackgroundColor(Application.getTheme()->getColor(SidebarBackgroundColor));
	_textLayer->setVerticalTextAlign(TEXTALIGN_CENTERED);
	_textLayer->setTextAlign(TEXTALIGN_RIGHT);
	_textLayer->setFont(&PTSansNarrow_24);
	_textLayer->setPadding(3);
	_textLayer->setContext(DisplayContext::Fixed);
	Display.addLayer(_textLayer);

	Rect bottomButtonRect(0,190,50,50);
	_actionButton = new BitmapButton(bottomButtonRect);
	_actionButton->setContext(DisplayContext::Fixed);
	_actionButton->setBackgroundColor(Application.getTheme()->getColor(HighlightBackgroundColor));
	_actionButton->setColor(Application.getTheme()->getColor(HighlighTextColor));
	_actionButton->setAlternateBackgroundColor(Application.getTheme()->getColor(HighlightAlternateBackgroundColor));
	_actionButton->setAlternateTextColor(Application.getTheme()->getColor(HighlightAlternateTextColor));
	_actionButton->setBitmap(getSidebarIcon(),24,24);
	addView(_actionButton);
}


void SidebarSceneController::setupDisplay()
{
	Display.setScrollInsets(50,0);
	Display.setScroll(0);
}


void SidebarSceneController::onWillAppear()
{
	setupSidebar();

	SceneController::onWillAppear();
}