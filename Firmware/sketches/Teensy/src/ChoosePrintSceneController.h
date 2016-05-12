//
// Created by Phillip Schuster on 30.01.16.
//

#ifndef TEENSYCMAKE_CHOOSEPRINTSCENECONTROLLER_H
#define TEENSYCMAKE_CHOOSEPRINTSCENECONTROLLER_H

#include "SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/LabelView.h"
#include "framework/views/LabelButton.h"
#include "ModelView.h"

class ChoosePrintSceneController: public SidebarSceneController, ButtonDelegate
{
#pragma mark Constructor
public:
	virtual String getSidebarTitle() const override;

	virtual const uint8_t *getSidebarIcon() override;

	ChoosePrintSceneController();
    virtual ~ChoosePrintSceneController();

#pragma mark Scene Controller
private:
    virtual void onWillAppear() override;
    virtual uint16_t getBackgroundColor() override;
    String getName() override;

#pragma mark Button Delegate
private:
    virtual void buttonPressed(void *button);

#pragma mark Member Variables
protected:
    ModelView* _modelView;
};

#endif //TEENSYCMAKE_CHOOSEPRINTSCENECONTROLLER_H
