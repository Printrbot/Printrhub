#ifndef JOBS_SCENE_H
#define JOBS_SCENE_H

#include "../SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/LabelView.h"
#include "framework/views/LabelButton.h"

typedef struct Job {
	char index[8];
	char title[32];
	char url[128];
} Job;

class JobsScene: public SidebarSceneController
{

public:
  JobsScene(String projectIndex);
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

protected:
	  BitmapButton* _printBtn;
};

#endif
