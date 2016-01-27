//
// Created by Phillip Schuster on 28.01.16.
//

#ifndef TEENSYCMAKE_WIFISETUPSCENECONTROLLER_H
#define TEENSYCMAKE_WIFISETUPSCENECONTROLLER_H


#include "SceneController.h"
#include "BitmapButton.h"
#include "LabelView.h"

class WiFiSetupSceneController: public SceneController, ButtonDelegate
{
public:
    WiFiSetupSceneController();
    virtual ~WiFiSetupSceneController();

    String getName();

    virtual void display();
    virtual void loop();

protected:
    StackArray<LabelView*> _labelViews;
    float _offset = 0;
    float _velocity = 4;
    bool _transition = false;

public:
    virtual void setup() override;

    virtual void onWillAppear() override;

private:
    virtual void buttonPressed(BitmapButton *button);
};

#endif //TEENSYCMAKE_WIFISETUPSCENECONTROLLER_H
