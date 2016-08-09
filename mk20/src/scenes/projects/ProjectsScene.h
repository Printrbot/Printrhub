#ifndef PROJECTS_SCENE_H
#define PROJECTS_SCENE_H

#include "../SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/LabelView.h"
#include "framework/views/LabelButton.h"
#include "ModelView.h"

struct ProjectData {
  char title[30];
  char job_file[16];
  char image_file[16];
};

class ProjectsScene: public SidebarSceneController
{

public:
  ProjectsScene();
  virtual ~ProjectsScene();

	virtual void handleTouchMoved(TS_Point point, TS_Point oldPoint) override;
	virtual void animationFinished(Animation *animation) override;
	virtual void onSidebarButtonTouchUp() override;
	virtual String getSidebarTitle() const override;
	virtual const uint8_t *getSidebarIcon() override;


private:
    virtual void onWillAppear() override;
    String getName() override;
    virtual void buttonPressed(void *button) override;

protected:
    ModelView* _modelView;
	   LabelButton* _printButton;
};

#endif
