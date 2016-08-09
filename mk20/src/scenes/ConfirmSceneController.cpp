//
// Created by Phillip Schuster on 09.05.16.
//

#include "ConfirmSceneController.h"

#include "Bitmaps.h"
#include "print/CleanPlasticSceneController.h"
#include "print/PausePrintSceneController.h"

ConfirmSceneController::ConfirmSceneController():
        SidebarSceneController::SidebarSceneController()
{

}

ConfirmSceneController::~ConfirmSceneController()
{

}

uint16_t ConfirmSceneController::getBackgroundColor()
{
    return Application.getTheme()->getColor(BackgroundColor);
}

String ConfirmSceneController::getName()
{
    return "ConfirmSceneController";
}


String ConfirmSceneController::getSidebarTitle() const
{
    return String("");
}


const uint8_t *ConfirmSceneController::getSidebarIcon()
{
    return imageOfCancelIcon_24_24;
}

void ConfirmSceneController::onWillAppear()
{
    BitmapLayer* iconLayer = new BitmapLayer(Rect(102,30,66,58));
    iconLayer->setBitmap(imageOfWarningIcon_66_58,66,58);
    iconLayer->setBackgroundColor(getBackgroundColor());
    iconLayer->setColor(Application.getTheme()->getColor(WarningColor));
    Display.addLayer(iconLayer);

    TextLayer* textLayer = new TextLayer(Rect(10,95,Display.getLayoutWidth()-20,20));
    textLayer->setFont(&PTSansNarrow_18);
    textLayer->setTextAlign(TEXTALIGN_CENTERED);
    textLayer->setText("Are you sure");
    Display.addLayer(textLayer);

    uint16_t buttonWidth = ((Display.getLayoutWidth()-30)-15)/2;  //15 gap between buttons

    _yesButton = new LabelButton("YES",Rect(15,160,buttonWidth,68));
    _yesButton->setName("YES");
    _yesButton->setDelegate(this);
    _yesButton->setIcon(imageOfAcceptIcon_32_30,Application.getTheme()->getColor(SuccessColor),32,30);
    addView(_yesButton);

    _noButton = new LabelButton("NO",Rect(15+buttonWidth+15,160,buttonWidth,68));
    _noButton->setName("NO");
    _noButton->setDelegate(this);
    _noButton->setIcon(imageOfCancelIcon_32_30,Application.getTheme()->getColor(AlertColor),32,30);
    addView(_noButton);

    SidebarSceneController::onWillAppear();
}

#pragma mark ButtonDelegate Implementation

void ConfirmSceneController::buttonPressed(void *button)
{
    if (button == _yesButton)
    {
        CleanPlasticSceneController* scene = new CleanPlasticSceneController();
        Application.pushScene(scene);
    }
    else if (button == _noButton)
    {
        PausePrintSceneController* scene = new PausePrintSceneController();
        Application.pushScene(scene);
    }
}
