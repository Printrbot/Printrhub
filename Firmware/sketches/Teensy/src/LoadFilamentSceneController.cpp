//
// Created by Phillip Schuster on 04.05.16.
//

#include "LoadFilamentSceneController.h"
#include "Bitmaps.h"
#include "CleanPlasticSceneController.h"
#include "ConfirmSceneController.h"
#include "framework/views/ProgressBar.h"

LoadFilamentSceneController::LoadFilamentSceneController():
SidebarSceneController::SidebarSceneController()
{
	_step = 0.05f;
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
	BitmapLayer* iconLayer = new BitmapLayer(Rect(102,30,66,58));
	iconLayer->setBitmap(imageOfArrowIcon_66_58,66,58);
	iconLayer->setBackgroundColor(getBackgroundColor());
	iconLayer->setColor(Application.getTheme()->getColor(HighlightBackgroundColor));
	Display.addLayer(iconLayer);

	TextLayer* textLayer = new TextLayer(Rect(10,95,270-20,20));
	textLayer->setFont(&PTSansNarrow_18);
	textLayer->setTextAlign(TEXTALIGN_CENTERED);
	textLayer->setText("Load new filament.");
	Display.addLayer(textLayer);

	_button = new LabelButton("DONE",Rect(15,120,270-30,68));
	_button->setName("DONE");
	_button->setDelegate(this);
	_button->setIcon(imageOfAcceptIcon_32_30,Application.getTheme()->getColor(SuccessColor),32,30);
	addView(_button);

	_progressBar = new ProgressBar(Rect(15,210,270-30,15));
	_progressBar->setValue(0.0f);
	addView(_progressBar);

	SidebarSceneController::onWillAppear();
}


void LoadFilamentSceneController::loop()
{
	if (_progressBar != NULL)
	{
		_progressBar->setValue(_progressBar->getValue()+_step);
		if (_progressBar->getValue() >= 1)
		{
			_step = -_step;
		}
		else if (_progressBar->getValue() <= 0)
		{
			_step = -_step;
		}
	}

	SceneController::loop();
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

