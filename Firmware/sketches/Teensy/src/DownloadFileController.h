//
// Created by Phillip Schuster on 13.05.16.
//

#ifndef TEENSY_PAUSEPRINTSCENECONTROLLER_H
#define TEENSY_PAUSEPRINTSCENECONTROLLER_H

#include "SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/LabelButton.h"
#include "framework/views/ProgressBar.h"

class DownloadFileController: public SidebarSceneController
{
#pragma mark Constructor
public:
	virtual void onSidebarButtonTouchUp() override;

	DownloadFileController(String host, int port, String remoteFilePath, String localFilePath);
	virtual ~DownloadFileController();

#pragma mark Sidebar Scene Controller
private:
	virtual const uint8_t *getSidebarIcon() override;
	virtual String getSidebarTitle() const override;

#pragma mark Scene Controller
private:
	String getName();
	virtual void onWillAppear() override;
	virtual uint16_t getBackgroundColor() override;
	virtual bool handlesTask(TaskID taskID);
	virtual bool runTask(CommHeader& header, const uint8_t* data, size_t dataSize, uint8_t* responseData, uint16_t* responseDataSize, bool* sendResponse, bool* success);

#pragma mark Button Delegate
private:
	virtual void buttonPressed(void *button) override;

#pragma mark Member functions
public:
	ProgressBar* getProgressBar() { return _progressBar; };

#pragma mark Member Variables
private:
	ProgressBar* _progressBar;
	TextLayer* _nameLayer;
	File _file;
	uint32_t _fileSize;
	uint32_t _bytesRead;
	int _previousPercent;
	String _host;
	String _port;
	String _remoteFilePath;
	String _localFilePath;
};


#endif //TEENSY_PAUSEPRINTSCENECONTROLLER_H
