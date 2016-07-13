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

bool DownloadFileController::handlesTask(TaskID taskID)
{
	if (taskID == GetJobWithID || taskID == FileSendData || taskID == FileClose || taskID == Error) return true;
	return false;
}

bool DownloadFileController::runTask(CommHeader &header, Stream *stream)
{
	if (header.getCurrentTask() == GetJobWithID)
	{
		if (header.commType == Response)
		{
			//Wait for data to be arrived
			while (!stream->available())
			{
				delay(10);
			}

			//First we ask for the job id which is sent using println on the other side so we read until a newline char
			String jobID = stream->readStringUntil('\n');
			LOG_VALUE("Got Response for GetJobWithID",jobID);

			//Add file suffix to job
			jobID = jobID + ".gcode";

			//Open a file on SD card
			_file = SD.open("job.gcode",O_WRITE);
			if (!_file.available())
			{
				//TODO: We should handle that. For now we will have to read data from ESP to clean the pipe but there should be better ways to handle errors
				Application.getESPStack()->requestTask(Error);
				return false;
			}

			LOG("File opened for writing. Now waiting for number of bytes to read");
		}
	}
	else if (header.getCurrentTask() == FileSendData)
	{
		if (header.commType == Request)
		{
			//Wait for data to be arrived
			while (!stream->available())
			{
				delay(10);
			}

			uint8_t numberOfBytes = stream->read();
			uint8_t byteIndex = 0;

			while (!stream->available())
			{
				delay(10);
			}

			uint8_t buffer[numberOfBytes];
			if (stream->readBytes(buffer,numberOfBytes) == numberOfBytes)
			{
				_file.write(buffer,numberOfBytes);
			}
		}
	}
	else if (header.getCurrentTask() == FileClose)
	{
		_file.close();

		PrintStatusSceneController* scene = new PrintStatusSceneController();
		Application.pushScene(scene);
	}
}

#pragma mark ButtonDelegate Implementation

void DownloadFileController::buttonPressed(void *button)
{

	SidebarSceneController::buttonPressed(button);
}

