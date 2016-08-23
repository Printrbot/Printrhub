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
  return NULL;
}


uint16_t NoProjects::getBackgroundColor()
{
  return Application.getTheme()->getColor(BackgroundColor);
}

void NoProjects::onWillAppear() {

  BitmapView* icon = new BitmapView(Rect(100,44,uiBitmaps.icon_alert.width, uiBitmaps.icon_alert.height));
  icon->setBitmap(&uiBitmaps.icon_alert);
  addView(icon);

  TextLayer* textLayer = new TextLayer(Rect(34,138, 197, 20));
  textLayer->setFont(&LiberationSans_14);
  textLayer->setTextAlign(TEXTALIGN_CENTERED);
  textLayer->setForegroundColor(ILI9341_WHITE);
  textLayer->setText("No projects available :(");
  Display.addLayer(textLayer);

  SidebarSceneController::onWillAppear();
}


void NoProjects::onSidebarButtonTouchUp() {
  SettingsScene * scene = new SettingsScene();
  Application.pushScene(scene);
}

void NoProjects::buttonPressed(void *button) {
  SidebarSceneController::buttonPressed(button);
}
