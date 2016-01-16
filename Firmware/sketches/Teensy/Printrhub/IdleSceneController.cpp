//
// Created by Phillip Schuster on 16.01.16.
//

#include "IdleSceneController.h"

IdleSceneController::IdleSceneController():
_view(IdleView(Rect(0,0,100,100))),
_size(10)
{

}

String IdleSceneController::getName()
{
    return "idleSceneController";
}

void IdleSceneController::display()
{
    _view.display();
}

void IdleSceneController::loop()
{
    RectangleLayer* layer = _view.getLayer();
    layer->setFrame(Rect(0,0,_size,_size));

    _size += 1;
}