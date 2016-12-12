#include "LightScene.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "SettingsScene.h"
#include "Printr.h"

extern UIBitmaps uiBitmaps;
extern Printr printr;

LightScene::LightScene(): SidebarSceneController::SidebarSceneController() {
}

LightScene::~LightScene() {
}


String LightScene::getName() {
  return "LightScene";
}

UIBitmap * LightScene::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap * LightScene::getSidebarBitmap() {
  return &uiBitmaps.sidebar_light;
}

uint16_t LightScene::getBackgroundColor()
{
  return Application.getTheme()->getColor(BackgroundColor);
}

void LightScene::onWillAppear() {
  setScrollSnap(Display.getLayoutWidth(), SnapMode::Flick);

  _bulbBtnOn = new BitmapButton(Rect(110, 28, uiBitmaps.light_scene_bulb.width, uiBitmaps.light_scene_bulb.height));
  _bulbBtnOn->setBitmap(&uiBitmaps.light_scene_bulb);
  _bulbBtnOn->setDelegate(this);
  addView(_bulbBtnOn);

  BitmapView* icon = new BitmapView(Rect(70,120,uiBitmaps.light_scene_title.width, uiBitmaps.light_scene_title.height));
  icon->setBitmap(&uiBitmaps.light_scene_title);
  addView(icon);

  _yellowBtn = new BitmapButton(Rect(18,165,uiBitmaps.light_scene_yellow_btn.width,uiBitmaps.light_scene_yellow_btn.height));
  _yellowBtn->setBitmap(&uiBitmaps.light_scene_yellow_btn);
  _yellowBtn->setDelegate(this);
  addView(_yellowBtn);

  _redBtn = new BitmapButton(Rect(78,165,uiBitmaps.light_scene_red_btn.width,uiBitmaps.light_scene_red_btn.height));
  _redBtn->setBitmap(&uiBitmaps.light_scene_red_btn);
  _redBtn->setDelegate(this);
  addView(_redBtn);

  _blueBtn = new BitmapButton(Rect(138,165,uiBitmaps.light_scene_blue_btn.width,uiBitmaps.light_scene_blue_btn.height));
  _blueBtn->setBitmap(&uiBitmaps.light_scene_blue_btn);
  _blueBtn->setDelegate(this);
  addView(_blueBtn);

  _greenBtn = new BitmapButton(Rect(198,165,uiBitmaps.light_scene_green_btn.width,uiBitmaps.light_scene_green_btn.height));
  _greenBtn->setBitmap(&uiBitmaps.light_scene_green_btn);
  _greenBtn->setDelegate(this);
  addView(_greenBtn);


  SidebarSceneController::onWillAppear();
}




void LightScene::onSidebarButtonTouchUp() {
  SettingsScene * scene = new SettingsScene();
  Application.pushScene(scene, true);
}

void LightScene::buttonPressed(void *button)
{
  // 2 = red
  // 1 = white
  // 3 = green
  // 4 = blue
  // 5 = yellow

  if (button == _redBtn) {
    printr.setLightColor(2);
  }
  else if (button == _greenBtn) {
    printr.setLightColor(3);
  }
  else if (button == _yellowBtn) {
    printr.setLightColor(5);
  }
  else if (button == _blueBtn) {
    printr.setLightColor(4);
  }

  else if (button == _bulbBtnOn) {

    if (printr.isLightOn()) {
      printr.turnLightOff();
    } else {
      printr.turnLightOn();
      printr.setLightColor(1); // set white light on
    }
  }

  SidebarSceneController::buttonPressed(button);
}
