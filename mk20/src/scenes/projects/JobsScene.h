#ifndef JOBS_SCENE_H
#define JOBS_SCENE_H

#include "../SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/LabelView.h"
#include "framework/views/LabelButton.h"
#include "ProjectsScene.h"

typedef struct Job {
  char index[9];
	uint8_t rev;
	uint8_t timesPrinted;
  char title[32];
  char url[256];
} Job;

class JobsScene: public SidebarSceneController
{

public:
	JobsScene(String projectIndex);
	JobsScene();
	virtual ~JobsScene();

  virtual void handleTouchMoved(TS_Point point, TS_Point oldPoint) override;
  virtual void animationFinished(Animation *animation) override;
  virtual void onSidebarButtonTouchUp() override;
  virtual UIBitmap * getSidebarBitmap() override;
  virtual UIBitmap * getSidebarIcon() override;


private:
	virtual void onWillAppear() override;
	String getName() override;
	virtual void buttonPressed(void *button) override;
	String _projectIndex;
  Job* _jobs;
	Project _project;
	String _jobFilePath;
	Job _selectedJob;

protected:
	BitmapButton* _printBtnDownload;
  BitmapButton* _printBtnStart;
};

#endif
