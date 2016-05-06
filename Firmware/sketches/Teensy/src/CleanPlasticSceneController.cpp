//
// Created by Phillip Schuster on 06.05.16.
//

#include "CleanPlasticSceneController.h"
#include "LoadFilamentSceneController.h"
#include "Bitmaps.h"

CleanPlasticSceneController::CleanPlasticSceneController():
        SidebarSceneController::SidebarSceneController()
{

}

CleanPlasticSceneController::~CleanPlasticSceneController()
{

}

uint16_t CleanPlasticSceneController::getBackgroundColor()
{
    return Application.getTheme()->getColor(BackgroundColor);
}

String CleanPlasticSceneController::getName()
{
    return "CleanPlasticSceneController";
}


String CleanPlasticSceneController::getSidebarTitle() const
{
    return String("FILAMENT");
}


const uint8_t *CleanPlasticSceneController::getSidebarIcon()
{
    return imageOfCancelIcon_24_24;
}

void CleanPlasticSceneController::onWillAppear()
{
    BitmapLayer* iconLayer = new BitmapLayer(Rect(102+50,30,66,58));
    iconLayer->setBitmap(imageOfWarningIcon_66_58,66,58);
    iconLayer->setBackgroundColor(getBackgroundColor());
    iconLayer->setColor(Application.getTheme()->getColor(WarningColor));
    Display.addLayer(iconLayer);

    TextLayer* textLayer = new TextLayer(Rect(10+50,95,320-20-50,22));
    textLayer->setFont(&PTSansNarrow_18);
    textLayer->setTextAlign(TEXTALIGN_CENTERED);
    textLayer->setText("Clean plastic after test");
    Display.addLayer(textLayer);

    textLayer = new TextLayer(Rect(10+50,117,320-20-50,22));
    textLayer->setFont(&PTSansNarrow_18);
    textLayer->setTextAlign(TEXTALIGN_CENTERED);
    textLayer->setText("extrusion");
    Display.addLayer(textLayer);

    _button = new LabelButton("DONE",Rect(15+50,160,320-30-50,68));
    _button->setName("DONE");
    _button->setDelegate(this);
    addView(_button);

    SidebarSceneController::onWillAppear();
}

#pragma mark ButtonDelegate Implementation

void CleanPlasticSceneController::buttonPressed(void *button)
{
    if (button == _button)
    {
        LoadFilamentSceneController* scene = new LoadFilamentSceneController();
        Application.pushScene(scene);
    }
}

