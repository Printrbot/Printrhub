/*

//
// Created by Phillip Schuster on 11.05.16.
//

#include "PrintStatusScene.h"

#include "framework/views/ProgressBar.h"
#include "framework/layers/SDBitmapLayer.h"
#include "framework/layers/TransparentTextLayer.h"
//#include "PausePrintSceneController.h"
#include "Printr.h"
//#include "FinishPrintSceneController.h"
#include "framework/core/Application.h"

PrintStatusScene::PrintStatusScene():
        SidebarSceneController::SidebarSceneController()
{

}

PrintStatusScene::~PrintStatusScene()
{

}

uint16_t PrintStatusScene::getBackgroundColor()
{
    return Application.getTheme()->getColor(BackgroundColor);
}

String PrintStatusScene::getName()
{
    return "PrintStatusScene";
}

String PrintStatusScene::getSidebarTitle() const
{
    return String("PRINTING");
}

const uint8_t *PrintStatusScene::getSidebarIcon()
{
    return imageOfPauseIcon_24_24;
}

void PrintStatusScene::onWillAppear()
{
    _imageLayer = new SDBitmapLayer(Rect(0,0,270,240));
    _imageLayer->setBitmap(Printer::project.image_file,270,240);
    Display.setFixedBackgroundLayer(_imageLayer);

    _nameLayer = new TransparentTextLayer(Rect(15,10,Display.getLayoutWidth()-30,25));
    _nameLayer->setTextAlign(TEXTALIGN_LEFT);
    _nameLayer->setFont(&PTSansNarrow_24);
    _nameLayer->setText(Printer::project.title);
    _nameLayer->setForegroundColor(Application.getTheme()->getColor(OverlayTextColor));
    Display.addLayer(_nameLayer);

    _progressBar = new ProgressBar(Rect(15,215,Display.getLayoutWidth()-30,7));
    _progressBar->setValue(0.0f);
    addView(_progressBar);

    _pLayer = new TransparentTextLayer(Rect(12,190,Display.getLayoutWidth()-30,20));
    _pLayer->setTextAlign(TEXTALIGN_LEFT);
    _pLayer->setFont(&PTSansNarrow_16);
    _pLayer->setText("Printing...");
    _pLayer->setForegroundColor(Application.getTheme()->getColor(OverlayTextColor));
    Display.addLayer(_pLayer);

    SidebarSceneController::onWillAppear();
}


void PrintStatusScene::loop()
{
    //Handle print process
    //Printer::process();

    //Check if we are done
    if (!Printer::printing)
    {
        //ProjectsSceneController * scene = new ProjectsSceneController();
        //Application.pushScene(scene);
        FinishPrintSceneController * scene = new FinishPrintSceneController();
        Application.pushScene(scene);
    } else if (_progressBar != NULL)
    {
        _progressBar->setValue(Printer::progress);
    }

    SceneController::loop();
}



#pragma mark ButtonDelegate Implementation

void PrintStatusScene::buttonPressed(void *button)
{
  PausePrintSceneController * scene = new PausePrintSceneController();
  Application.pushScene(scene);
}
*/
