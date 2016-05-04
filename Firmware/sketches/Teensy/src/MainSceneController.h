//
// Created by Phillip Schuster on 20.01.16.
//

#ifndef TEENSYCMAKE_MAINSCENECONTROLLER_H
#define TEENSYCMAKE_MAINSCENECONTROLLER_H

#include "SceneController.h"
#include "BitmapButton.h"
#include "LabelView.h"
#include "LabelButton.h"
#include "NavBar.h"

class MainSceneController: public SceneController, ButtonDelegate
{
public:
    virtual void onWillAppear() override;

    MainSceneController();
    virtual ~MainSceneController();

    String getName();

    virtual void display();
    virtual void loop();

protected:
    LabelButton* _printButton;
    LabelButton* _filamentButton;
    LabelButton* _settingsButton;
    LabelButton* _hotendButton;
    NavBar* _navBar;

public:
    virtual void setup() override;

private:
    virtual void buttonPressed(void *button);

public:
    virtual uint16_t getBackgroundColor() override;
};


#endif //TEENSYCMAKE_MAINSCENECONTROLLER_H
