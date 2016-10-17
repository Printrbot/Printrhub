#include "PrintStatusScene.h"
#include "framework/views/ProgressBar.h"
#include "ConfirmCancelPrint.h"
#include "Printr.h"
#include "UIBitmaps.h"
#include "framework/layers/TransparentTextLayer.h"
#include "font_LiberationSans.h"
#include "FinishPrint.h";

#include "scenes/print/PrintStatusScene.h"

extern UIBitmaps uiBitmaps;
extern Printr printr;
extern int lastJobIndex;

PrintStatusScene::PrintStatusScene(String jobFilePath, Project project, Job job):
  SidebarSceneController::SidebarSceneController(),
  _jobFilePath(jobFilePath),
  _project(project),
  _job(job) {
  printr.setListener(this);
}

PrintStatusScene::~PrintStatusScene() {
  printr.setListener(nullptr);
}

UIBitmap * PrintStatusScene::getSidebarBitmap() {
  return &uiBitmaps.sidebar_printing;
}


UIBitmap * PrintStatusScene::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

String PrintStatusScene::getName() {
  return "PrintStatusScene";
}

void PrintStatusScene::onWillAppear() {

  // start the print only if not running already (in case we are returning from CancelPrint scene)
  if (!printr.isPrinting()) {
    _totalJobLines = printr.startJob(_jobFilePath);
  } else {
    _totalJobLines = printr.getTotalJobLines();
  }

  // display job image
  String _projectFilePath = String("/projects/" + String(_project.index));
  _imageLayer = new SDBitmapLayer(Rect(0,0,270,240));
  _imageLayer->setBitmap(_projectFilePath.c_str(), 270,240,  129675 + (129899 * lastJobIndex) + 299);
  Display.setFixedBackgroundLayer(_imageLayer);

  // job title
  _nameLayer = new TransparentTextLayer(Rect(15,10,Display.getLayoutWidth()-30,25));
  _nameLayer->setTextAlign(TEXTALIGN_LEFT);
  _nameLayer->setFont(&LiberationSans_14);
  _nameLayer->setText(String("Printing: ") + String(_job.title));
  _nameLayer->setForegroundColor(ILI9341_BLACK);
  Display.addLayer(_nameLayer);

  // print time
  _printTime = new TransparentTextLayer(Rect(10,100,Display.getLayoutWidth()-30,60));
  _printTime->setTextAlign(TEXTALIGN_LEFT);
  _printTime->setFont(&LiberationSans_12);
  _printTime->setText(String("Print Time: ") + printr.getPrintTime());
  _printTime->setForegroundColor(ILI9341_BLACK);
  Display.addLayer(_printTime);

  // Resolution
  _resolution = new TransparentTextLayer(Rect(10,120,Display.getLayoutWidth()-30,60));
  _resolution->setTextAlign(TEXTALIGN_LEFT);
  _resolution->setFont(&LiberationSans_12);
  _resolution->setText(String("Resolution: ") + printr.getResolution());
  _resolution->setForegroundColor(ILI9341_BLACK);
  Display.addLayer(_resolution);

  // Infill
  _infill = new TransparentTextLayer(Rect(10,140,Display.getLayoutWidth()-30,60));
  _infill->setTextAlign(TEXTALIGN_LEFT);
  _infill->setFont(&LiberationSans_12);
  _infill->setText(String("Infill: ") + printr.getInfill());
  _infill->setForegroundColor(ILI9341_BLACK);
  Display.addLayer(_infill);

  // Support
  _support = new TransparentTextLayer(Rect(10,160,Display.getLayoutWidth()-30,60));
  _support->setTextAlign(TEXTALIGN_LEFT);
  _support->setFont(&LiberationSans_12);
  _support->setText(String("Print Support: ") + printr.getSupport());
  _support->setForegroundColor(ILI9341_BLACK);
  Display.addLayer(_support);

  // Filament required
  _filament = new TransparentTextLayer(Rect(10,180,Display.getLayoutWidth()-30,60));
  _filament->setTextAlign(TEXTALIGN_LEFT);
  _filament->setFont(&LiberationSans_12);
  _filament->setText(String("Filament required: ") + printr.getFilamentLength());
  _filament->setForegroundColor(ILI9341_BLACK);
  Display.addLayer(_filament);


  // progress bar
  _progressBar = new ProgressBar(Rect(0,228,Display.getLayoutWidth(),12));
  _progressBar->setValue(0.0f);
  addView(_progressBar);

  SidebarSceneController::onWillAppear();
}

void PrintStatusScene::printrCallback(const char ctype[], float * fdata, int * idata) {
  PRINTER_NOTICE("Got printer callback: %s",ctype);
  if (strcmp(ctype,"end") == 0) {
    printr.setListener(nullptr);
    FinishPrint * scene = new FinishPrint(_jobFilePath, _project, _job);
    Application.pushScene(scene);
  }
  else if ( _totalJobLines != -1 && strcmp(ctype,"line") == 0) {
    // update the progress bar here...
    float _v = (float) *idata / (float) _totalJobLines;
    _progressBar->setValue(_v);
  }
}


void PrintStatusScene::loop()
{
  SceneController::loop();
}



void PrintStatusScene::buttonPressed(void *button) {
  printr.setListener(nullptr);
  ConfirmCancelPrint * scene = new ConfirmCancelPrint(_jobFilePath, _project, _job);
  Application.pushScene(scene);
}
