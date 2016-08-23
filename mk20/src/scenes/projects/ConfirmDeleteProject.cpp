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

  BitmapView* icon = new BitmapView(Rect(100,24,uiBitmaps.icon_alert.width, uiBitmaps.icon_alert.height));
  icon->setBitmap(&uiBitmaps.icon_alert);
  addView(icon);

  TextLayer* textLayer = new TextLayer(Rect(34,118, 197, 20));
  textLayer->setFont(&LiberationSans_14);
  textLayer->setTextAlign(TEXTALIGN_CENTERED);
  textLayer->setForegroundColor(ILI9341_WHITE);
  textLayer->setText("Are you sure");
  Display.addLayer(textLayer);

  _yesBtn = new BitmapButton(Rect(50,178, uiBitmaps.btn_yes.width, uiBitmaps.btn_yes.height));
  _yesBtn->setBitmap(&uiBitmaps.btn_yes);
  _yesBtn->setDelegate(this);
  addView(_yesBtn);

  _noBtn = new BitmapButton(Rect(137,178, uiBitmaps.btn_no.width, uiBitmaps.btn_no.height));
  _noBtn->setDelegate(this);
  _noBtn->setBitmap(&uiBitmaps.btn_no);
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
    SD.remove(_filePath.c_str());
    ProjectsScene * scene = new ProjectsScene();
    Application.pushScene(scene);
  }

  SidebarSceneController::buttonPressed(button);
}
