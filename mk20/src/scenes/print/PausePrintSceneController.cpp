//
// Created by Phillip Schuster on 13.05.16.
//

#include "PausePrintSceneController.h"

#include "Bitmaps.h"
#include "CleanPlasticSceneController.h"
#include "../ConfirmSceneController.h"
#include "framework/views/ProgressBar.h"
#include "../projects/ProjectsScene.h"
#include "PrintStatusSceneController.h"

PausePrintSceneController::PausePrintSceneController():
		SidebarSceneController::SidebarSceneController()
{
	_step = 0.05f;
}

PausePrintSceneController::~PausePrintSceneController()
{

}

uint16_t PausePrintSceneController::getBackgroundColor()
{
	return Application.getTheme()->getColor(BackgroundColor);
}

String PausePrintSceneController::getName()
{
	return "PausePrintSceneController";
}


String PausePrintSceneController::getSidebarTitle() const
{
	return String("PAUSED");
}


const uint8_t *PausePrintSceneController::getSidebarIcon()
{
	return imageOfCancelIcon_24_24;
}

void PausePrintSceneController::onWillAppear()
{
	_changeFilamentButton = new LabelButton("CHANGE FILAMENT", Rect(15, 6, 270 - 30, 68));
	_changeFilamentButton->setName("CF");
	_changeFilamentButton->setDelegate(this);
	_changeFilamentButton->setIcon(imageOfHotendIcon_32_30, Application.getTheme()->getColor(HighlightBackgroundColor), 32, 30);
	addView(_changeFilamentButton);

	_cancelPrintButton = new LabelButton("CANCEL PRINT", Rect(15, 86, 270 - 30, 68));
	_cancelPrintButton->setName("CP");
	_cancelPrintButton->setDelegate(this);
	_cancelPrintButton->setIcon(imageOfCancelIcon_32_30, Application.getTheme()->getColor(AlertColor), 32, 30);
	addView(_cancelPrintButton);

	_resumePrintButton = new LabelButton("RESUME PRINT", Rect(15, 168, 270 - 30, 68));
	_resumePrintButton->setName("RP");
	_resumePrintButton->setDelegate(this);
	_resumePrintButton->setIcon(imageOfResumeIcon_32_30, Application.getTheme()->getColor(SuccessColor), 32, 30);
	addView(_resumePrintButton);

	SidebarSceneController::onWillAppear();
}

void PausePrintSceneController::onSidebarButtonTouchUp()
{
	PrintStatusSceneController * scene = new PrintStatusSceneController();
	Application.pushScene(scene);
}


#pragma mark ButtonDelegate Implementation

void PausePrintSceneController::buttonPressed(void *button)
{
	if (button == _resumePrintButton)
	{
		PrintStatusSceneController * scene = new PrintStatusSceneController();
		Application.pushScene(scene);
	}
	else if (button == _cancelPrintButton)
	{
		ProjectsScene * scene = new ProjectsScene();
		Application.pushScene(scene);
	}
	else if (button == _changeFilamentButton)
	{
		ConfirmSceneController * scene = new ConfirmSceneController();
		Application.pushScene(scene);
	}

	SidebarSceneController::buttonPressed(button);
}
