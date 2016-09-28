#include "PreheatExtruder.h"
#include "framework/views/BitmapButton.h"
#include "../SidebarSceneController.h"
#include "font_LiberationSans.h"
#include "SelectFilamentAction.h"
#include "Printr.h"
#include "UnloadFilament.h"
#include "LoadFilament.h"

extern UIBitmaps uiBitmaps;
extern Printr printr;

PreheatExtruder::PreheatExtruder():
  SidebarSceneController::SidebarSceneController()
{
  printr.setListener(this);
}

PreheatExtruder::~PreheatExtruder() {

}



String PreheatExtruder::getName() {
  return "PreheatExtruder";
}


UIBitmap * PreheatExtruder::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap * PreheatExtruder::getSidebarBitmap() {
  return &uiBitmaps.sidebar_filament;
}

uint16_t PreheatExtruder::getBackgroundColor()
{
  return Application.getTheme()->getColor(BackgroundColor);
}

void PreheatExtruder::onWillAppear() {

  BitmapView* icon = new BitmapView(Rect(0,0,uiBitmaps.heating_screen.width, uiBitmaps.heating_screen.height - 5));
  icon->setBitmap(&uiBitmaps.heating_screen);
  addView(icon);

/*
  TextLayer* textLayer = new TextLayer(Rect(5, 200, 260, 20));
  textLayer->setFont(&LiberationSans_12);
  textLayer->setTextAlign(TEXTALIGN_CENTERED);
  textLayer->setForegroundColor(ILI9341_WHITE);
  textLayer->setText("Heating extruder, please wait...");
  Display.addLayer(textLayer);
*/
  _progressBar = new ProgressBar(Rect(0,235,270,5));
	//_progressBar->setTrackColor(ILI9341_RED);
	_progressBar->setValue(0.0f);
	addView(_progressBar);

  //printr.sendLine("G28.2 X0 Y0 Z0\nM100 ({he1st:50});\nM101 ({he1at:t});\nG0 Z40\nG0 X100 Y50\nM2");

  printr.homeY();
  printr.homeX();

  printr.sendLine("G0 X105");
  printr.sendLine("M100({he1st:200})");
  printr.sendLine("M100({_leds:2})");
  printr.sendLine("M101({he1at:t})");
  if (!printr.isHomed()) {
    printr.homeZ();
  }
  printr.sendLine("G0 Z40");
  printr.sendLine("N10 G0 Y30");

  SidebarSceneController::onWillAppear();
}

void PreheatExtruder::printrCallback(const char ctype[], float * fdata, int * idata) {
  if (strcmp(ctype,"he1t") == 0) {
    float _v = *fdata / 200;
    _progressBar->setValue(_v);
  } else if (strcmp(ctype,"line") == 0) {
    // show instruction scene with pullout text
    if (_nextScene == 1) {
      UnloadFilament * scene = new UnloadFilament();
      Application.pushScene(scene);
    }
    else if (_nextScene == 2) {
      // load filament scene
      LoadFilament * scene = new LoadFilament();
      Application.pushScene(scene);
    }
    else {
      SelectFilamentAction * scene = new SelectFilamentAction();
      Application.pushScene(scene);
    }
  }
}


void PreheatExtruder::onSidebarButtonTouchUp() {
  // flush the queue
  printr.stopAndFlush();
  printr.sendLine("M100({_leds:1})");
  printr.sendLine("G0 Y120");
  printr.turnOffHotend();

  SelectFilamentAction * scene = new SelectFilamentAction();
  Application.pushScene(scene);
}

void PreheatExtruder::buttonPressed(void *button)
{
  SidebarSceneController::buttonPressed(button);
}
