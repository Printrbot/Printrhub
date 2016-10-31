#include "ConfirmDeleteProject.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "font_LiberationSans.h"
#include "ProjectsScene.h"

extern UIBitmaps uiBitmaps;
extern int lastProjectIndex;
extern int lastJobIndex;

ConfirmDeleteProject::ConfirmDeleteProject(Project project):
  SidebarSceneController::SidebarSceneController(),
  _project(project)
{
}

ConfirmDeleteProject::~ConfirmDeleteProject() {
}


String ConfirmDeleteProject::getName() {
  return "ConfirmDeleteProject";
}


UIBitmap * ConfirmDeleteProject::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap * ConfirmDeleteProject::getSidebarBitmap() {
  return &uiBitmaps.sidebar_project;
}


uint16_t ConfirmDeleteProject::getBackgroundColor()
{
  return Application.getTheme()->getColor(BackgroundColor);
}

void ConfirmDeleteProject::onWillAppear() {
  setScrollSnap(Display.getLayoutWidth(), SnapMode::Flick);

  BitmapView* icon = new BitmapView(Rect(90,20,uiBitmaps.icon_alert.width, uiBitmaps.icon_alert.height));
  icon->setBitmap(&uiBitmaps.icon_alert);
  addView(icon);

  _yesBtn = new BitmapButton(Rect(17,112, uiBitmaps.btn_delete_project.width, uiBitmaps.btn_delete_project.height));
  _yesBtn->setBitmap(&uiBitmaps.btn_delete_project);
  _yesBtn->setDelegate(this);
  addView(_yesBtn);

  _noBtn = new BitmapButton(Rect(17,172, uiBitmaps.btn_cancel.width, uiBitmaps.btn_cancel.height));
  _noBtn->setDelegate(this);
  _noBtn->setBitmap(&uiBitmaps.btn_cancel);
  addView(_noBtn);

  SidebarSceneController::onWillAppear();
}

void ConfirmDeleteProject::onDidAppear() {
  SidebarSceneController::onDidAppear();
}

void ConfirmDeleteProject::onSidebarButtonTouchUp() {
  ProjectsScene * scene = new ProjectsScene();
  Application.pushScene(scene);
}

void ConfirmDeleteProject::buttonPressed(void *button)
{
  if (button == _noBtn) {
    ProjectsScene * scene = new ProjectsScene();
    Application.pushScene(scene);
  } else if (button == _yesBtn) {
    IndexDb * idb = new IndexDb();
    idb->deleteProject(_project);
    lastJobIndex = 0;
    lastProjectIndex = lastProjectIndex > 0 ? lastProjectIndex-1 : 0;

    ProjectsScene * scene = new ProjectsScene();
    Application.pushScene(scene);
  }

  SidebarSceneController::buttonPressed(button);
}
