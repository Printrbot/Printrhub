//
// Created by Phillip Schuster on 16.01.16.
//

#include "IdleSceneController.h"

IdleSceneController::IdleSceneController():
_view(IdleView(Rect(0,0,320,240))),
_size(10)
{
    _velocity = 1;
}

String IdleSceneController::getName()
{
    return "idleSceneController";
}

void IdleSceneController::display()
{
    _view.display();
}

void IdleSceneController::setup()
{

}


void IdleSceneController::onWillAppear()
{
    SceneController::onWillAppear();

    RectangleLayer* layer = _view.getLayer();
    layer->setFrame(Rect(50,50,20,20));

    _view.getLayer2()->setFrame(Rect(120,120,30,30));


}

void IdleSceneController::loop()
{
    _view.getLayer()->setFrame(Rect(50+_size,50+_size,20,20));
    _view.getLayer2()->setFrame(Rect(120-_size,120+_size*2,30+_size,30+_size));
    _size += _velocity;
    if (_size > 50 || _size < -30)
    {
        _velocity = -_velocity;
    }

}