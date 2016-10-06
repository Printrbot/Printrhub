#include "NoProjects.h"
#include "../SidebarSceneController.h"
#include "font_LiberationSans.h"
#include "../settings/SettingsScene.h"

extern UIBitmaps uiBitmaps;

NoProjects::NoProjects(): SidebarSceneController::SidebarSceneController()
{
}

NoProjects::~NoProjects() {
}


String NoProjects::getName() {
  return "NoProjects";
}


UIBitmap * NoProjects::getSidebarIcon() {
  return &uiBitmaps.btn_settings;
}

UIBitmap * NoProjects::getSidebarBitmap() {
  return &uiBitmaps.sidebar_project;
}


uint16_t NoProjects::getBackgroundColor()
{
  return Application.getTheme()->getColor(BackgroundColor);
}

void NoProjects::onWillAppear() {

  BitmapView* icon = new BitmapView(Rect(0,0,uiBitmaps.scene_empty_project.width, uiBitmaps.scene_empty_project.height));
  icon->setBitmap(&uiBitmaps.scene_empty_project);
  addView(icon);
/*
  TextLayer* textLayer = new TextLayer(Rect(34,158, 197, 20));
  textLayer->setFont(&LiberationSans_14);
  textLayer->setTextAlign(TEXTALIGN_CENTERED);
  textLayer->setForegroundColor(ILI9341_WHITE);
  textLayer->setText("No projects available :(");
  Display.addLayer(textLayer);
*/
  SidebarSceneController::onWillAppear();
}


void NoProjects::onSidebarButtonTouchUp() {
  SettingsScene * scene = new SettingsScene();
  Application.pushScene(scene);
}

void NoProjects::buttonPressed(void *button) {
  SidebarSceneController::buttonPressed(button);
}
