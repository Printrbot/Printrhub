#ifndef CALIBRATE_SCENE_H
#define CALIBRATE_SCENE_H

#include "../SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/layers/TextLayer.h"

class CalibrateScene: public SidebarSceneController {
public:

	CalibrateScene();
  virtual ~CalibrateScene();

	virtual void onSidebarButtonTouchUp() override;

	virtual uint16_t getBackgroundColor() override;
	virtual UIBitmap * getSidebarBitmap() override;
	virtual UIBitmap * getSidebarIcon() override;

private:
  virtual void onWillAppear() override;
  String getName() override;
  virtual void buttonPressed(void *button) override;
	float _offset;

protected:
	TextLayer* _offsetText;
	BitmapButton* _saveBtn;
	BitmapButton* _plusBtn;
	BitmapButton* _minusBtn;

};

#endif
