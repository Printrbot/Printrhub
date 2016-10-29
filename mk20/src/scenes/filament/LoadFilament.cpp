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
  BitmapView* icon = new BitmapView(Rect(10,24,uiBitmaps.load_scene.width, uiBitmaps.load_scene.height));
  icon->setBitmap(&uiBitmaps.load_scene);
  addView(icon);

  _doneBtn = new BitmapButton(Rect(15,178, uiBitmaps.btn_done.width, uiBitmaps.btn_done.height));
  _doneBtn->setBitmap(&uiBitmaps.btn_done);
  _doneBtn->setDelegate(this);
  addView(_doneBtn);

  printr.sendLine("M100({_leds:1})");
  printr.sendLine("G1 A1000 F2000");

  SidebarSceneController::onWillAppear();
}


void LoadFilament::onSidebarButtonTouchUp() {

}

void LoadFilament::buttonPressed(void *button)
{
  printr.stopAndFlush();
  printr.sendWaitCommand(1000);
  printr.sendLine("M100({_leds:4})");
  printr.sendLine("G0 Y150");
  printr.sendLine("G0 Z30");
  printr.turnOffHotend();

  SettingsScene * scene = new SettingsScene();
  Application.pushScene(scene);

  //SidebarSceneController::buttonPressed(button);
}
