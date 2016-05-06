//
// Created by Phillip Schuster on 04.05.16.
//

#ifndef TEENSY_LOADFILAMENTSCENECONTROLLER_H
#define TEENSY_LOADFILAMENTSCENECONTROLLER_H

#include "SidebarSceneController.h"
#include "BitmapButton.h"
#include "LabelButton.h"

class LoadFilamentSceneController: public SidebarSceneController, public ButtonDelegate
{
public:
	virtual void buttonPressed(void *button) override;

	LoadFilamentSceneController();
	virtual ~LoadFilamentSceneController();

private:
	virtual const uint8_t *getSidebarIcon() override;
	virtual String getSidebarTitle() const override;

	String getName();

	virtual void onWillAppear() override;

protected:
	virtual uint16_t getBackgroundColor() override;

private:
	LabelButton* _button;
};


#endif //TEENSY_LOADFILAMENTSCENECONTROLLER_H
