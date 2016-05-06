//
// Created by Phillip Schuster on 06.05.16.
//

#ifndef TEENSY_CLEANPLASTICSCENECONTROLLER_H
#define TEENSY_CLEANPLASTICSCENECONTROLLER_H

#include "SidebarSceneController.h"
#include "BitmapButton.h"
#include "LabelButton.h"

class CleanPlasticSceneController: public SidebarSceneController, public ButtonDelegate
{
public:
    CleanPlasticSceneController();
    virtual ~CleanPlasticSceneController();

private:
    virtual const uint8_t *getSidebarIcon() override;
    virtual String getSidebarTitle() const override;

    String getName();

    virtual void onWillAppear() override;
    void buttonPressed(void *button);

protected:
    virtual uint16_t getBackgroundColor() override;

private:
    LabelButton* _button;
};


#endif //TEENSY_CLEANPLASTICSCENECONTROLLER_H
