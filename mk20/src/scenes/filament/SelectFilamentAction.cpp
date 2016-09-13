#include "SelectFilamentAction.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "font_LiberationSans.h"
#include "../settings/SettingsScene.h"
#include "PreheatExtruder.h"
#include "UnloadFilament.h"
#include "framework/layers/TransparentTextLayer.h"

extern UIBitmaps uiBitmaps;

SelectFilamentAction::SelectFilamentAction():
  SidebarSceneController::SidebarSceneController()
{
}

SelectFilamentAction::~SelectFilamentAction() {
}


String SelectFilamentAction::getName() {
  return "SelectFilamentAction";
}


UIBitmap * SelectFilamentAction::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap * SelectFilamentAction::getSidebarBitmap() {
  return &uiBitmaps.sidebar_filament;
}


uint16_t SelectFilamentAction::getBackgroundColor()
{
  return Application.getTheme()->getColor(BackgroundColor);
}

void SelectFilamentAction::onWillAppear() {

  BitmapView* icon = new BitmapView(Rect(85,10,uiBitmaps.icon_filament.width, uiBitmaps.icon_filament.height));
  icon->setBitmap(&uiBitmaps.icon_filament);
  addView(icon);

/*
  TextLayer* textLayer = new TextLayer(Rect(10,136, 250, 20));
  textLayer->setFont(&LiberationSans_14);
  textLayer->setTextAlign(TEXTALIGN_CENTERED);
  textLayer->setForegroundColor(ILI9341_WHITE);
  textLayer->setBackgroundColor(getBackgroundColor());
  textLayer->setText("Please select below");
  Display.addLayer(textLayer);
*/



  _loadBtn = new BitmapButton(Rect(15,112, uiBitmaps.btn_load.width, uiBitmaps.btn_load.height));
  _loadBtn->setBitmap(&uiBitmaps.btn_load);
  _loadBtn->setDelegate(this);
  addView(_loadBtn);

  _unloadBtn = new BitmapButton(Rect(15,172, uiBitmaps.btn_unload.width, uiBitmaps.btn_unload.height));
  _unloadBtn->setBitmap(&uiBitmaps.btn_unload);
  _unloadBtn->setDelegate(this);
  addView(_unloadBtn);

  SidebarSceneController::onWillAppear();
}


void SelectFilamentAction::onSidebarButtonTouchUp() {
  SettingsScene * scene = new SettingsScene();
  Application.pushScene(scene);
}

void SelectFilamentAction::buttonPressed(void *button)
{
  if (button == _loadBtn) {
    PreheatExtruder * scene = new PreheatExtruder();
    scene->setNextScene(2);
    Application.pushScene(scene);
  } else if (button == _unloadBtn) {
    PreheatExtruder * scene = new PreheatExtruder();
    scene->setNextScene(1);
    Application.pushScene(scene);
  }

  SidebarSceneController::buttonPressed(button);
}
