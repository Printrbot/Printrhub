//
// Created by Phillip Schuster on 16.01.16.
//

#ifndef TEENSYCMAKE_IDLESCENECONTROLLER_H
#define TEENSYCMAKE_IDLESCENECONTROLLER_H

#include "SceneController.h"
#include "IdleView.h"
#include "Arduino.h"

class IdleSceneController: public SceneController
{
public:
    IdleSceneController();

    String getName();

    virtual void display();
    virtual void loop();

protected:
    IdleView _view;
    uint8_t _size;
    uint8_t _velocity;
public:

    virtual void onWillAppear() override;
};


#endif //TEENSYCMAKE_IDLESCENECONTROLLER_H
