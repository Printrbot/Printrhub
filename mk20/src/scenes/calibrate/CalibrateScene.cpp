#include "CalibrateScene.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "font_LiberationSans.h"
#include "../settings/SettingsScene.h"
#include "Printr.h"

extern UIBitmaps uiBitmaps;
extern Printr printr;

CalibrateScene::CalibrateScene():
  SidebarSceneController::SidebarSceneController()
{
}

CalibrateScene::~CalibrateScene() {
}


String CalibrateScene::getName() {
  return "Calibrate";
}


UIBitmap * CalibrateScene::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap * CalibrateScene::getSidebarBitmap() {
  return &uiBitmaps.sidebar_calibrate;
}


uint16_t CalibrateScene::getBackgroundColor()
{
  return Application.getTheme()->getColor(BackgroundColor);
}

void CalibrateScene::onWillAppear() {


  BitmapView* hb = new BitmapView(Rect(16,10,uiBitmaps.hotend.width, uiBitmaps.hotend.height));
  hb->setBitmap(&uiBitmaps.hotend);
  addView(hb);

  _offset = 0.1;

  TransparentTextLayer* _offsetText = new TransparentTextLayer(Rect(84,50, 150, 24));
  _offsetText->setFont(&LiberationSans_32);
  _offsetText->setTextAlign(TEXTALIGN_CENTERED);
  _offsetText->setForegroundColor(ILI9341_BLACK);
  _offsetText->setText(_offset);



  Display.addLayer(_offsetText);

  _minusBtn = new BitmapButton(Rect(15,112, uiBitmaps.btn_down.width, uiBitmaps.btn_down.height));
  _minusBtn->setBitmap(&uiBitmaps.btn_down);
  _minusBtn->setDelegate(this);
  addView(_minusBtn);

  _plusBtn = new BitmapButton(Rect(138,112, uiBitmaps.btn_up.width, uiBitmaps.btn_up.height));
  _plusBtn->setBitmap(&uiBitmaps.btn_up);
  _plusBtn->setDelegate(this);
  addView(_plusBtn);

  _saveBtn = new BitmapButton(Rect(15,173, uiBitmaps.btn_save.width, uiBitmaps.btn_save.height));
  _saveBtn->setBitmap(&uiBitmaps.btn_save);
  _saveBtn->setDelegate(this);
  addView(_saveBtn);


  SidebarSceneController::onWillAppear();
}


void CalibrateScene::onSidebarButtonTouchUp() {
  SettingsScene * scene = new SettingsScene();
  Application.pushScene(scene);
}

void CalibrateScene::buttonPressed(void *button)
{
  if (button == _plusBtn) {
    //_offset += 0.1f;
    _offsetText->setText(_offset);
  }
  if (button == _minusBtn) {
    //_offset -= 0.1f;
    _offsetText->setText(_offset);
  }

  SidebarSceneController::buttonPressed(button);
}
