#include "ConfirmDeleteProject.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "font_LiberationSans.h"
#include "ProjectsScene.h"

extern UIBitmaps uiBitmaps;

ConfirmDeleteProject::ConfirmDeleteProject(String filePath):
  SidebarSceneController::SidebarSceneController(),
  _filePath(filePath)
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
  return NULL;
}


uint16_t ConfirmDeleteProject::getBackgroundColor()
{
  return Application.getTheme()->getColor(BackgroundColor);
}

void ConfirmDeleteProject::onWillAppear() {

  BitmapView* icon = new BitmapView(Rect(90,20,uiBitmaps.icon_alert.width, uiBitmaps.icon_alert.height));
  icon->setBitmap(&uiBitmaps.icon_alert);
  addView(icon);

/*
  TextLayer* textLayer = new TextLayer(Rect(34,118, 197, 20));
  textLayer->setFont(&LiberationSans_14);
  textLayer->setTextAlign(TEXTALIGN_CENTERED);
  textLayer->setForegroundColor(ILI9341_WHITE);
  textLayer->setText("Are you sure");
  Display.addLayer(textLayer);
*/
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


void ConfirmDeleteProject::onSidebarButtonTouchUp() {
  // todo
  ProjectsScene * scene = new ProjectsScene();
  Application.pushScene(scene);
}

void ConfirmDeleteProject::handleTouchMoved(TS_Point point, TS_Point oldPoint) {
  SceneController::handleTouchMoved(point, oldPoint);
}

void ConfirmDeleteProject::animationFinished(Animation *animation) {
  SceneController::animationFinished(animation);
}

void ConfirmDeleteProject::buttonPressed(void *button)
{
  if (button == _noBtn) {
    ProjectsScene * scene = new ProjectsScene();
    Application.pushScene(scene);
  } else if (button == _yesBtn) {

    // remove project index
    SD.remove(_filePath.c_str());

    // remove job directory and all files in it
    _filePath.remove(0,9);
    String jobsFolderPath = String("/jobs" + _filePath);

    if (SD.exists(jobsFolderPath.c_str())) {
      File jobsFolder = SD.open(jobsFolderPath.c_str());

      while (true) {
        File jf = jobsFolder.openNextFile();
        if (!jf) {
          break;
        }
        String jfn = jobsFolderPath + String("/");
        jfn = jfn + String(jf.name());
        jf.close();
        SD.remove(jfn.c_str());
      }

      // remove now empty directory
      SD.rmdir(jobsFolderPath.c_str());
    }


    ProjectsScene * scene = new ProjectsScene();
    Application.pushScene(scene);
  }

  SidebarSceneController::buttonPressed(button);
}
