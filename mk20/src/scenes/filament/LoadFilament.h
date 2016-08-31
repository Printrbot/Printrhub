#ifndef LOAD_FILAMENT_ACTION_H
#define LOAD_FILAMENT_ACTION_H

#include "../SidebarSceneController.h"
#include "framework/views/BitmapButton.h"

class LoadFilament: public SidebarSceneController {
public:

	LoadFilament();
  virtual ~LoadFilament();

	virtual void onSidebarButtonTouchUp() override;

	virtual uint16_t getBackgroundColor() override;
	virtual UIBitmap * getSidebarBitmap() override;
	virtual UIBitmap * getSidebarIcon() override;

private:
  virtual void onWillAppear() override;
  String getName() override;
  virtual void buttonPressed(void *button) override;

protected:
	BitmapButton* _doneBtn;
};

#endif
