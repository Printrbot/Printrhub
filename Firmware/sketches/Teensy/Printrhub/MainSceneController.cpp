//
// Created by Phillip Schuster on 20.01.16.
//

#include "MainSceneController.h"
#include "Bitmaps.h"

MainSceneController::MainSceneController():
SceneController::SceneController()
{
    _printButton = new LabelView("Print",Rect(0,0,160,119));
    addView(_printButton);

    _hotendButton = new LabelView("Hotend",Rect(160,0,160,120));
    addView(_hotendButton);

    _filamentButton = new LabelView("Filament",Rect(0,120,160,120));
    addView(_filamentButton);

    _settingsButton= new LabelView("Settings",Rect(160,120,160,120));
    addView(_settingsButton);
}

MainSceneController::~MainSceneController()
{

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

void MainSceneController::onWillAppear()
{
    SceneController::onWillAppear();

    _printButton->display();
    _filamentButton->display();
    _settingsButton->display();
    _hotendButton->display();
}

#pragma mark ButtonDelegate Implementation

void MainSceneController::buttonPressed(BitmapButton *button)
{
    LOG("MainSceneController::buttonPressed");

}
