//
// Created by Phillip Schuster on 11.05.16.
//

#ifndef TEENSY_PRINTSTATUSSCENECONTROLLER_H
#define TEENSY_PRINTSTATUSSCENECONTROLLER_H


#include "SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/LabelButton.h"
#include "framework/views/ProgressBar.h"
#include "framework/layers/SDBitmapLayer.h"

class PrintStatusSceneController: public SidebarSceneController
{
#pragma mark Constructor
public:
    virtual void loop() override;

    PrintStatusSceneController();
    virtual ~PrintStatusSceneController();

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
    LabelButton* _button;
    ProgressBar* _progressBar;
    SDBitmapLayer* _imageLayer;
    TextLayer* _nameLayer;
    float _step;
};

#endif //TEENSY_PRINTSTATUSSCENECONTROLLER_H
