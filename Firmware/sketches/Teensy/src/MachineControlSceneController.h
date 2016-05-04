//
// Created by Phillip Schuster on 20.02.16.
//

#ifndef TEENSYCMAKE_MACHINECONTROLSCENECONTROLLER_H
#define TEENSYCMAKE_MACHINECONTROLSCENECONTROLLER_H

#include "SceneController.h"
#include "BitmapButton.h"
#include "LabelView.h"
#include "LabelButton.h"

class MachineControlSceneController: public SceneController, ButtonDelegate
{
public:
    MachineControlSceneController();
    virtual ~MachineControlSceneController();

    String getName();

    virtual void display();

protected:
    LabelButton* _moveZUpButton;
    LabelButton* _moveZDownButton;
    LabelButton* _moveXRightButton;
    LabelButton* _moveXLeftButton;

private:
    virtual void buttonPressed(void *button);

public:
    virtual uint16_t getBackgroundColor() override;
};

#endif //TEENSYCMAKE_MACHINECONTROLSCENECONTROLLER_H
