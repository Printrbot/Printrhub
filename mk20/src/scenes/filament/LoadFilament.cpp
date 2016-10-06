#include "LoadFilament.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "font_LiberationSans.h"
#include "../settings/SettingsScene.h"
#include "PreheatExtruder.h"
#include "Printr.h"

extern UIBitmaps uiBitmaps;
extern Printr printr;

LoadFilament::LoadFilament():
  SidebarSceneController::SidebarSceneController()
{
}

LoadFilament::~LoadFilament() {
}


String LoadFilament::getName() {
  return "LoadFilament";
}


UIBitmap * LoadFilament::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap * LoadFilament::getSidebarBitmap() {
  return &uiBitmaps.sidebar_filament;
}


uint16_t LoadFilament::getBackgroundColor()
{
  return Application.getTheme()->getColor(BackgroundColor);
}

void LoadFilament::onWillAppear() {

/*
  BitmapView* icon = new BitmapView(Rect(100,24,uiBitmaps.icon_load_filmanet.width, uiBitmaps.icon_load_filmanet.height));
  icon->setBitmap(&uiBitmaps.icon_load_filmanet);
  addView(icon);

  TextLayer* textLayer = new TextLayer(Rect(10,118, 250, 20));
  textLayer->setFont(&LiberationSans_12);
  textLayer->setTextAlign(TEXTALIGN_CENTERED);
  textLayer->setForegroundColor(ILI9341_WHITE);
  textLayer->setText("Start feeding the filament into extruder.\nPress DONE when you see filament coming\n out of the nozzle.");
  Display.addLayer(textLayer);
*/
  _doneBtn = new BitmapButton(Rect(80,178, uiBitmaps.btn_done.width, uiBitmaps.btn_done.height));
  _doneBtn->setBitmap(&uiBitmaps.btn_done);
  _doneBtn->setDelegate(this);
  addView(_doneBtn);
  printr.sendLine("M100({_leds:1})");
  printr.sendLine("G1 A1000 F2000");

  SidebarSceneController::onWillAppear();
}


void LoadFilament::onSidebarButtonTouchUp() {
  SettingsScene * scene = new SettingsScene();
  Application.pushScene(scene);
}

void LoadFilament::buttonPressed(void *button)
{
  if (button == _doneBtn) {
    // stop the extruder
    printr.turnOffHotend();
    printr.stopAndFlush();
    printr.sendLine("G0 Y120");
    printr.sendLine("G0 Z30");
    printr.setListener(NULL);

    SettingsScene * scene = new SettingsScene();
    Application.pushScene(scene);
    return;
  }

  SidebarSceneController::buttonPressed(button);
}
