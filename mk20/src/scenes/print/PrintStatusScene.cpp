#include "PrintStatusScene.h"

#include "framework/views/ProgressBar.h"
//#include "layers/SDBitmapLayer.h"
//#include "layers/TransparentTextLayer.h"
//#include "PausePrintSceneController.h"
#include "Printr.h"
#include "UIBitmaps.h"
//#include "FinishPrintSceneController.h"
//#include "core/Application.h"

#include "scenes/print/PrintStatusScene.h"

extern UIBitmaps uiBitmaps;
extern Printr printr;

PrintStatusScene::PrintStatusScene(String jobFilePath, Project project, Job job, uint16_t jobOffset):
  SidebarSceneController::SidebarSceneController(),
  _jobFilePath(jobFilePath),
  _project(project),
  _job(job),
  _jobOffset(jobOffset){
  printr.setListener(this);
}

PrintStatusScene::~PrintStatusScene() {
}

UIBitmap * PrintStatusScene::getSidebarBitmap() {
  return &uiBitmaps.sidebar_jobs;
}


UIBitmap * PrintStatusScene::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

String PrintStatusScene::getName() {
  return "PrintStatusScene";
}

void PrintStatusScene::onWillAppear()
{
  /*
    _imageLayer = new SDBitmapLayer(Rect(0,0,270,240));
    _imageLayer->setBitmap(Printr::project.image_file,270,240);
    Display.setFixedBackgroundLayer(_imageLayer);

    _nameLayer = new TransparentTextLayer(Rect(15,10,Display.getLayoutWidth()-30,25));
    _nameLayer->setTextAlign(TEXTALIGN_LEFT);
    _nameLayer->setFont(&PTSansNarrow_24);
    _nameLayer->setText(Printer::project.title);
    _nameLayer->setForegroundColor(Application.getTheme()->getColor(OverlayTextColor));
    Display.addLayer(_nameLayer);
*/
    _progressBar = new ProgressBar(Rect(15,215,Display.getLayoutWidth()-30,7));
    _progressBar->setValue(0.0f);
    addView(_progressBar);


    printr.startJob(_jobFilePath);

/*
    _pLayer = new TransparentTextLayer(Rect(12,190,Display.getLayoutWidth()-30,20));
    _pLayer->setTextAlign(TEXTALIGN_LEFT);
    _pLayer->setFont(&PTSansNarrow_16);
    _pLayer->setText("Printing...");
    _pLayer->setForegroundColor(Application.getTheme()->getColor(OverlayTextColor));
    Display.addLayer(_pLayer);
*/
    SidebarSceneController::onWillAppear();
}

void PrintStatusScene::printrCallback(const char ctype[], float * data) {
  if (strcmp(ctype,"end") == 0) {
    printr.setListener(nullptr);
    ProjectsScene * scene = new ProjectsScene();
    Application.pushScene(scene);
  }
}


void PrintStatusScene::loop()
{
    //Handle print process
    //Printer::process();

    //Check if we are done
    /*
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
    */

    SceneController::loop();
}



void PrintStatusScene::buttonPressed(void *button) {
  //PausePrintSceneController * scene = new PausePrintSceneController();
  //Application.pushScene(scene);
}
