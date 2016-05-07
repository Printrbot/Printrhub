//
// Created by Phillip Schuster on 30.01.16.
//

#ifndef TEENSYCMAKE_CHOOSEPRINTSCENECONTROLLER_H
#define TEENSYCMAKE_CHOOSEPRINTSCENECONTROLLER_H

#include "SceneController.h"
#include "BitmapButton.h"
#include "LabelView.h"
#include "LabelButton.h"
#include "ModelView.h"

class ChoosePrintSceneController: public SceneController, ButtonDelegate
{
#pragma mark Constructor
public:
    ChoosePrintSceneController();
    virtual ~ChoosePrintSceneController();

#pragma mark Scene Controller
private:
    virtual void onWillAppear() override;
    virtual void handleTouchUp(TS_Point &point) override;
    virtual uint16_t getBackgroundColor() override;
    virtual void loop() override;
    String getName() override;

#pragma mark Button Delegate
private:
    virtual void buttonPressed(void *button);

#pragma mark Member Variables
protected:
    ModelView* _modelView;
};

#endif //TEENSYCMAKE_CHOOSEPRINTSCENECONTROLLER_H
