//
// Created by Phillip Schuster on 09.05.16.
//

#ifndef TEENSY_CONFIRMSCENECONTROLLER_H
#define TEENSY_CONFIRMSCENECONTROLLER_H


#include "SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/LabelButton.h"

class ConfirmSceneController: public SidebarSceneController, public ButtonDelegate
{
#pragma mark Constructor
public:
    ConfirmSceneController();
    virtual ~ConfirmSceneController();

#pragma mark Sidebar Scene Controller
private:
    virtual const uint8_t *getSidebarIcon() override;
    virtual String getSidebarTitle() const override;

#pragma mark Scene Controller
private:
    String getName();
    virtual void onWillAppear() override;
    virtual uint16_t getBackgroundColor() override;

#pragma mark Button Delegate
private:
    virtual void buttonPressed(void *button) override;

#pragma mark Member Variables
private:
    LabelButton* _yesButton;
    LabelButton* _noButton;
};



#endif //TEENSY_CONFIRMSCENECONTROLLER_H
