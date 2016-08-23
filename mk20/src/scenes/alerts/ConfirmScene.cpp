#include "ConfirmScene.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "font_LiberationSans.h"

extern UIBitmaps uiBitmaps;

ConfirmScene::ConfirmScene(): SidebarSceneController::SidebarSceneController() {
}

ConfirmScene::~ConfirmScene() {
}


String ConfirmScene::getName() {
  return "ConfirmScene";
}


UIBitmap * ConfirmScene::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap * ConfirmScene::getSidebarBitmap() {
  return NULL;
}


uint16_t ConfirmScene::getBackgroundColor()
{
  return Application.getTheme()->getColor(BackgroundColor);
}

void ConfirmScene::onWillAppear() {

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
  addView(_yesBtn);

  _noBtn = new BitmapButton(Rect(137,178, uiBitmaps.btn_no.width, uiBitmaps.btn_no.height));
  _noBtn->setBitmap(&uiBitmaps.btn_no);
  addView(_noBtn);

  SidebarSceneController::onWillAppear();
}


void ConfirmScene::onSidebarButtonTouchUp() {
  // todo
}

void ConfirmScene::handleTouchMoved(TS_Point point, TS_Point oldPoint) {
  SceneController::handleTouchMoved(point, oldPoint);
}

void ConfirmScene::animationFinished(Animation *animation) {
  SceneController::animationFinished(animation);
}

void ConfirmScene::buttonPressed(void *button)
{
  SidebarSceneController::buttonPressed(button);
}
