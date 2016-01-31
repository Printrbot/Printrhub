//
// Created by Phillip Schuster on 31.01.16.
//

#include "VirtualKeyboardSceneController.h"

//
// Created by Phillip Schuster on 20.01.16.
//

#include "MainSceneController.h"
#include "Bitmaps.h"
#include "LabelButton.h"
#include "Application.h"
#include "ChoosePrintSceneController.h"
#include "WiFiSetupSceneController.h"

VirtualKeyboardSceneController::VirtualKeyboardSceneController():
        SceneController::SceneController()
{
    char c = 65;
    for (int y=1;y<5;y++)
    {
        for (int x=0;x<8;x++)
        {
            Rect frame = Rect(x*39,y*39,38,38);

            LabelButton* button = new LabelButton(String(c),frame);
            button->setBorderColor(Application.getTheme()->getSecondaryColor2());
            button->setBackgroundColor(Application.getTheme()->getBackgroundColor());
            button->setAlternateBackgroundColor(Application.getTheme()->getPrimaryColor());
            button->setTextColor(Application.getTheme()->getSecondaryColor2());
            button->setBorderWidth(0);
            button->setName(String(c));
            button->setDelegate(this);
            addView(button);

            c++;
        }
    }
}

VirtualKeyboardSceneController::~VirtualKeyboardSceneController()
{

}

uint16_t VirtualKeyboardSceneController::getBackgroundColor()
{
    return Application.getTheme()->getSecondaryColor2();
}

String VirtualKeyboardSceneController::getName()
{
    return "VirtualKeyboardSceneController";
}

#pragma mark ButtonDelegate Implementation

void VirtualKeyboardSceneController::buttonPressed(void *button)
{
    LOG("VirtualKeyboardSceneController::buttonPressed");

}

