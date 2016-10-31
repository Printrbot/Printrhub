#ifndef MK20_CONFIRM_DELETE_PROJECT_H
#define MK20_CONFIRM_DELETE_PROJECT_H

#include "../SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "IndexDb.h"

class ConfirmDeleteProject: public SidebarSceneController {
public:

	ConfirmDeleteProject(Project project);
  virtual ~ConfirmDeleteProject();

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
	BitmapButton* _yesBtn;
	BitmapButton* _noBtn;
  Project _project;
};

#endif
