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

public:
	virtual void onSidebarButtonTouchUp() override;

	DownloadFileController(String url, String localFilePath);
	DownloadFileController();
	virtual ~DownloadFileController();
	ProgressBar* getProgressBar() { return _progressBar; };
	virtual bool runTask(CommHeader& header, const uint8_t* data, size_t dataSize, uint8_t* responseData, uint16_t* responseDataSize, bool* sendResponse, bool* success);


private:
	virtual const uint8_t *getSidebarIcon() override;
	virtual String getSidebarTitle() const override;
	String getName();
	virtual void onWillAppear() override;
	virtual uint16_t getBackgroundColor() override;
	virtual bool handlesTask(TaskID taskID);
	virtual void buttonPressed(void *button) override;
	ProgressBar* _progressBar;
	TextLayer* _nameLayer;
	File _file;
	uint32_t _fileSize;
	uint32_t _bytesRead;
	int _previousPercent;
	String _url;
	String _localFilePath;
};


#endif //TEENSY_PAUSEPRINTSCENECONTROLLER_H
