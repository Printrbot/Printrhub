//
// Created by Phillip Schuster on 28.01.16.
//

#ifndef TEENSYCMAKE_WIFISETUPSCENECONTROLLER_H
#define TEENSYCMAKE_WIFISETUPSCENECONTROLLER_H


#include "framework/core/SceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/LabelView.h"

class WiFiSetupSceneController: public SceneController, ButtonDelegate
{
public:
    WiFiSetupSceneController();
    virtual ~WiFiSetupSceneController();

    String getName();

    virtual void display();

protected:
    StackArray<LabelView*> _labelViews;
    float _offset = 0;
    float _velocity = 4;
    bool _transition = false;

    virtual void onWillAppear() override;

private:
    virtual void buttonPressed(void *button);
};

#endif //TEENSYCMAKE_WIFISETUPSCENECONTROLLER_H
