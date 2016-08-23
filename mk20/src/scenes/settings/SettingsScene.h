#ifndef SETTING_SCENE_H
#define SETTING_SCENE_H

#include "../SidebarSceneController.h"
#include "framework/views/BitmapButton.h"

class SettingsScene: public SidebarSceneController {
public:

	SettingsScene();
  virtual ~SettingsScene();

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
		BitmapButton* _cloud;
		BitmapButton* _filament;
	  BitmapButton* _wifi;
    BitmapButton* _calibrate;
		BitmapButton* _update;
		BitmapButton* _password;
};

#endif
