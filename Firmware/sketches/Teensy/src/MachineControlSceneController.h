//
// Created by Phillip Schuster on 20.02.16.
//

#ifndef TEENSYCMAKE_MACHINECONTROLSCENECONTROLLER_H
#define TEENSYCMAKE_MACHINECONTROLSCENECONTROLLER_H

#include "framework/core/SceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/LabelView.h"
#include "framework/views/LabelButton.h"

class MachineControlSceneController: public SceneController, ButtonDelegate
{
#pragma mark Constructor
public:
    MachineControlSceneController();
    virtual ~MachineControlSceneController();

#pragma mark SceneController
private:
    String getName();
    virtual void display();
    virtual uint16_t getBackgroundColor() override;

#pragma mark Button Delegate
private:
    virtual void buttonPressed(void *button);

#pragma mark Member Variables
protected:
    LabelButton* _moveZUpButton;
    LabelButton* _moveZDownButton;
    LabelButton* _moveXRightButton;
    LabelButton* _moveXLeftButton;
};

#endif //TEENSYCMAKE_MACHINECONTROLSCENECONTROLLER_H
