/*
 * Sets nozzle temperature of printer to 200 degree and captures temp feedback
 * from printer to show a progress bar. Either pushes Load or Unload filament
 * scenes after temperature has reached.
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/behind-the-scenes-printrbot-simple-2016/
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Mick Balaban
 * https://github.com/Printrbot/Printrhub
 *
 * Developed in cooperation with Phillip Schuster (@appfruits) from appfruits.com
 * http://www.appfruits.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

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

PreheatExtruder::PreheatExtruder() :
	SidebarSceneController::SidebarSceneController() {

}

PreheatExtruder::~PreheatExtruder() {
  printr.setListener(nullptr);
}

String PreheatExtruder::getName() {
  return "PreheatExtruder";
}

UIBitmap *PreheatExtruder::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

UIBitmap *PreheatExtruder::getSidebarBitmap() {
  return &uiBitmaps.sidebar_filament;
}

uint16_t PreheatExtruder::getBackgroundColor() {
  return Application.getTheme()->getColor(BackgroundColor);
}

void PreheatExtruder::onWillAppear() {

  BitmapView *icon = new BitmapView(Rect(0, 0, uiBitmaps.heating_screen.width, uiBitmaps.heating_screen.height - 5));
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
  _progressBar = new ProgressBar(Rect(0, 228, 270, 12));
  _progressBar->setBackgroundColor(RGB565(255, 56, 38));
  _progressBar->setValue(0.0f);
  addView(_progressBar);

  //printr.sendLine("G28.2 X0 Y0 Z0\nM100 ({he1st:50});\nM101 ({he1at:t});\nG0 Z40\nG0 X100 Y50\nM2");



  SidebarSceneController::onWillAppear();
}

void PreheatExtruder::onDidAppear() {
  printr.setListener(this);
  printr.reset();
  printr.sendWaitCommand(1000);
  printr.homeY();
  // start listening for temperature
  printr.startListening();
  printr.sendWaitCommand(500);
  printr.sendLine("M100({he1st:200})");
  printr.sendLine("M100({_leds:2})"); // red
  printr.sendLine("M101({he1at:t})");

  if (!printr.isHomed()) {
	printr.homeZ();
  }
  printr.sendLine("G0 Z40");
  printr.sendLine("G0 Y30");

  //Set as Printr listener


  SidebarSceneController::onDidAppear();
}

void PreheatExtruder::onNewNozzleTemperature(float temp) {
  float v = temp / 200.0f;
  _progressBar->setValue(v);

  if (temp >= 200.0f) {
	if (_nextScene == 1) {
	  UnloadFilament *scene = new UnloadFilament();
	  Application.pushScene(scene);
	} else if (_nextScene == 2) {
	  // load filament scene
	  LoadFilament *scene = new LoadFilament();
	  Application.pushScene(scene);
	} else {
	  SelectFilamentAction *scene = new SelectFilamentAction();
	  Application.pushScene(scene);
	}
  }
}

void PreheatExtruder::onPrintProgress(float progress) {
}

void PreheatExtruder::onPrintComplete(bool success) {
}

void PreheatExtruder::onSidebarButtonTouchUp() {
  // flush the queue
  printr.stopAndFlush();
  printr.sendWaitCommand(1000);
  printr.sendLine("M100({_leds:4})");
  printr.sendLine("G0 Y150");
  printr.turnOffHotend();
  printr.stopListening();

  SelectFilamentAction *scene = new SelectFilamentAction();
  Application.pushScene(scene);
}

void PreheatExtruder::buttonPressed(void *button) {
  SidebarSceneController::buttonPressed(button);
}
