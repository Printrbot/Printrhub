//
// Created by Phillip Schuster on 28.01.16.
//

#include "WiFiSetupSceneController.h"
#include "Bitmaps.h"

WiFiSetupSceneController::WiFiSetupSceneController():
        SceneController::SceneController()
{
    Rect frame = Rect(10,(240-40)/2,40,40);

    for (int i=0;i<26;i++)
    {
        char c = 65 + i;
        LabelView* labelView = new LabelView(String(c),frame);
        labelView->setTextColor(ILI9341_BLACK);
        labelView->setBackgroundColor(ILI9341_CYAN);
        addView(labelView);
        _labelViews.push(labelView);

        frame.x += 60;
    }
}

WiFiSetupSceneController::~WiFiSetupSceneController()
{

}

void WiFiSetupSceneController::display()
{

}

String WiFiSetupSceneController::getName()
{
    return "WiFiSetupSceneController";
}

void WiFiSetupSceneController::onWillAppear()
{
    SceneController::onWillAppear();

    for (int i=0;i<_labelViews.count();i++)
    {
        LabelView* labelView = _labelViews.at(i);
        labelView->display();
    }
}

#pragma mark ButtonDelegate Implementation

void WiFiSetupSceneController::buttonPressed(void *button)
{
    LOG("MainSceneController::buttonPressed");

}
