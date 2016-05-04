//
// Created by Phillip Schuster on 30.01.16.
//

#ifndef TEENSYCMAKE_CHOOSEPRINTSCENECONTROLLER_H
#define TEENSYCMAKE_CHOOSEPRINTSCENECONTROLLER_H

#include "SceneController.h"
#include "BitmapButton.h"
#include "LabelView.h"
#include "LabelButton.h"
#include "ModelView.h"

class ChoosePrintSceneController: public SceneController, ButtonDelegate
{
public:
    virtual void onWillAppear() override;

    ChoosePrintSceneController();
    virtual ~ChoosePrintSceneController();

    String getName();

    virtual void display();
    virtual void loop();

protected:
    ModelView* _modelView;

private:
    virtual void buttonPressed(void *button);

public:
    virtual uint16_t getBackgroundColor() override;

    virtual void handleTouchUp(TS_Point &point) override;
};

#endif //TEENSYCMAKE_CHOOSEPRINTSCENECONTROLLER_H
