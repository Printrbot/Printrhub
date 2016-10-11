#include "ConfirmCancelPrint.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "Printr.h"
#include "UIBitmaps.h"
#include "scenes/projects/ProjectsScene.h"
#include "scenes/projects/JobsScene.h"
#include "scenes/print/PrintStatusScene.h"

extern UIBitmaps uiBitmaps;
extern Printr printr;


ConfirmCancelPrint::ConfirmCancelPrint(String jobFilePath, Project project, Job job, uint16_t jobOffset):
    SidebarSceneController::SidebarSceneController(),
    _jobFilePath(jobFilePath),
    _project(project),
    _job(job),
    _jobOffset(jobOffset){
}

ConfirmCancelPrint::~ConfirmCancelPrint() {
}


String ConfirmCancelPrint::getName() {
  return "PausePrintScene";
}


UIBitmap * ConfirmCancelPrint::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap * ConfirmCancelPrint::getSidebarBitmap() {
  return NULL;
}

void ConfirmCancelPrint::animationFinished(Animation *animation) {
  SceneController::animationFinished(animation);
}

uint16_t ConfirmCancelPrint::getBackgroundColor()
{
  return Application.getTheme()->getColor(BackgroundColor);
}

void ConfirmCancelPrint::onWillAppear() {


  BitmapView* icon = new BitmapView(Rect(100,24,uiBitmaps.icon_alert.width, uiBitmaps.icon_alert.height));
  icon->setBitmap(&uiBitmaps.icon_alert);
  addView(icon);

  _btnCancelPrint = new BitmapButton(Rect(17,112, uiBitmaps.btn_cancel_print.width, uiBitmaps.btn_cancel_print.height));
  _btnCancelPrint->setBitmap(&uiBitmaps.btn_cancel_print);
  _btnCancelPrint->setDelegate(this);
  addView(_btnCancelPrint);

  _btnBack = new BitmapButton(Rect(17,172, uiBitmaps.btn_back.width, uiBitmaps.btn_back.height));
  _btnBack->setDelegate(this);
  _btnBack->setBitmap(&uiBitmaps.btn_back);
  addView(_btnBack);

  SidebarSceneController::onWillAppear();

}


void ConfirmCancelPrint::onSidebarButtonTouchUp() {
  PrintStatusScene * scene = new PrintStatusScene(_jobFilePath, _project, _job, _jobOffset);
  Application.pushScene(scene);
}

void ConfirmCancelPrint::handleTouchMoved(TS_Point point, TS_Point oldPoint) {
  SceneController::handleTouchMoved(point, oldPoint);
}

void ConfirmCancelPrint::buttonPressed(void *button)
{
  if (button == _btnBack) {
    PrintStatusScene * scene = new PrintStatusScene(_jobFilePath, _project, _job, _jobOffset);
    Application.pushScene(scene);
  } else if (button == _btnCancelPrint) {
    printr.cancelCurrentJob();
    String pi = String("/projects/") + String(_project.index);
    JobsScene * js = new JobsScene(pi, _jobOffset);
    Application.pushScene(js);
  }

  SidebarSceneController::buttonPressed(button);
}
