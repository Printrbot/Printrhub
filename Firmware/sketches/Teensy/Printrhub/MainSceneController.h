//
// Created by Phillip Schuster on 20.01.16.
//

#ifndef TEENSYCMAKE_MAINSCENECONTROLLER_H
#define TEENSYCMAKE_MAINSCENECONTROLLER_H

#include "SceneController.h"
#include "BitmapButton.h"

class MainSceneController: public SceneController, ButtonDelegate
{
public:
    MainSceneController();
    virtual ~MainSceneController();

    String getName();

    virtual void display();
    virtual void loop();

protected:
    BitmapButton* _printButton;
    BitmapButton* _filamentButton;
    BitmapButton* _settingsButton;
    float _offset = 0;
    float _velocity = 4;
    bool _transition = false;

public:
    virtual void setup() override;

    virtual void onWillAppear() override;

private:
    virtual void buttonPressed(BitmapButton *button);
};


#endif //TEENSYCMAKE_MAINSCENECONTROLLER_H
