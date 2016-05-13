//
// Created by Phillip Schuster on 06.05.16.
//

#ifndef TEENSY_CLEANPLASTICSCENECONTROLLER_H
#define TEENSY_CLEANPLASTICSCENECONTROLLER_H

#include "SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/LabelButton.h"

class CleanPlasticSceneController: public SidebarSceneController
{
#pragma mark Constructor
public:
    CleanPlasticSceneController();
    virtual ~CleanPlasticSceneController();

#pragma mark Sidebar Scene Controller
private:
    virtual const uint8_t *getSidebarIcon() override;
    virtual String getSidebarTitle() const override;

#pragma mark Scene Controller
private:
    String getName() override;
    virtual uint16_t getBackgroundColor() override;
    virtual void onWillAppear() override;

#pragma mark Button Delegate
private:
    void buttonPressed(void *button);

#pragma mark Member Variables
protected:
    LabelButton* _button;
};


#endif //TEENSY_CLEANPLASTICSCENECONTROLLER_H
