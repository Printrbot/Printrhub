//
// Created by Phillip Schuster on 11.05.16.
//

#include "PrintStatusSceneController.h"

#include "Bitmaps.h"
#include "CleanPlasticSceneController.h"
#include "ConfirmSceneController.h"
#include "framework/views/ProgressBar.h"
#include "framework/layers/SDBitmapLayer.h"
#include "framework/layers/TransparentTextLayer.h"
#include "PausePrintSceneController.h"

PrintStatusSceneController::PrintStatusSceneController():
        SidebarSceneController::SidebarSceneController()
{
    _step = 0.05f;
}

PrintStatusSceneController::~PrintStatusSceneController()
{

}

uint16_t PrintStatusSceneController::getBackgroundColor()
{
    return Application.getTheme()->getColor(BackgroundColor);
}

String PrintStatusSceneController::getName()
{
    return "PrintStatusSceneController";
}

String PrintStatusSceneController::getSidebarTitle() const
{
    return String("PRINTING");
}

const uint8_t *PrintStatusSceneController::getSidebarIcon()
{
    return imageOfPauseIcon_24_24;
}


void PrintStatusSceneController::onSidebarButtonTouchUp()
{
    PausePrintSceneController * scene = new PausePrintSceneController();
    Application.pushScene(scene);
}


void PrintStatusSceneController::onWillAppear()
{
    _imageLayer = new SDBitmapLayer(Rect(0,0,270,240));
    _imageLayer->setBitmap("printjob.dat",270,240);
    Display.setFixedBackgroundLayer(_imageLayer);

    _nameLayer = new TransparentTextLayer(Rect(15,10,Display.getLayoutWidth()-30,25));
    _nameLayer->setTextAlign(TEXTALIGN_LEFT);
    _nameLayer->setFont(&PTSansNarrow_24);
    _nameLayer->setText("Nice case");
    _nameLayer->setForegroundColor(Application.getTheme()->getColor(OverlayTextColor));
    Display.addLayer(_nameLayer);

    _progressBar = new ProgressBar(Rect(15,215,Display.getLayoutWidth()-30,7));
    _progressBar->setValue(0.0f);
    addView(_progressBar);

    SidebarSceneController::onWillAppear();
}


void PrintStatusSceneController::loop()
{
    if (_progressBar != NULL)
    {
        _progressBar->setValue(_progressBar->getValue()+_step);
        if (_progressBar->getValue() >= 1)
        {
            _step = -_step;
        }
        else if (_progressBar->getValue() <= 0)
        {
            _step = -_step;
        }
    }

    SceneController::loop();
}


#pragma mark ButtonDelegate Implementation

void PrintStatusSceneController::buttonPressed(void *button)
{
    if (button == _button)
    {
        ConfirmSceneController * scene = new ConfirmSceneController();
        Application.pushScene(scene);
    }

    SidebarSceneController::buttonPressed(button);
}

