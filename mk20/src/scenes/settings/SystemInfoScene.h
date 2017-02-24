/*
 * Shows an info screen with various settings and info about the printer
 * like IP address, name and serial number
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/behind-the-scenes-printrbot-simple-2016/
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Phillip Schuster
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

#ifndef MK20_SYSTEMINFO_H
#define MK20_SYSTEMINFO_H

#include "../SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/LabelView.h"

class SystemInfoScene : public SidebarSceneController {
 public:

  SystemInfoScene();
  virtual ~SystemInfoScene();
  virtual void onSidebarButtonTouchUp() override;

  virtual uint16_t getBackgroundColor() override;
  virtual UIBitmap *getSidebarBitmap() override;
  virtual UIBitmap *getSidebarIcon() override;

 private:
  virtual void onWillAppear() override;
  String getName() override;
  virtual void buttonPressed(void *button) override;
  virtual void loop() override;
  virtual bool isModal() override;

  virtual bool runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool *success);
  virtual bool handlesTask(TaskID taskID);

  void configureLabelView(LabelView *labelView);
  void queryData();

 private:
  LabelView *_printerNameLabel;
  LabelView *_printerName;
  LabelView *_ipaddressLabel;
  LabelView *_ipaddress;
  LabelView *_macAddressLabel;
  LabelView *_macAddress;
  LabelView *_ssidLabel;
  LabelView *_ssid;
  LabelView *_serialNumberLabel;
  LabelView *_serialNumber;
  LabelView *_firmwareVersionLabel;
  LabelView *_firmwareVersion;
  LabelView *_networkModeLabel;
  LabelView *_networkMode;
  SystemInfo _systemInfo;
  bool _dataReceived;
  unsigned long _lastPing;
};

#endif //MK20_SYSTEMINFO_H
