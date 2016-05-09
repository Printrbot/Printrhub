//
// Created by Phillip Schuster on 04.05.16.
//

#include "LoadFilamentSceneController.h"
#include "Bitmaps.h"
#include "CleanPlasticSceneController.h"
#include "ConfirmSceneController.h"

LoadFilamentSceneController::LoadFilamentSceneController():
SidebarSceneController::SidebarSceneController()
{

}

LoadFilamentSceneController::~LoadFilamentSceneController()
{

}

uint16_t LoadFilamentSceneController::getBackgroundColor()
{
	return Application.getTheme()->getColor(BackgroundColor);
}

String LoadFilamentSceneController::getName()
{
	return "LoadFilamentSceneController";
}


String LoadFilamentSceneController::getSidebarTitle() const
{
	return String("FILAMENT");
}


const uint8_t *LoadFilamentSceneController::getSidebarIcon()
{
	return imageOfCancelIcon_24_24;
}

void LoadFilamentSceneController::onWillAppear()
{
	BitmapLayer* iconLayer = new BitmapLayer(Rect(102+50,30,66,58));
	iconLayer->setBitmap(imageOfArrowIcon_66_58,66,58);
	iconLayer->setBackgroundColor(getBackgroundColor());
	iconLayer->setColor(Application.getTheme()->getColor(HighlightBackgroundColor));
	Display.addLayer(iconLayer);

	TextLayer* textLayer = new TextLayer(Rect(10+50,95,320-20-50,20));
	textLayer->setFont(&PTSansNarrow_18);
	textLayer->setTextAlign(TEXTALIGN_CENTERED);
	textLayer->setText("Load new filament.");
	Display.addLayer(textLayer);

	_button = new LabelButton("DONE",Rect(15+50,160,320-30-50,68));
	_button->setName("DONE");
	_button->setDelegate(this);
	_button->setIcon(imageOfAcceptIcon_32_30,Application.getTheme()->getColor(SuccessColor),32,30);
	addView(_button);

	SidebarSceneController::onWillAppear();
}

#pragma mark ButtonDelegate Implementation

void LoadFilamentSceneController::buttonPressed(void *button)
{
	if (button == _button)
	{
		ConfirmSceneController * scene = new ConfirmSceneController();
		Application.pushScene(scene);
	}
}

