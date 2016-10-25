#ifndef PROJECTS_SCENE_H
#define PROJECTS_SCENE_H

#include "../SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/LabelView.h"
#include "framework/views/LabelButton.h"
#include "ImageView.h"

typedef struct Project {
  char index[9];
  uint8_t rev;
  char title[32];
  uint8_t jobs;
} Project;

class ProjectsScene: public SidebarSceneController
{

public:
  ProjectsScene(const char* initialIndex = NULL);
  virtual ~ProjectsScene();

  virtual void handleTouchMoved(TS_Point point, TS_Point oldPoint) override;
  virtual void animationFinished(Animation *animation) override;
  virtual void onSidebarButtonTouchUp() override;
  virtual UIBitmap * getSidebarBitmap() override;
  virtual UIBitmap * getSidebarIcon() override;

private:
	virtual void onWillAppear() override;
	String getName() override;
	virtual void buttonPressed(void *button) override;
	uint16_t _totalProjects;

protected:
	BitmapButton* _openBtn;
	BitmapButton* _deleteBtn;
	char _initialIndex[9];
};

#endif
