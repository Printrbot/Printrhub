//
// Created by Phillip Schuster on 20.02.16.
//

#include "MachineControlSceneController.h"

MachineControlSceneController::MachineControlSceneController():
        SceneController::SceneController()
{
    _moveZUpButton = new LabelButton("Z-Up",Rect(0,0,159,119));
    _moveZUpButton->setBorderColor(Application.getTheme()->getSecondaryColor2());
    _moveZUpButton->setBackgroundColor(Application.getTheme()->getBackgroundColor());
    _moveZUpButton->setAlternateBackgroundColor(Application.getTheme()->getPrimaryColor());
    _moveZUpButton->setTextColor(Application.getTheme()->getSecondaryColor2());
    _moveZUpButton->setBorderWidth(0);
    _moveZUpButton->setName("Move Z Up");
    _moveZUpButton->setDelegate(this);
    addView(_moveZUpButton);

    _moveZDownButton = new LabelButton("Z-Down",Rect(160,0,159,119));
    _moveZDownButton->setBorderColor(Application.getTheme()->getSecondaryColor2());
    _moveZDownButton->setBackgroundColor(Application.getTheme()->getBackgroundColor());
    _moveZDownButton->setAlternateBackgroundColor(Application.getTheme()->getPrimaryColor());
    _moveZDownButton->setTextColor(Application.getTheme()->getSecondaryColor2());
    _moveZDownButton->setBorderWidth(0);
    _moveZDownButton->setName("Move Z Down");
    _moveZDownButton->setDelegate(this);
    addView(_moveZDownButton);

    _moveXLeftButton = new LabelButton("X-Left",Rect(0,120,159,120));
    _moveXLeftButton->setBorderColor(Application.getTheme()->getSecondaryColor2());
    _moveXLeftButton->setBackgroundColor(Application.getTheme()->getBackgroundColor());
    _moveXLeftButton->setAlternateBackgroundColor(Application.getTheme()->getPrimaryColor());
    _moveXLeftButton->setTextColor(Application.getTheme()->getSecondaryColor2());
    _moveXLeftButton->setName("Move X Left");
    _moveXLeftButton->setBorderWidth(0);
    _moveXLeftButton->setDelegate(this);
    addView(_moveXLeftButton);

    _moveXRightButton= new LabelButton("X-Right",Rect(160,120,159,120));
    _moveXRightButton->setBorderColor(Application.getTheme()->getSecondaryColor2());
    _moveXRightButton->setBackgroundColor(Application.getTheme()->getBackgroundColor());
    _moveXRightButton->setAlternateBackgroundColor(Application.getTheme()->getPrimaryColor());
    _moveXRightButton->setTextColor(Application.getTheme()->getSecondaryColor2());
    _moveXRightButton->setName("Move X Right");
    _moveXRightButton->setBorderWidth(0);
    _moveXRightButton->setDelegate(this);
    addView(_moveXRightButton);
}

MachineControlSceneController::~MachineControlSceneController()
{

}


uint16_t MachineControlSceneController::getBackgroundColor()
{
    return Application.getTheme()->getSecondaryColor2();
}

void MachineControlSceneController::display()
{

}

String MachineControlSceneController::getName()
{
    return "MachineControlSceneController";
}

void MachineControlSceneController::setup()
{
    SceneController::setup();
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
