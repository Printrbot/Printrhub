//
// Created by Phillip Schuster on 27.09.16.
//

#include "ErrorScene.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "font_LiberationSans.h"
#include "../projects/ProjectsScene.h"

extern UIBitmaps uiBitmaps;

ErrorScene::ErrorScene(const String& errorMessage, bool showButton): SidebarSceneController::SidebarSceneController() {
  _errorMessage = errorMessage;
  _showButton = showButton;
}

ErrorScene::~ErrorScene() {
}


String ErrorScene::getName() {
  return "ErrorScene";
}


UIBitmap * ErrorScene::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap * ErrorScene::getSidebarBitmap() {
  return NULL;
}


uint16_t ErrorScene::getBackgroundColor()
{
  return Application.getTheme()->getColor(BackgroundColor);
}

void ErrorScene::onWillAppear() {

  BitmapView* icon = new BitmapView(Rect(100,24,uiBitmaps.icon_alert.width, uiBitmaps.icon_alert.height));
  icon->setBitmap(&uiBitmaps.icon_alert);
  addView(icon);

  LabelView* labelView = new LabelView(_errorMessage,Rect(34,118,197,20));
  labelView->setFont(&LiberationSans_14);
  labelView->setTextAlign(TEXTALIGN_CENTERED);
  addView(labelView);

  if (_showButton)
  {
    _okBtn = new LabelButton("OK",Rect(50,178,170,38));
    _okBtn->setBackgroundColor(ILI9341_DARKGREEN);
    _okBtn->setTextColor(ILI9341_WHITE);
    _okBtn->setDelegate(this);
    addView(_okBtn);
  }


  SidebarSceneController::onWillAppear();
}

void ErrorScene::onSidebarButtonTouchUp() {
  ProjectsScene* scene = new ProjectsScene();
  Application.pushScene(scene);
}

void ErrorScene::buttonPressed(void *button)
{
  if (button == _okBtn)
  {
    ProjectsScene* scene = new ProjectsScene();
    Application.pushScene(scene);
  }
}
