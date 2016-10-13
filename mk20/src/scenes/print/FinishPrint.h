#ifndef MK20_FINISHPRINT_H
#define MK20_FINISHPRINT_H

#include "UIBitmaps.h"
#include "../SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "scenes/projects/ProjectsScene.h"
#include "scenes/projects/JobsScene.h"

class FinishPrint: public SidebarSceneController {
public:

  FinishPrint(String jobFilePath, Project project, Job job, uint16_t offset);
  virtual ~FinishPrint();

  virtual void handleTouchMoved(TS_Point point, TS_Point oldPoint) override;
  virtual void animationFinished(Animation *animation) override;
  virtual void onSidebarButtonTouchUp() override;

  virtual uint16_t getBackgroundColor() override;
  virtual UIBitmap * getSidebarBitmap() override;
  virtual UIBitmap * getSidebarIcon() override;

private:
  virtual void onWillAppear() override;
  String getName() override;
  virtual void buttonPressed(void *button) override;

  Job _job;
  String _jobFilePath;
  String _projectIndex;
  int _jobOffset;
  Project _project;

protected:
  BitmapButton *_btnYes;
  BitmapButton *_btnNo;
};

#endif
