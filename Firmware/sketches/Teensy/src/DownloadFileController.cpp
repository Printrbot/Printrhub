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

DownloadFileController::DownloadFileController(String host, int port, String remoteFilePath, String localFilePath) :
		SidebarSceneController::SidebarSceneController(),
		_fileSize(0),
		_bytesRead(0),
		_previousPercent(0),
		_remoteFilePath(remoteFilePath),
		_localFilePath(localFilePath),
		_port(port),
		_host(host)
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
	const char* jobID = _remoteFilePath.c_str();
	Application.getESPStack()->requestTask(GetJobWithID,strlen(jobID),(uint8_t*)jobID);

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

bool DownloadFileController::runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool* sendResponse, bool* success)
{
	LOG_VALUE("DownloadFileController handling task",header.getCurrentTask());

	if (header.getCurrentTask() == GetJobWithID)
	{
		LOG("Handling GetJobWithID Task");

		if (header.commType == ResponseSuccess)
		{
			if (dataSize != sizeof(uint32_t))
			{
				LOG_VALUE("Expected content of GetJobWithID to be uint32_t with the number of bytes to receive, but received a content length of",*responseDataSize);
			}
			else
			{
				uint32_t contentLength;
				memcpy(&contentLength,data,sizeof(uint32_t));
				_fileSize = contentLength;
				_bytesRead = 0;

				LOG_VALUE("Expected file size of",_fileSize);

				//Open a file on SD card
				SD.remove(_localFilePath.c_str());
				_file = SD.open(_localFilePath.c_str(),FILE_WRITE);
				if (!_file.available())
				{
					//TODO: We should handle that. For now we will have to read data from ESP to clean the pipe but there should be better ways to handle errors
					//Application.getESPStack()->requestTask(Error);
					//return false;
				}

				LOG("File opened for writing. Now waiting for number of bytes to read");
				LOG("Now Expecting file chunks with FileSendData tasks");
			}
		}
		else
		{
			//TODO: Show an error message
			ChoosePrintSceneController* scene = new ChoosePrintSceneController();
			Application.pushScene(scene);
		}
	}
	else if (header.getCurrentTask() == FileSendData)
	{
		LOG("Handling FileSendData Task");
		if (header.commType == Request)
		{
			LOG_VALUE("Received Chunk of Data with Size",dataSize);
			int numBytesWritten = _file.write(data,dataSize);
			LOG_VALUE("Written number of bytes to file",numBytesWritten);

			//That's it. We don't send a response
			*sendResponse = true;
			*responseDataSize = 0;

			//Add number of bytes received to total bytes read
			_bytesRead += dataSize;

			float fraction = (float)_bytesRead / (float)_fileSize;

			int percent = (int)(fraction*100.0f);
			if (percent != _previousPercent)
			{
				LOG_VALUE("Progress Bar Percent",percent);
				if (percent % 5 == 0)
				{
					LOG_VALUE("Updating Bar with fraction",fraction);
					//Only update the progress bar once in a while as the communication is blocked while doing so and we don't want to loose too much speed
					_progressBar->setValue(fraction);
				}
			}
			_previousPercent = percent;
		}
	}
	else if (header.getCurrentTask() == FileClose)
	{
		LOG("Handling FileClose Task");
		LOG_VALUE("Bytes read",_bytesRead);
		_file.close();

		PrintStatusSceneController* scene = new PrintStatusSceneController();
		Application.pushScene(scene);
	}

	return true;
/*	else if (header.getCurrentTask() == FileSendData)
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
	}*/
}

#pragma mark ButtonDelegate Implementation

void DownloadFileController::buttonPressed(void *button)
{

	SidebarSceneController::buttonPressed(button);
}

