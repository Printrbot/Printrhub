//
// Created by Phillip Schuster on 13.05.16.
//

#include "DownloadFileController.h"

#include "Bitmaps.h"
#include "CleanPlasticSceneController.h"
#include "ConfirmSceneController.h"
#include "framework/views/ProgressBar.h"
#include "ChoosePrintSceneController.h"
#include "PrintStatusSceneController.h"
#include "MainSceneController.h"

DownloadFileController::DownloadFileController():
		SidebarSceneController::SidebarSceneController()
{

}

DownloadFileController::~DownloadFileController()
{

}

uint16_t DownloadFileController::getBackgroundColor()
{
	return Application.getTheme()->getColor(BackgroundColor);
}

String DownloadFileController::getName()
{
	return "DownloadFileController";
}


String DownloadFileController::getSidebarTitle() const
{
	return String("DOWNLOAD");
}


const uint8_t *DownloadFileController::getSidebarIcon()
{
	return imageOfCancelIcon_24_24;
}

void DownloadFileController::onWillAppear()
{
	_nameLayer = new TextLayer(Rect(15,10,Display.getLayoutWidth()-30,25));
	_nameLayer->setTextAlign(TEXTALIGN_LEFT);
	_nameLayer->setFont(&PTSansNarrow_24);
	_nameLayer->setText("Downloading File");
	_nameLayer->setForegroundColor(Application.getTheme()->getColor(TextColor));
	Display.addLayer(_nameLayer);

	_progressBar = new ProgressBar(Rect(15,215,Display.getLayoutWidth()-30,7));
	_progressBar->setValue(0.0f);
	addView(_progressBar);

	//Trigger file download
	if (Application.getESPStack()->requestTask(GetJobWithID))
	//if (Application.getESPStack()->requestTask(GetProjectItemWithID))
	{
		//Now send the unique ID of the project - this is just a hardcoded junk hash
		Application.getESPStack()->getPort()->println("/files/firmware.bin");
	}

	SidebarSceneController::onWillAppear();
}

void DownloadFileController::onSidebarButtonTouchUp()
{
	ChoosePrintSceneController* scene = new ChoosePrintSceneController();
	Application.pushScene(scene);
}


#pragma mark ButtonDelegate Implementation

void DownloadFileController::buttonPressed(void *button)
{

	SidebarSceneController::buttonPressed(button);
}

