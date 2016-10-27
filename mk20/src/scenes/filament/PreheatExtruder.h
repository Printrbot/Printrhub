#ifndef PREHEAT_EXTRUDER_H
#define PREHEAT_EXTRUDER_H

#include "../SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/ProgressBar.h"
#include "../../Printr.h"

class PreheatExtruder: public SidebarSceneController, public PrintrListener {
public:

	PreheatExtruder();
  virtual ~PreheatExtruder();
	virtual void onSidebarButtonTouchUp() override;
	virtual uint16_t getBackgroundColor() override;
	virtual UIBitmap * getSidebarBitmap() override;
	virtual UIBitmap * getSidebarIcon() override;

	virtual void onNewNozzleTemperature(float temp);
	virtual void onPrintProgress(float progress);
	virtual void onPrintComplete(bool success);

	void setNextScene(uint8_t scene) {_nextScene = scene;};

private:
  virtual void onWillAppear() override;
  String getName() override;
  virtual void buttonPressed(void *button) override;

	ProgressBar* _progressBar;
	int _targetTemp;
	uint8_t _nextScene;
};

#endif
