//
// Created by Phillip Schuster on 20.02.16.
//

#include "MachineControlSceneController.h"

MachineControlSceneController::MachineControlSceneController():
        SceneController::SceneController()
{
    _moveZUpButton = new LabelButton("Z-Up",Rect(0,0,159,119));
    _moveZUpButton->setName("Move Z Up");
    _moveZUpButton->setDelegate(this);
    addView(_moveZUpButton);

    _moveZDownButton = new LabelButton("Z-Down",Rect(160,0,159,119));
    _moveZDownButton->setName("Move Z Down");
    _moveZDownButton->setDelegate(this);
    addView(_moveZDownButton);

    _moveXLeftButton = new LabelButton("X-Left",Rect(0,120,159,120));
    _moveXLeftButton->setDelegate(this);
    addView(_moveXLeftButton);

    _moveXRightButton= new LabelButton("X-Right",Rect(160,120,159,120));
    _moveXRightButton->setDelegate(this);
    addView(_moveXRightButton);
}

MachineControlSceneController::~MachineControlSceneController()
{

}


uint16_t MachineControlSceneController::getBackgroundColor()
{
    return Application.getTheme()->getColor(SpacerColor);
}

void MachineControlSceneController::display()
{

}

String MachineControlSceneController::getName()
{
    return "MachineControlSceneController";
}

#pragma mark ButtonDelegate Implementation

void MachineControlSceneController::buttonPressed(void *button)
{
    LOG("MachineControlSceneController::buttonPressed");

    Serial1.println("G91"); //Relative Movement

    if (button == _moveZUpButton)
    {

        Serial1.println("G1 Z10");
    }
    else if (button == _moveZDownButton)
    {
        Serial1.println("G1 Z-10");
    }
    else if (button == _moveXLeftButton)
    {
        Serial1.println("G1 X10");
    }
    else if (button == _moveXRightButton)
    {
        Serial1.println("G1 X-10");
    }

    Serial1.println("G90"); //Absolute Movement
}
