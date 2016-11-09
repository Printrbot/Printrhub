#include "FinishPrint.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "Printr.h"
#include "UIBitmaps.h"
#include "scenes/projects/ProjectsScene.h"
#include "scenes/projects/JobsScene.h"
#include "scenes/print/PrintStatusScene.h"

extern UIBitmaps uiBitmaps;
extern Printr printr;


FinishPrint::FinishPrint(String jobFilePath, Project project, Job job):
    SidebarSceneController::SidebarSceneController(),
    _jobFilePath(jobFilePath),
    _project(project),
    _job(job){
}

FinishPrint::~FinishPrint() {
}


String FinishPrint::getName() {
  return "FinishPrint";
}


UIBitmap * FinishPrint::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap * FinishPrint::getSidebarBitmap() {
  return &uiBitmaps.sidebar_printing;
}

void FinishPrint::animationFinished(Animation *animation) {
  SceneController::animationFinished(animation);
}

uint16_t FinishPrint::getBackgroundColor()
{
  return Application.getTheme()->getColor(BackgroundColor);
}

void FinishPrint::onWillAppear() {
  BitmapView* icon = new BitmapView(Rect(26,18,uiBitmaps.job_finish_scene.width, uiBitmaps.job_finish_scene.height));
  icon->setBitmap(&uiBitmaps.job_finish_scene);
  addView(icon);

  _btnDone = new BitmapButton(Rect(16,176, uiBitmaps.btn_done.width, uiBitmaps.btn_done.height));
  _btnDone->setBitmap(&uiBitmaps.btn_done);
  _btnDone->setDelegate(this);
  addView(_btnDone);

  SidebarSceneController::onWillAppear();
}


void FinishPrint::onSidebarButtonTouchUp() {
  PrintStatusScene * scene = new PrintStatusScene(_jobFilePath, _project, _job);
  Application.pushScene(scene);
}

void FinishPrint::handleTouchMoved(TS_Point point, TS_Point oldPoint) {
  SceneController::handleTouchMoved(point, oldPoint);
}

void FinishPrint::buttonPressed(void *button)
{
  // move print head out of the way
  printr.sendLine("G0 Y150"); // move head out of the way
  // set the light to blue
  printr.sendLine("M100({_leds:4})"); //switch to blue light

  JobsScene * js = new JobsScene(_project);
  Application.pushScene(js);

  //SidebarSceneController::buttonPressed(button);
}
