#ifndef PROJECTS_SCENE_H
#define PROJECTS_SCENE_H

#include "../SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/LabelView.h"
#include "framework/views/LabelButton.h"
#include "ImageView.h"
#include "IndexDb.h"


class ProjectsScene: public SidebarSceneController
{

public:
  ProjectsScene();
  virtual ~ProjectsScene();

  virtual void handleTouchMoved(TS_Point point, TS_Point oldPoint) override;
  virtual void animationFinished(Animation *animation) override;
  virtual void onSidebarButtonTouchUp() override;
  virtual UIBitmap * getSidebarBitmap() override;
  virtual UIBitmap * getSidebarIcon() override;

private:
	virtual void onWillAppear() override;
	virtual void onDidAppear() override;
	String getName() override;
	virtual void buttonPressed(void *button) override;
  void updateButtons();
  IndexDb * projectIndexDb;
protected:
	BitmapButton* _openBtn;
	BitmapButton* _deleteBtn;
};

#endif
