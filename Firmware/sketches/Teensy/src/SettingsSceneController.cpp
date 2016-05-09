//
// Created by Phillip Schuster on 09.05.16.
//

#include "SettingsSceneController.h"

#include "Bitmaps.h"
#include "CleanPlasticSceneController.h"
#include "ConfirmSceneController.h"

SettingsSceneController::SettingsSceneController():
        SidebarSceneController::SidebarSceneController()
{

}

SettingsSceneController::~SettingsSceneController()
{

}

uint16_t SettingsSceneController::getBackgroundColor()
{
    return Application.getTheme()->getColor(BackgroundColor);
}

String SettingsSceneController::getName()
{
    return "SettingsSceneController";
}


String SettingsSceneController::getSidebarTitle() const
{
    return String("SETTINGS");
}


const uint8_t *SettingsSceneController::getSidebarIcon()
{
    return imageOfCancelIcon_24_24;
}

void SettingsSceneController::setupButton(LabelButton* button)
{
    button->setDelegate(this);
    button->setTextAlign(TEXTALIGN_LEFT);
    button->setFont(&PTSansNarrow_16);
    button->setBackgroundColor(getBackgroundColor());
    button->setAlternateBackgroundColor(Application.getTheme()->getColor(ControlBackgroundColor));
    addView(button);
}

void SettingsSceneController::onWillAppear()
{
    _wifiSettingsButton = new LabelButton("WIFI SETTINGS",Rect(15+50,10,320-50-15-15,40));
    _wifiSettingsButton->setName("wifi");
    _wifiSettingsButton->setIcon(imageOfWifiIcon_32_30,Application.getTheme()->getColor(HighlightBackgroundColor),32,30);
    setupButton(_wifiSettingsButton);

    _calibrateHotendButton = new LabelButton("CALIBRATE HOTEND",Rect(15+50,70,320-50-15-15,40));
    _calibrateHotendButton->setName("hotend");
    _calibrateHotendButton->setIcon(imageOfCloudIcon_32_30,Application.getTheme()->getColor(HighlightBackgroundColor),32,30);
    setupButton(_calibrateHotendButton);

    _cloudSetupButton = new LabelButton("CLOUD SETUP",Rect(15+50,130,320-50-15-15,40));
    _cloudSetupButton->setName("cloud");
    _cloudSetupButton->setIcon(imageOfHotendIcon_32_30,Application.getTheme()->getColor(HighlightBackgroundColor),32,30);
    setupButton(_cloudSetupButton);

    _changeFilamentButton = new LabelButton("CHANGE FILAMENT",Rect(15+50,190,320-50-15-15,40));
    _changeFilamentButton->setName("filament");
    _changeFilamentButton->setIcon(imageOfResumeIcon_32_30,Application.getTheme()->getColor(HighlightBackgroundColor),32,30);
    setupButton(_changeFilamentButton);

    SidebarSceneController::onWillAppear();
}

#pragma mark ButtonDelegate Implementation

void SettingsSceneController::buttonPressed(void *button)
{
    if (button == _wifiSettingsButton)
    {
        ConfirmSceneController * scene = new ConfirmSceneController();
        Application.pushScene(scene);
    }
}

