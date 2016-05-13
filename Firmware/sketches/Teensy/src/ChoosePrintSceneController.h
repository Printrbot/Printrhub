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

class ChoosePrintSceneController: public SidebarSceneController
{
#pragma mark Constructor
public:
	virtual void handleTouchMoved(TS_Point point, TS_Point oldPoint) override;

	virtual void animationFinished(Animation *animation) override;

	virtual void onSidebarButtonTouchUp() override;

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
    virtual void buttonPressed(void *button) override;

#pragma mark Member Variables
protected:
    ModelView* _modelView;
	LabelButton* _printButton;
};

#endif //TEENSYCMAKE_CHOOSEPRINTSCENECONTROLLER_H
