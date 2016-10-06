#ifndef CONFIRM_SCENE_H
#define CONFIRM_SCENE_H

#include "../SidebarSceneController.h"
#include "framework/views/BitmapButton.h"

class ConfirmDeleteProject: public SidebarSceneController {
public:

	ConfirmDeleteProject(String filePath);
  virtual ~ConfirmDeleteProject();

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

protected:
		BitmapButton* _yesBtn;
		BitmapButton* _noBtn;
		String _filePath;
};

#endif
