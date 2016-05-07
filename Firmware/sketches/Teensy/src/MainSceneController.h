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
#pragma mark Constructor
public:
    MainSceneController();
    virtual ~MainSceneController();

#pragma mark Scene Controller
private:
    virtual void onWillAppear() override;
    String getName();
    virtual void display();
    virtual void loop();
    virtual uint16_t getBackgroundColor() override;

#pragma mark Button Delegate
private:
    virtual void buttonPressed(void *button);

#pragma mark Member Variables
protected:
    LabelButton* _printButton;
    LabelButton* _filamentButton;
    LabelButton* _settingsButton;
    LabelButton* _hotendButton;
    NavBar* _navBar;
};


#endif //TEENSYCMAKE_MAINSCENECONTROLLER_H
