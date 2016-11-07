//
// Created by Phillip Schuster on 13.05.16.
//

#ifndef TEENSY_PAUSEPRINTSCENECONTROLLER_H
#define TEENSY_PAUSEPRINTSCENECONTROLLER_H

#include "SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/LabelButton.h"
#include "framework/views/ProgressBar.h"
#include "projects/ProjectsScene.h"
#include "projects/JobsScene.h"

typedef enum NextScene {
	StartPrint = 0,
	NewProject = 1,
	Materials = 2
};

class DownloadFileController: public SidebarSceneController
{

public:

	virtual void onSidebarButtonTouchUp() override;

  DownloadFileController(String url);
	DownloadFileController(String url, String localFilePath);
	DownloadFileController(String url, String localFilePath, String jobFilePath, Project project, Job job);
	DownloadFileController();
	virtual ~DownloadFileController();

	ProgressBar* getProgressBar() { return _progressBar; };
	virtual bool runTask(CommHeader& header, const uint8_t* data, size_t dataSize, uint8_t* responseData, uint16_t* responseDataSize, bool* sendResponse, bool* success);
	virtual bool handlesTask(TaskID taskID);

	virtual UIBitmap * getSidebarBitmap() override;
	virtual UIBitmap * getSidebarIcon() override;

private:
	String getName();
	virtual void onWillAppear() override;
	virtual uint16_t getBackgroundColor() override;

	virtual void buttonPressed(void *button) override;

protected:
	ProgressBar* _progressBar;
	File _file;
	uint32_t _fileSize;
  String _fileName;
	uint32_t _bytesRead;
	int _previousPercent;
	String _url;
	String _localFilePath;
	NextScene _nextScene;
	String _jobFilePath;
	Project _project;
	Job _job;
};


#endif //TEENSY_PAUSEPRINTSCENECONTROLLER_H
