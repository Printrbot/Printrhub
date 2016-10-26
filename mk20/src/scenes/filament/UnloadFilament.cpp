#include "UnloadFilament.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "font_LiberationSans.h"
#include "../settings/SettingsScene.h"
#include "PreheatExtruder.h"
#include "Printr.h"
#include "framework/layers/TransparentTextLayer.h"

extern UIBitmaps uiBitmaps;
extern Printr printr;

UnloadFilament::UnloadFilament():
  SidebarSceneController::SidebarSceneController()
{
}

UnloadFilament::~UnloadFilament() {
}


String UnloadFilament::getName() {
  return "UnloadFilament";
}


UIBitmap * UnloadFilament::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap * UnloadFilament::getSidebarBitmap() {
  return &uiBitmaps.sidebar_filament;
}


uint16_t UnloadFilament::getBackgroundColor()
{
  return Application.getTheme()->getColor(BackgroundColor);
}

void UnloadFilament::onWillAppear() {

/*
  BitmapView* icon = new BitmapView(Rect(86,25, uiBitmaps.icon_unload_filament.width, uiBitmaps.icon_unload_filament.height));
  icon->setBitmap(&uiBitmaps.icon_unload_filament);
  addView(icon);


  TextLayer* textLayer = new TextLayer(Rect(10,125, 250, 40));
  textLayer->setFont(&LiberationSans_12);
  textLayer->setTextAlign(TEXTALIGN_CENTERED);
  textLayer->setForegroundColor(ILI9341_WHITE);
  textLayer->setText("Pull the filament out.");
  Display.addLayer(textLayer);
*/

  _doneBtn = new BitmapButton(Rect(80,178, uiBitmaps.btn_done.width, uiBitmaps.btn_done.height));
  _doneBtn->setBitmap(&uiBitmaps.btn_done);
  _doneBtn->setDelegate(this);
  addView(_doneBtn);

  printr.sendLine("G1 A-200 F7000");

  SidebarSceneController::onWillAppear();
}


void UnloadFilament::onSidebarButtonTouchUp() {
  SettingsScene * scene = new SettingsScene();
  Application.pushScene(scene);
}

void UnloadFilament::buttonPressed(void *button)
{
  if (button == _doneBtn) {
    // stop the extruder
    printr.turnOffHotend();
    printr.sendWaitCommand(1000);
    printr.stopAndFlush();
    printr.sendWaitCommand(1000);
    printr.sendLine("M100({_leds:1})");
    printr.sendLine("G0 Y120");
    printr.sendLine("G0 Z30");
    printr.setListener(NULL);
    SettingsScene * scene = new SettingsScene();
    Application.pushScene(scene);
  }

  SidebarSceneController::buttonPressed(button);
}
