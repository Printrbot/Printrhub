//
// Created by Phillip Schuster on 09.05.16.
//

#ifndef TEENSY_SETTINGSSCENECONTROLLER_H
#define TEENSY_SETTINGSSCENECONTROLLER_H

#include "SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/LabelButton.h"

class SettingsSceneController: public SidebarSceneController
{
#pragma mark Constructor
public:
	virtual void loop() override;

	virtual void onSidebarButtonTouchUp() override;

	SettingsSceneController();
    virtual ~SettingsSceneController();

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


#pragma mark Member Functions
private:
    void setupButton(LabelButton* button);

#pragma mark Member Variables
private:
    LabelButton* _wifiSettingsButton;
    LabelButton* _calibrateHotendButton;
    LabelButton* _cloudSetupButton;
    LabelButton* _changeFilamentButton;
    LabelButton* _aboutButton;
    LabelButton* _themeButton;
};

#endif //TEENSY_SETTINGSSCENECONTROLLER_H
