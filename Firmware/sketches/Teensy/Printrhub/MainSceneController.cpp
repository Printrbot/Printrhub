//
// Created by Phillip Schuster on 20.01.16.
//

#include "MainSceneController.h"
#include "Bitmaps.h"

MainSceneController::MainSceneController():
SceneController::SceneController()
{
    _printButton = new BitmapButton(Rect(23,90,80,80));
    _printButton->setBitmap(printerButton,80,80);

    _filamentButton = new BitmapButton(Rect(120,90,80,80));
    _filamentButton->setBitmap(filamentButton,80,80);

    _settingsButton = new BitmapButton(Rect(215,90,80,80));
    _settingsButton->setBitmap(settingsButton,80,80);
}

MainSceneController::~MainSceneController()
{
    delete _printButton;
    delete _filamentButton;
    delete _settingsButton;
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
    if (_transition)
    {
        _printButton->setFrame(Rect(23,90+_offset,80,80));
        _filamentButton->setFrame(Rect(120,90+(_offset/2),80,80));
        _settingsButton->setFrame(Rect(215,90+(_offset/4),80,80));
        _offset += _velocity;

        if (_offset > 50 || _offset < -50)
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
/*        else
        {
            _printButton->setFrame(Rect(23,90,80,80));
        }*/

        _transition = true;
    }
    else
    {
       // _printButton->setFrame(Rect(23,89,80,80));
    }
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
}
