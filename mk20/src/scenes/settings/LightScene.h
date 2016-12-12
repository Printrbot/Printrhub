#ifndef LIGHT_SCENE_H
#define LIGHT_SCENE_H

#include "../SidebarSceneController.h"
#include "framework/views/BitmapButton.h"

class LightScene: public SidebarSceneController {
public:

	LightScene();
  virtual ~LightScene();
	virtual void onSidebarButtonTouchUp() override;

	virtual uint16_t getBackgroundColor() override;
	virtual UIBitmap * getSidebarBitmap() override;
	virtual UIBitmap * getSidebarIcon() override;

private:
  virtual void onWillAppear() override;
  String getName() override;
  virtual void buttonPressed(void *button) override;

protected:
  BitmapButton* _bulbBtnOn;
  BitmapButton* _bulbBtnOff;
	BitmapButton* _redBtn;
	BitmapButton* _yellowBtn;
  BitmapButton* _blueBtn;
  BitmapButton* _greenBtn;
  BitmapButton* _bulbBtn;
};

#endif
