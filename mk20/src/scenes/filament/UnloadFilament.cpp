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

  BitmapView* icon = new BitmapView(Rect(10,24,uiBitmaps.unload_scene.width, uiBitmaps.unload_scene.height));
  icon->setBitmap(&uiBitmaps.unload_scene);
  addView(icon);

  _doneBtn = new BitmapButton(Rect(15,178, uiBitmaps.btn_done.width, uiBitmaps.btn_done.height));
  _doneBtn->setBitmap(&uiBitmaps.btn_done);
  _doneBtn->setDelegate(this);
  addView(_doneBtn);

  printr.sendLine("G1 A-200 F9000");

  SidebarSceneController::onWillAppear();
}


void UnloadFilament::onSidebarButtonTouchUp() {

}

void UnloadFilament::buttonPressed(void *button)
{
  printr.stopAndFlush();
  printr.sendWaitCommand(1000);
  printr.sendLine("M100({_leds:4})");
  printr.sendLine("G0 Y150");
  printr.sendLine("G0 Z30");
  printr.turnOffHotend();

  SettingsScene * scene = new SettingsScene();
  Application.pushScene(scene);
}
