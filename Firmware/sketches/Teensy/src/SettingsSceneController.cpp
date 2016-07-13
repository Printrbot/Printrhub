//
// Created by Phillip Schuster on 09.05.16.
//

#include "SettingsSceneController.h"

#include "Bitmaps.h"
#include "CleanPlasticSceneController.h"
#include "ConfirmSceneController.h"
#include "ChoosePrintSceneController.h"

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
    setScrollSnap(Display.getLayoutWidth(), SnapMode::Flick);

    _wifiSettingsButton = new LabelButton("WIFI SETTINGS",Rect(15,10,Display.getLayoutWidth()-15-15,40));
    _wifiSettingsButton->setName("wifi");
    _wifiSettingsButton->setIcon(imageOfWifiIcon_32_30,Application.getTheme()->getColor(HighlightBackgroundColor),32,30);
    setupButton(_wifiSettingsButton);

    _calibrateHotendButton = new LabelButton("CALIBRATE HOTEND",Rect(15,70,Display.getLayoutWidth()-15-15,40));
    _calibrateHotendButton->setName("hotend");
    _calibrateHotendButton->setIcon(imageOfCloudIcon_32_30,Application.getTheme()->getColor(HighlightBackgroundColor),32,30);
    setupButton(_calibrateHotendButton);

    _cloudSetupButton = new LabelButton("CLOUD SETUP",Rect(15,130,Display.getLayoutWidth()-15-15,40));
    _cloudSetupButton->setName("cloud");
    _cloudSetupButton->setIcon(imageOfHotendIcon_32_30,Application.getTheme()->getColor(HighlightBackgroundColor),32,30);
    setupButton(_cloudSetupButton);

    _changeFilamentButton = new LabelButton("CHANGE FILAMENT",Rect(15,190,Display.getLayoutWidth()-15-15,40));
    _changeFilamentButton->setName("filament");
    _changeFilamentButton->setIcon(imageOfResumeIcon_32_30,Application.getTheme()->getColor(HighlightBackgroundColor),32,30);
    setupButton(_changeFilamentButton);

    _aboutButton = new LabelButton("ABOUT",Rect(15+Display.getLayoutWidth(),10,Display.getLayoutWidth()-15-15,40));
    _aboutButton->setName("about");
    _aboutButton->setIcon(imageOfCardIcon_32_30,Application.getTheme()->getColor(HighlightBackgroundColor),32,30);
    setupButton(_aboutButton);

    _themeButton = new LabelButton("THEME",Rect(15+Display.getLayoutWidth(),70,Display.getLayoutWidth()-15-15,40));
    _themeButton->setName("theme");
    _themeButton->setIcon(imageOfGaugeIcon_32_30,Application.getTheme()->getColor(HighlightBackgroundColor),32,30);
    setupButton(_themeButton);

    //Add a small rect that makes sure we span two pages
    RectangleLayer* spacer = new RectangleLayer(Rect(0,239,Display.getLayoutWidth()*2,1));
    spacer->setBackgroundColor(getBackgroundColor());
    Display.addLayer(spacer);

    SidebarSceneController::onWillAppear();
}


void SettingsSceneController::onSidebarButtonTouchUp()
{
    ChoosePrintSceneController * scene = new ChoosePrintSceneController();
    Application.pushScene(scene);
}

#pragma mark ButtonDelegate Implementation

void SettingsSceneController::buttonPressed(void *button)
{
    if (button == _wifiSettingsButton)
    {
        ConfirmSceneController * scene = new ConfirmSceneController();
        Application.pushScene(scene);
    }
    else if (button == _calibrateHotendButton)
    {
        Display.setCursor(10,10);
        Display.println("Talk to ESP ...");

        LOG("Request datetime from ESP8266");
        Application.getESPStack()->requestTask(GetTimeAndDate);
        LOG("Datetime request sent");
    }

    SidebarSceneController::buttonPressed(button);
}


void SettingsSceneController::loop()
{
    SceneController::loop();
}

