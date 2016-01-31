//
// Created by Phillip Schuster on 20.01.16.
//

#include "MainSceneController.h"
#include "Bitmaps.h"
#include "LabelButton.h"
#include "Application.h"
#include "ChoosePrintSceneController.h"
#include "WiFiSetupSceneController.h"
#include "VirtualKeyboardSceneController.h"

MainSceneController::MainSceneController():
SceneController::SceneController()
{
    _printButton = new LabelButton("PRINT",Rect(0,0,159,119));
    _printButton->setBorderColor(Application.getTheme()->getSecondaryColor2());
    _printButton->setBackgroundColor(Application.getTheme()->getBackgroundColor());
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
}

MainSceneController::~MainSceneController()
{

}


uint16_t MainSceneController::getBackgroundColor()
{
    return Application.getTheme()->getSecondaryColor2();
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

/*    _printButton->setFrame(Rect(23,90+_offset,80,80));
    _filamentButton->setFrame(Rect(120,90+(_offset/2),80,80));
    _settingsButton->setFrame(Rect(215,90+(_offset/4),80,80));
    _offset += _velocity;

    if (_offset > 80 || _offset < -80)
    {
        _velocity = -_velocity;
    }*/

 /*   if (_transition)
    {
        _printButton->setFrame(Rect(23+_offset,90+_offset,80,80));
        _filamentButton->setFrame(Rect(120,90+(_offset/2),80,80));
        _settingsButton->setFrame(Rect(215,90+(_offset/4),80,80));
        _offset += _velocity;

        if (_offset > 200 || _offset < -200)
        {
            _velocity = -_velocity;
        }

        return;
    }

    if (Touch.touched())
    {
        //Y = X;

        TS_Point point = Touch.getPoint();
        swap(point.x,point.y);

        point.y = 240-point.y;


        //if (_printButton->getFrame().containsPoint(point.x,point.y))
        {
            //_printButton->setFrame(Rect(23,93,80,80));
        }
//        else
//        {
//            _printButton->setFrame(Rect(23,90,80,80));
//        }

        _transition = true;
    }
    else
    {
       // _printButton->setFrame(Rect(23,89,80,80));
    }*/
}

void MainSceneController::setup()
{
    SceneController::setup();
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
}

