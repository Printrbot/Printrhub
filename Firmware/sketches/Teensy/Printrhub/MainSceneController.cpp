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
    _printButton->display();
    _filamentButton->display();
    _settingsButton->display();
}

String MainSceneController::getName()
{
    return "MainSceneController";
}

void MainSceneController::loop()
{
    SceneController::loop();

    if (Touch.touched())
    {
        //Y = X;

        TS_Point point = Touch.getPoint();
        swap(point.x,point.y);

        point.y = 240-point.y;

        LOG("Touched");
        LOG_VALUE("X",point.x);
        LOG_VALUE("Y",point.y);

        //if (_printButton->getFrame().containsPoint(point.x,point.y))
        {
            _printButton->setFrame(Rect(23,100,80,80));
        }
/*        else
        {
            _printButton->setFrame(Rect(23,90,80,80));
        }*/
    }
}

void MainSceneController::setup()
{
    SceneController::setup();
}

void MainSceneController::onWillAppear()
{
    SceneController::onWillAppear();
}
