#ifndef MK20_CONFIRMCANCELPRINT_H
#define MK20_CONFIRMCANCELPRINT_H

#include "UIBitmaps.h"
#include "../SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "scenes/projects/ProjectsScene.h"
#include "scenes/projects/JobsScene.h"

class ConfirmCancelPrint: public SidebarSceneController {
public:

  ConfirmCancelPrint(String * jobFilePath, Project * project, Job * job);
  virtual ~ConfirmCancelPrint();

  virtual uint16_t getBackgroundColor() override;
  virtual void onSidebarButtonTouchUp() override;
  virtual UIBitmap * getSidebarBitmap() override;
  virtual UIBitmap * getSidebarIcon() override;

private:
  virtual void onWillAppear() override;
  virtual void onDidAppear() override;
  String getName() override;
  virtual void buttonPressed(void *button) override;

protected:
  Job _job;
  String _jobFilePath;
  String _projectIndex;
  Project _project;
  BitmapButton *_btnCancelPrint;
  BitmapButton *_btnBack;
};

#endif
