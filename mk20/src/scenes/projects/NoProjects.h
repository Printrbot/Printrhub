#ifndef NO_PROJECTS_H
#define NO_PROJECTS_H

#include "../SidebarSceneController.h"

class NoProjects: public SidebarSceneController {
public:

	NoProjects();
  virtual ~NoProjects();

	virtual void onSidebarButtonTouchUp() override;

	virtual uint16_t getBackgroundColor() override;
	virtual UIBitmap * getSidebarBitmap() override;
	virtual UIBitmap * getSidebarIcon() override;

private:
    virtual void onWillAppear() override;
    String getName() override;
    virtual void buttonPressed(void *button) override;

};

#endif
