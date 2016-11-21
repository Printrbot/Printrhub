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

#ifndef PREHEAT_EXTRUDER_H
#define PREHEAT_EXTRUDER_H

#include "../SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/ProgressBar.h"
#include "../../Printr.h"

class PreheatExtruder : public SidebarSceneController, public PrintrListener {
 public:

  PreheatExtruder();
  virtual ~PreheatExtruder();
  virtual void onSidebarButtonTouchUp() override;
  virtual uint16_t getBackgroundColor() override;
  virtual UIBitmap *getSidebarBitmap() override;
  virtual UIBitmap *getSidebarIcon() override;

  virtual void onNewNozzleTemperature(float temp);
  virtual void onPrintProgress(float progress);
  virtual void onPrintComplete(bool success);

  void setNextScene(uint8_t scene) { _nextScene = scene; };

 private:
  virtual void onWillAppear() override;
  virtual void onDidAppear() override;
  String getName() override;
  virtual void buttonPressed(void *button) override;

  ProgressBar *_progressBar;
  int _targetTemp;
  uint8_t _nextScene;
};

#endif
