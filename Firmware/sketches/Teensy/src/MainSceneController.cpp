//
// Created by Phillip Schuster on 20.01.16.
//

#include "MainSceneController.h"
#include "Bitmaps.h"
#include "LabelButton.h"
#include "Application.h"
#include "NavBar.h"
#include "ChoosePrintSceneController.h"
#include "WiFiSetupSceneController.h"
#include "VirtualKeyboardSceneController.h"
#include "MachineControlSceneController.h"
#include "ColorTheme.h"
#include <ArduinoJson.h>

MainSceneController::MainSceneController():
SceneController::SceneController()
{
  _navBar = new NavBar(0,200,320,40);
  //_navBar->setBackgroundColor(Application.getTheme()->getPrimaryColor());
  addView(_navBar);



/*
    _printButton = new LabelButton("PRINT",Rect(0,0,159,119));
    //_printButton->setBorderColor(Application.getTheme()->getSecondaryColor2(Application.getTheme()->Shade::Lighter));
    _printButton->setBackgroundColor(Application.getTheme()->getPrimaryColor(Application.getTheme()->Shade::Lighter));
    _printButton->setAlternateBackgroundColor(Application.getTheme()->getPrimaryColor());
    _printButton->setTextColor(Application.getTheme()->getSecondaryColor2());
    _printButton->setBorderWidth(0);
    _printButton->setName("Print Button");
    _printButton->setDelegate(this);
    addView(_printButton);



    _hotendButton = new LabelButton("HOTEND",Rect(160,0,159,119));
    _hotendButton->setBorderColor(Application.getTheme()->getSecondaryColor2());
    _hotendButton->setBackgroundColor(Application.getTheme()->getBackgroundColor());
    _hotendButton->setAlternateBackgroundColor(Application.getTheme()->getPrimaryColor());
    _hotendButton->setTextColor(Application.getTheme()->getSecondaryColor2());
    _hotendButton->setBorderWidth(0);
    _hotendButton->setName("Hotend Button");
    _hotendButton->setDelegate(this);
    addView(_hotendButton);

    _filamentButton = new LabelButton("FILAMENT",Rect(0,120,159,120));
    _filamentButton->setBorderColor(Application.getTheme()->getSecondaryColor2());
    _filamentButton->setBackgroundColor(Application.getTheme()->getBackgroundColor());
    _filamentButton->setAlternateBackgroundColor(Application.getTheme()->getPrimaryColor());
    _filamentButton->setTextColor(Application.getTheme()->getSecondaryColor2());
    _filamentButton->setBorderWidth(0);
    addView(_filamentButton);

    _settingsButton= new LabelButton("SETTINGS",Rect(160,120,159,120));
    _settingsButton->setBorderColor(Application.getTheme()->getSecondaryColor2());
    _settingsButton->setBackgroundColor(Application.getTheme()->getBackgroundColor());
    _settingsButton->setAlternateBackgroundColor(Application.getTheme()->getPrimaryColor());
    _settingsButton->setTextColor(Application.getTheme()->getSecondaryColor2());
    _settingsButton->setBorderWidth(0);
    _settingsButton->setDelegate(this);
    addView(_settingsButton);
    */
}

MainSceneController::~MainSceneController()
{

}


uint16_t MainSceneController::getBackgroundColor()
{
    return Application.getTheme()->getBackgroundColor();
}

void MainSceneController::display()
{

}

String MainSceneController::getName()
{
    return "MainSceneController";
}

void MainSceneController::loop()
{
    SceneController::loop();
}

void MainSceneController::setup()
{
    SceneController::setup();

    Display.setScrollInsets(0,0);
}


void MainSceneController::onWillAppear()
{
    SceneController::onWillAppear();

    Display.setScrollInsets(0,0);
}

#pragma mark ButtonDelegate Implementation

void MainSceneController::buttonPressed(void *button)
{
    LOG("MainSceneController::buttonPressed");

    if (button == _printButton)
    {
        ChoosePrintSceneController* choosePrintSceneController = new ChoosePrintSceneController();
        Application.pushScene(choosePrintSceneController);
    }
    else if (button == _settingsButton)
    {
        VirtualKeyboardSceneController* scene = new VirtualKeyboardSceneController();
        Application.pushScene(scene);
    }
    else if (button == _hotendButton)
    {
        MachineControlSceneController* scene = new MachineControlSceneController();
        Application.pushScene(scene);
    }
    else
    {
        MainSceneController* scene = new MainSceneController();
        Application.pushScene(scene);
    }
}
